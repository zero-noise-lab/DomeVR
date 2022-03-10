// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrialPerformanceCounter.generated.h"

/**
 * Class for saving the count of some performance category in each trial
 */
UCLASS(Blueprintable)
class DOMEVR_API UTrialPerformanceCounter : public UObject
{
public:
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "DomeVR Data Types")
	bool IsInitialized;

	// Array to save the summed performance count
	UPROPERTY(BlueprintReadOnly)
	TArray<int> PerformanceCount;
	// Array to save the trial number
	UPROPERTY(BlueprintReadOnly)
	TArray<int> Trial;

	UTrialPerformanceCounter();

	//~UTrialPerformanceCounter();
	
	/**
	* Function for initializing instance with a first trial number FirstTrial
	* and an initial performance count of FirstPerformanceCount
	* @param FirstTrial - Number of first counted trial 
	* @param FirstPerformanceCount - Count of first counted trial
	*/
	void Init(int FirstTrial, int FirstPerformanceCount);

	/**
	* Function that increments the performance count in this trial
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Data Types")
	void IncrementPerformanceCount(int PerformanceIncrement);

	/**
	* Function that keeps the performance count in this trial
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Data Types")
	void KeepPerformanceCount();

	/**
	* Function that returns the performance count of trial ITrial
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Data Types")
	int GetPerformanceCountOfTrial(int ITrial);

	/**
	* Function that returns the performance count NPrior trials before
	* the current trial
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Data Types")
	int GetNPriorPerformanceCount(int NPrior);

	/**
	* Function that returns the last performance count
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Data Types")
	int GetLastPerformanceCount();

	/**
	* Function that returns the number of performance counts
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Data Types")
	int GetNumElements();
};
