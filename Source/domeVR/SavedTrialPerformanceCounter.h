// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrialPerformanceCounter.h"
#include "SavedTrialPerformanceCounter.generated.h"
/**
 * Wrapper class for saving TrialPerformanceCounter
 */
UCLASS(Blueprintable)
class DOMEVR_API USavedTrialPerformanceCounter : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY()
	TMap<FName, UTrialPerformanceCounter*> TrialPerformanceCounter;

public:


	USavedTrialPerformanceCounter();

	/**
	* Function to initialize class that saves a TrialPerformanceCounter
	* @param TrialPerformanceCounterToSave - Counter that should saved
	*/
	void Init(TMap<FName, UTrialPerformanceCounter*> TrialPerformanceCounterToSave);
	
	/**
	* Function to get the saved TrialPerformanceCounter
	* @return saved TrialPerformanceCounter
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Data Types")
	TMap<FName, UTrialPerformanceCounter*> GetSavedTrialPerformanceCounter();
};
