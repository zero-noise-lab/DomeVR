// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red,text)

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime> 
// Includes for being able to work with D3D11Viewport reference
#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include "d3d11.h"
#include "Windows/HideWindowsPlatformTypes.h"
#endif
#include "D3D11State.h"
#include "D3D11ShaderResources.h"
#include "D3D11Resources.h"
#include "D3D11Viewport.h"
#include "D3D11RHI.h"
#include "D3D11Util.h"
//
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UObject/Object.h"
#include "Engine.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "HAL/Platform.h"
#include "DomeVRBaseGameMode.generated.h"



/**
 * Enum for definition of possible log types that are selectable in BPs
 *
 */

UENUM(BlueprintType)
enum class LogTypes : uint8 {
	ObjectName				UMETA(DisplayName = "Name of actor"),
	GUID					UMETA(DisplayName = "Unique global ID"),
	IntParameterHeader		UMETA(DisplayName = "Header for int parameters"),
	IntParameter			UMETA(DisplayName = "Int parameters"),
	FloatParameterHeader	UMETA(DisplayName = "Header for float parameters"),
	FloatParameter			UMETA(DisplayName = "Float parameters"),
	VectorParameterHeader	UMETA(DisplayName = "Header for vector parameters"),
	VectorParameter			UMETA(DisplayName = "Vector parameters"),
	SpawnLocation			UMETA(DisplayName = "Location of actor at spawn"),
	DestroyLocation			UMETA(DisplayName = "Location of actor when destroyed"),
	Location				UMETA(DisplayName = "Current Location of an actor"),
	SpawnRotation			UMETA(DisplayName = "Rotation of actor at spawn"),
	DestroyRotation			UMETA(DisplayName = "Rotation of a actor when destroyed"),
	Rotation				UMETA(DisplayName = "Current Rotation of an actor"),
	StringParameterHeader	UMETA(DisplayName = "Header for string parameters"),
	StringParameter			UMETA(DisplayName = "String parameters"),
	BoolParameterHeader		UMETA(DisplayName = "Header for bool parameters"),
	BoolParameter			UMETA(DisplayName = "Bool parameters"),
	Message					UMETA(DisplayName = "Log message given by string"),
	StateMachineName		UMETA(DisplayName = "Name of initialized state machine"),
	TransitionPassed		UMETA(DisplayName = "State machine passed transition"),
	StateChanged			UMETA(DisplayName = "State machine changed state"),
	StateName				UMETA(DisplayName = "Name of initialized state"),
	StateStarted			UMETA(DisplayName = "State starts"),
	StateEnded				UMETA(DisplayName = "State ends"),
	StateInitialized		UMETA(DisplayName = "State initialized"),
	StateShutdown			UMETA(DisplayName = "State shutdown"),
	StateNameDestroy		UMETA(DisplayName = "State destroyed"),
	StateMachineStart		UMETA(DisplayName = "State machine starts"),
	StateMachineStops		UMETA(DisplayName = "State machine stops"),
	Eventmarker				UMETA(DisplayName = "Sent an eventmarker"),
	EventmarkerDescription	UMETA(DisplayName = "Description of the eventmarker"),
	Reward					UMETA(DisplayName = "Sent reward"),
	DigitalInput			UMETA(DisplayName = "Digital event from NiDaq"),
	ChildObjectIDs			UMETA(DisplayName = "Object IDs of child actors"),
	InputData				UMETA(DisplayName = "Axis event input data"),
	PhotodiodeBrightness	UMETA(DisplayName = "Brightness of the photodiode"),
	PhotodiodeColor			UMETA(DisplayName = "Color of the photodiode"),
	ViewportFrameStatistics	UMETA(DisplayName = "Tick information of GameMode"),
	SpawnSpherical			UMETA(DisplayName = "Spherical coordinates of actor at spawn"),
	DestroySpherical		UMETA(DisplayName = "Spherical coordinates of actor when destroyed"),
	Spherical				UMETA(DisplayName = "Current spherical coordinates of an actor")
};


