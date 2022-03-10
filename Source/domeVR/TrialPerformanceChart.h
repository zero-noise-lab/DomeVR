// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KantanCartesianDatapoint.h"
#include "TrialPerformanceChart.generated.h"
/**
 * Class for saving the values of the performance chart in each trial
 */
UCLASS(Blueprintable)
class DOMEVR_API UTrialPerformanceChart : public UObject
{
public:

	GENERATED_BODY()

	// Array that holds the data points of the performance chart
	UPROPERTY()
	TArray<FKantanCartesianDatapoint> ChartDatapoints;

	UTrialPerformanceChart();
	~UTrialPerformanceChart();
	/**
	* Function for adding a data point (Trial, Performance)
	* @param Trial - Number of trial
	* @param Performance - Performance value in Trial
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Data Types")
	void AddDataPoint(float Trial, float Performance);

	/**
	* Function to get data point (Trial, Performance) of Trial
	* @param Trial - Trial number
	* @return data point (Trial, Performance)
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Data Types")
	FKantanCartesianDatapoint GetDataPoint(int Trial);

	/**
	* Function to get all saved data points 
	* @return Array of data points (Trial, Performance)
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Data Types")
	TArray<FKantanCartesianDatapoint> GetDataPoints();

	/**
	* Function to get last data point (Trial, Performance)
	* @return data point (Trial, Performance)
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Data Types")
	FKantanCartesianDatapoint GetLastDatapoint();
};
