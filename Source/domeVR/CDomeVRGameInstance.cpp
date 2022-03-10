// Fill out your copyright notice in the Description page of Project Settings.


#include "CDomeVRGameInstance.h"

UCDomeVRGameInstance::UCDomeVRGameInstance(const FObjectInitializer& ObjectInitializer) : Super()
{

	//Eventmarkers
	EventmarkerDictionary = {};
	EventmarkerDictionary.Reserve(17);

	// Responses
	EventmarkerDictionary.Add(1, FString(TEXT("ResponseCorrect")));
	EventmarkerDictionary.Add(2, FString(TEXT("ResponseWrong")));

	//Errors during Approach
	EventmarkerDictionary.Add(10, FString(TEXT("ErrorApproachWobble")));
	EventmarkerDictionary.Add(11, FString(TEXT("ErrorApproachTimeout")));
	EventmarkerDictionary.Add(12, FString(TEXT("ErrorApproachStationary")));

	//Errors during Task
	EventmarkerDictionary.Add(20, FString(TEXT("ErrorTaskWobble")));
	EventmarkerDictionary.Add(21, FString(TEXT("ErrorTaskTimeout")));
	EventmarkerDictionary.Add(22, FString(TEXT("ErrorTaskStationary")));

	//Trial numbers
	EventmarkerDictionary.Add(1000, FString(TEXT("BaseTrialNumber")));

	//Trial events
	EventmarkerDictionary.Add(3001, FString(TEXT("SpawnedPlaceholderStimulus")));
	EventmarkerDictionary.Add(3002, FString(TEXT("OverlappedPlaceholder")));
	EventmarkerDictionary.Add(3003, FString(TEXT("SpawnedTargetStimuli")));
	EventmarkerDictionary.Add(3004, FString(TEXT("OverlappedTarget")));
	EventmarkerDictionary.Add(3005, FString(TEXT("PunishmentStart")));
	EventmarkerDictionary.Add(3006, FString(TEXT("PunishmentStop")));

	//Trial end
	EventmarkerDictionary.Add(3099, FString(TEXT("TrialEnd")));

	//Stimulus features
	EventmarkerDictionary.Add(3100, FString(TEXT("BaseStimulusFeature")));

	// Other variables

	//UIMode = EDomeVRDisplayMode::NewEnumerator1;

	TimeRunning = 0.0f;
	ActCameraChangeValue = 10.0f;

	ActResolution.X = 1920;
	ActResolution.Y = 1200;

	//VisualStimuli = TArray<FVisualSimulusSettings>();

	OIDLength = 12;
	CaptureResolution = 1024;
	FisheyeResolution = 2048;

	SelectableFramerates = TArray<int32>();
	SelectableFramerates.Reserve(4);
	SelectableFramerates.Add(30);
	SelectableFramerates.Add(60);
	SelectableFramerates.Add(120);
	SelectableFramerates.Add(240);

	SelectableResolutions = TArray<FIntPoint>();
	SelectableResolutions.Reserve(4);
	SelectableResolutions.Add(FIntPoint(EForceInit::ForceInit));
	SelectableResolutions[0].X = 1280;
	SelectableResolutions[0].Y = 720;
	SelectableResolutions.Add(FIntPoint(EForceInit::ForceInit));
	SelectableResolutions[1].X = 1920;
	SelectableResolutions[1].Y = 1080;
	SelectableResolutions.Add(FIntPoint(EForceInit::ForceInit));
	SelectableResolutions[2].X = 1920;
	SelectableResolutions[2].Y = 1200;
	SelectableResolutions.Add(FIntPoint(EForceInit::ForceInit));
	SelectableResolutions[3].X = 3860;
	SelectableResolutions[3].Y = 2160;

	DefaultResolution.X = 1920;
	DefaultResolution.Y = 1200;

	DefaultSecondWindowResolution = FVector2D(1920.0, 1200.0);
	bDefaultVSync = true;
	DefaultWindowmode = EWindowMode::Type::Windowed;
	DefaultMaxFramerate = 60;
	bSwitchSplitscreens = true;

	VerbosityLevel = 2;

};