USTRUCT(Blueprintable, BlueprintType)
struct FSimulationSettingsData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Simulation Data")
	FIntPoint ScreenResolution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Simulation Data")
	bool bSwitchSplitScreens;
};

USTRUCT(Blueprintable, BlueprintType)
struct FSessionData
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Session Data")
	FString SelectedLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Session Data")
	FString SelectedStateMachine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Session Data")
	FString Subject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Session Data")
	FString Experiment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Session Data")
	FString Session;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Session Data")
	FString Experimenter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Session Data")
	FString Resolution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Session Data")
	FString ViewportResolution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Session Data")
	FString RefreshRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Session Data")
	FString VSyncState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Session Data")
	FString SplitScreenOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Session Data")
	FString RenderingMethod;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Session Data")
	FString CaptureResolution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Session Data")
	FString FisheyeResolution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Session Data")
	FString InputForwardScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Session Data")
	FString InputSidewardScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Session Data")
	FString InputRotationScale;
};


/**
 * Definition of structs for log data exchange
 *
 * FLogData defines basic log data structure
 *
 * FLogData text defines log data structure for text
 * FLogDataInt defines log data for single integers
 * FLogDataIntArray defines log data structure for an array of integers
 * FLogDataFloat defines log data structure for a single float value
 * FLogDataFloatArray defines log data structure for an array of float
 * FLogDataVector defines log data structure for a vector
 * FLogDataVectorArray defines log data structure for an array of vectors
 */

USTRUCT(Blueprintable, BlueprintType)
struct FLogData
{

	GENERATED_BODY()

	FString COL_SEP = FString(TEXT(","));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR BaseGameMode Logging Data")
		float Time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR BaseGameMode Logging Data")
		LogTypes LogType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR BaseGameMode Logging Data")
		FString OID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR BaseGameMode Logging Data")
		int32 VerbosityLevel;

	int32 GetVerbosityLevel()
	{
		return VerbosityLevel;
	}

	FString GetLogString()
	{	
		FString LogString = FString::Printf(TEXT("%f"), Time) + COL_SEP 
							+ FString::Printf(TEXT("[%s]"), *OID) 
							+ COL_SEP + FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(LogType)) 
							+ COL_SEP;
		return LogString;
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FLogDataText : public FLogData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR BaseGameMode Logging Data")
		FString LogText;

	FString GetLogString()
	{
		FString LogString = Super::GetLogString();
		LogString = LogString + FString::Printf(TEXT("%s"), *LogText);
		return LogString;
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FLogDataInt : public FLogData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR BaseGameMode Logging Data")
		int LogInt;

	FString GetLogString()
	{
		FString LogString = Super::GetLogString();
		LogString = LogString + FString::Printf(TEXT("%i"), LogInt);
		return LogString;
	}

};

USTRUCT(Blueprintable, BlueprintType)
struct FLogDataIntArray : public FLogData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR BaseGameMode Logging Data")
		TArray<int> LogInt;

	FString GetLogString()
	{

		uint8 Len = LogInt.Num();

		if (Len == 0)
		{
			FString LogString = "";
			return LogString;
		}

		FString LogString = Super::GetLogString();
		LogString = LogString + FString::Printf(TEXT("%i"), LogInt[0]);

		for (uint8 i = 1; i < Len; ++i)
		{
			LogString = LogString + COL_SEP + FString::Printf(TEXT("%i"), LogInt[i]);
		}

		return LogString;
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FLogDataEventmarker : public FLogData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR BaseGameMode Logging Data")
		int Eventmarker;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR BaseGameMode Logging Data")
		int64 QPC;

	FString GetLogString()
	{
		FString LogString = Super::GetLogString();
		LogString = LogString + FString::Printf(TEXT("%i,%llu"), Eventmarker, QPC);
		return LogString;
	}

};

USTRUCT(Blueprintable, BlueprintType)
struct FLogDataFloat : public FLogData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR BaseGameMode Logging Data")
		float LogFloat;

