// Copyright 2016-2020 Chris Conway (Koderz). All Rights Reserved.

#pragma once

#include "Engine/Engine.h"
#include "Components/MeshComponent.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Stats/Stats.h"
#include "Logging//LogMacros.h"
#include "HAL/CriticalSection.h"
#include "StaticMeshResources.h"
#include "RuntimeMeshCore.generated.h"

#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION < 22
	// This version of the RMC is only supported by engine version 4.22 and above
#endif

DECLARE_STATS_GROUP(TEXT("RuntimeMesh"), STATGROUP_RuntimeMesh, STATCAT_Advanced);

#define RUNTIMEMESH_MAXTEXCOORDS MAX_STATIC_TEXCOORDS
#define RUNTIMEMESH_MAXLODS MAX_STATIC_MESH_LODS

#define RUNTIMEMESH_ENABLE_DEBUG_RENDERING (!(UE_BUILD_SHIPPING || UE_BUILD_TEST) || WITH_EDITOR)


template<typename InElementType>
using TInlineLODArray = TArray<InElementType, TInlineAllocator<RUNTIMEMESH_MAXLODS>>;


// Custom version for runtime mesh serialization
namespace FRuntimeMeshVersion
{
	enum Type
	{
		Initial = 0,
		StaticProviderSupportsSerialization = 1,
		StaticProviderSupportsSerializationV2 = 2,

		// -----<new versions can be added above this line>-------------------------------------------------
		VersionPlusOne,
		LatestVersion = VersionPlusOne - 1
	};

	// The GUID for this custom version
	const static FGuid GUID = FGuid(0xEE48714B, 0x8A2C4652, 0x98BE40E6, 0xCB7EF0E6);
};


class FRuntimeMeshVertexFactory;


/* Mobility status for a RuntimeMeshComponent */
UENUM(BlueprintType)
enum class ERuntimeMeshMobility : uint8
{
	Movable,
	Stationary,
	Static
};

/* Update frequency for a section. Used to optimize for update or render speed*/
UENUM(BlueprintType)
enum class ERuntimeMeshUpdateFrequency : uint8
{
	/* Tries to skip recreating the scene proxy if possible. */
	Average UMETA(DisplayName = "Average"),
	/* Tries to skip recreating the scene proxy if possible and optimizes the buffers for frequent updates. */
	Frequent UMETA(DisplayName = "Frequent"),
	/* If the component is static it will try to use the static rendering path (this will force a recreate of the scene proxy) */
	Infrequent UMETA(DisplayName = "Infrequent")
};

/*
*	Configuration flag for the collision cooking to prioritize cooking speed or collision performance.
*/
UENUM(BlueprintType)
enum class ERuntimeMeshCollisionCookingMode : uint8
{
	/*
	*	Favors runtime collision performance of cooking speed.
	*	This means that cooking a new mesh will be slower, but collision will be faster.
	*/
	CollisionPerformance UMETA(DisplayName = "Collision Performance"),

	/*
	*	Favors cooking speed over collision performance.
	*	This means that cooking a new mesh will be faster, but collision will be slower.
	*/
	CookingPerformance UMETA(DisplayName = "Cooking Performance"),
};

UENUM(BlueprintType)
enum class ERuntimeMeshThreadingPriority : uint8
{
	Normal,
	AboveNormal,
	BelowNormal,
	Highest,
	Lowest,
	SlightlyBelowNormal,
	TimeCritical,
};


/**
*	Struct used to specify a tangent vector for a vertex
*	The Y tangent is computed from the cross product of the vertex normal (Tangent Z) and the TangentX member.
*/
USTRUCT(BlueprintType)
struct FRuntimeMeshTangent
{
	GENERATED_USTRUCT_BODY()

public:

	/** Direction of X tangent for this vertex */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tangent)
	FVector TangentX;

	/** Bool that indicates whether we should flip the Y tangent when we compute it using cross product */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tangent)
	bool bFlipTangentY;

	FRuntimeMeshTangent()
		: TangentX(1.f, 0.f, 0.f)
		, bFlipTangentY(false)
	{}

	FRuntimeMeshTangent(float X, float Y, float Z, bool bInFlipTangentY = false)
		: TangentX(X, Y, Z)
		, bFlipTangentY(bInFlipTangentY)
	{}

	FRuntimeMeshTangent(FVector InTangentX, bool bInFlipTangentY = false)
		: TangentX(InTangentX)
		, bFlipTangentY(bInFlipTangentY)
	{}
};


struct FRuntimeMeshMisc
{
	template<typename LambdaType>
	static void DoOnGameThread(LambdaType&& InFunction)
	{
		if (IsInGameThread())
		{
			InFunction();
		}
		else
		{
			FFunctionGraphTask::CreateAndDispatchWhenReady([InFunction]()
				{
					InFunction();
				}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
	}
};


