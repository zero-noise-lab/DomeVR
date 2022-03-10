// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/GameFramework/GameUserSettings.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
//#include "VisualStimulusSettings__pf802250765.h"
//#include "EDomeVRDisplayMode__pf2132744816.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CDomeVRGameInstance.generated.h"

/**
 *
 */
UCLASS(config=Game, Blueprintable, BlueprintType)
class UCDomeVRGameInstance : public UGameInstance
{
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVRInstance")
	TMap<int32, FString> EventmarkerDictionary;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVRInstance")
	//EDomeVRDisplayMode UIMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVRInstance")
	float TimeRunning;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVRInstance")
	float ActCameraChangeValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVRInstance")
	FIntPoint ActResolution;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVRInstance")
	//TArray<FVisualSimulusSettings> VisualStimuli;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVRInstance")
	int32 OIDLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVRInstance")
	int32 CaptureResolution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVRInstance")
	int32 FisheyeResolution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVRInstance")
	TArray<int32> SelectableFramerates;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVRInstance")
	TArray<FIntPoint> SelectableResolutions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVRInstance")
	FIntPoint DefaultResolution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVRInstance")
	FVector2D DefaultSecondWindowResolution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVRInstance")
	bool bDefaultVSync;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVRInstance")
	TEnumAsByte<EWindowMode::Type> DefaultWindowmode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVRInstance")
	int32 DefaultMaxFramerate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVRInstance")
	bool bSwitchSplitscreens;
	
	int32 VerbosityLevel;

	UCDomeVRGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
		
	//virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;
	//static void __CustomDynamicClassInitialization(UDynamicClass* InDynamicClass);





};

