// Fill out your copyright notice in the Description page of Project Settings.


#include "SavedTrialPerformanceCounter.h"

USavedTrialPerformanceCounter::USavedTrialPerformanceCounter()
{
}

void USavedTrialPerformanceCounter::Init(TMap<FName, UTrialPerformanceCounter*> TrialPerformanceCounterToSave)
{
	TrialPerformanceCounter = TrialPerformanceCounterToSave;
}

TMap<FName, UTrialPerformanceCounter*> USavedTrialPerformanceCounter::GetSavedTrialPerformanceCounter()
{
	return this->TrialPerformanceCounter;
}