	FString GetLogString()
	{
		FString LogString = Super::GetLogString();
		LogString = LogString + FString::Printf(TEXT("%f"), LogFloat);
		return LogString;
	}

};

USTRUCT(Blueprintable, BlueprintType)
struct FLogDataFloatArray : public FLogData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR BaseGameMode Logging Data")
		TArray<float> LogFloat;

	FString GetLogString()
	{

		uint8 Len = LogFloat.Num();

		if (Len == 0)
		{
			FString LogString = "";
			return LogString;
		}

		FString LogString = Super::GetLogString();
		LogString = LogString + FString::Printf(TEXT("%f"), LogFloat[0]);

		for (uint8 i = 1; i < Len; ++i)
		{
			LogString = LogString + COL_SEP + FString::Printf(TEXT("%f"), LogFloat[i]);
		}

		return LogString;
	}

};

USTRUCT(Blueprintable, BlueprintType)
struct FLogDataVector : public FLogData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR BaseGameMode Logging Data")
		FVector LogVector;

	FString GetLogString()
	{
		FString LogString = Super::GetLogString();
		LogString = LogString + LogVector.ToString();
		return LogString;
	}

};

USTRUCT(Blueprintable, BlueprintType)
struct FLogDataVectorArray : public FLogData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR BaseGameMode Logging Data")
		TArray<FVector> LogVector;

	FString GetLogString()
	{

		uint8 Len = LogVector.Num();

		if (Len == 0)
		{
			FString LogString = "";
			return LogString;
		}

		FString LogString = Super::GetLogString();
		LogString = LogString + LogVector[0].ToString();

		for (uint8 i = 1; i < Len; ++i)
		{
			LogString = LogString + COL_SEP + LogVector[1].ToString();
		}

		return LogString;
	}

};

USTRUCT(Blueprintable, BlueprintType)
struct FLogDataGUID : public FLogData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR BaseGameMode Logging Data")
		FGuid LogGuid;

	FString GetLogString()
	{
		FString LogString = Super::GetLogString();
		LogString = LogString + LogGuid.ToString();

		return LogString;
	}

};

USTRUCT(Blueprintable, BlueprintType)
struct FLogDataUE4ObjectArray : public FLogData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR BaseGameMode Logging Data")
		TArray<UObject*> LogUE4Object;

	FString GetLogString()
	{

		uint8 Len = LogUE4Object.Num();

		if (Len == 0)
		{
			FString LogString = "";
			return LogString;
		}

		FString LogString = Super::GetLogString();
		uint32 UniqueID;
		if (IsValid(LogUE4Object[0]))
		{
			UniqueID = LogUE4Object[0]->GetUniqueID();

		}
		else
		{
			UniqueID = 0x00000000;
		}

		LogString = LogString + FString::Printf(TEXT("%d"), UniqueID);

		for (uint8 i = 1; i < Len; ++i)
		{
			if (IsValid(LogUE4Object[i]))
			{
				UniqueID = LogUE4Object[i]->GetUniqueID();

			}
			else
			{
				UniqueID = 0x00000000;
			}
			LogString = LogString + COL_SEP + FString::Printf(TEXT("%d"), UniqueID);
		}

		return LogString;
	}

};

/**
 * Base GameMode class of DomeVR project that implements the log writer module
 */
UCLASS()
class DOMEVR_API ADomeVRBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:

	TArray<FString> ActLog;
	TArray<FString> ActLogContinuous;
	bool bLoadViewportReference = true;

protected:

	FString FileName;
	FString WorldName = "NO_WORLD_LOADED";
	FString SaveString;
	FString HeaderString;
	bool bHeaderWritten = false;

public:

	UPROPERTY(Blueprintable, BlueprintReadWrite, BlueprintReadWrite, EditAnywhere, Category = "DomeVR BaseGameMode|Logging")
	int32 VerbosityLevel = 0; // Standard value of log verbosity level

	UPROPERTY(Blueprintable, BlueprintReadOnly, EditAnywhere, Category = "DomeVR BaseGameMode|Logging")
	FString AbsoluteFilePath = FString("C:/DomeVR_LOG/"); // Set standard value for log path

	UPROPERTY(Blueprintable, BlueprintReadOnly, EditAnywhere, Category = "DomeVR BaseGameMode|Logging")
	int32 BufferSize = 10000; // Set standard value for number of log elements that are buffered

	UPROPERTY(Blueprintable, BlueprintReadWrite, EditAnywhere, Category = "DomeVR BaseGameMode|Logging")
	FSessionData SessionData;

	UPROPERTY(Blueprintable, BlueprintReadWrite, EditAnywhere, Category = "DomeVR BaseGameMode|Logging")
	bool Debug = 0; // Toggle debugging of code on and off

	FD3D11Viewport * ViewportD3D11;

public:

	ADomeVRBaseGameMode();
	/**
	* Methods for retrieving different log data types
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR BaseGameMode|Logging")
	void SendLog(FLogData SentLog);

	UFUNCTION(BlueprintCallable, Category = "DomeVR BaseGameMode|Logging")
	void SendTextLog(FLogDataText SentLog);

	UFUNCTION(BlueprintCallable, Category = "DomeVR BaseGameMode|Logging")
	void SendIntLog(FLogDataInt SentLog);

	UFUNCTION(BlueprintCallable, Category = "DomeVR BaseGameMode|Logging")
	void SendIntArrayLog(FLogDataIntArray SentLog);

	UFUNCTION(BlueprintCallable, Category = "DomeVR BaseGameMode|Logging")
	void SendEventmarkerLog(FLogDataEventmarker SentLog);

	UFUNCTION(BlueprintCallable, Category = "DomeVR BaseGameMode|Logging")
	void SendFloatLog(FLogDataFloat SentLog);

	UFUNCTION(BlueprintCallable, Category = "DomeVR BaseGameMode|Logging")
	void SendFloatArrayLog(FLogDataFloatArray SentLog);

	UFUNCTION(BlueprintCallable, Category = "DomeVR BaseGameMode|Logging")
	void SendVectorLog(FLogDataVector SentLog);

	UFUNCTION(BlueprintCallable, Category = "DomeVR BaseGameMode|Logging")
	void SendVectorArrayLog(FLogDataVectorArray SentLog);

	UFUNCTION(BlueprintCallable, Category = "DomeVR BaseGameMode|Logging")
	void SendGUIDLog(FLogDataGUID SentLog);

	UFUNCTION(BlueprintCallable, Category = "DomeVR BaseGameMode|Logging")
	void SendUObjectIDsLog(FLogDataUE4ObjectArray SentLog);

	UFUNCTION(BlueprintCallable, Category = "DomeVR BaseGameMode|Logging")
	void WriteSessionDataHeader();

	UFUNCTION(BlueprintCallable, Category = "DomeVR BaseGameMode|Logging")
	void UpdateSaveStringWithSessionData();

	UFUNCTION(BlueprintCallable, Category = "DomeVR BaseGameMode|Game Logic")
	void LoadLevel(FString LevelName, FString GameModePath);

	virtual void Tick(float DeltaSeconds) override;

	virtual void InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage) override;

	virtual void StartPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:

	UFUNCTION(BlueprintCallable, Category = "DomeVR BaseGameMode|Logging")
	void WriteLog();
	UFUNCTION(BlueprintCallable, Category = "DomeVR BaseGameMode|Logging")
	void WriteLogContinuous();


private:

	virtual void BeginDestroy() override; 

};
