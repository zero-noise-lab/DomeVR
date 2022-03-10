// Fill out your copyright notice in the Description page of Project Settings.


#include "TrialPerformanceCounter.h"

UTrialPerformanceCounter::UTrialPerformanceCounter()
{
}

void UTrialPerformanceCounter::Init(int FirstTrial, int FirstPerformanceCount)
{
	PerformanceCount.Add(FirstPerformanceCount);
	Trial.Add(FirstTrial);
	IsInitialized = true;
}

void UTrialPerformanceCounter::IncrementPerformanceCount(int PerformanceIncrement)
{
	if (IsInitialized)
	{
		PerformanceCount.Add((PerformanceCount.Top() + PerformanceIncrement));
		Trial.Add((Trial.Top() + 1));
	}
}

void UTrialPerformanceCounter::KeepPerformanceCount()
{

	if (IsInitialized)
	{
		PerformanceCount.Add((PerformanceCount.Top() + 0));
		Trial.Add((Trial.Top() + 1));
	}
}

int UTrialPerformanceCounter::GetPerformanceCountOfTrial(int ITrial)
{

	if (Trial.Find(ITrial))
	{
		int ITrialIndex = Trial.Find(ITrial);
		return PerformanceCount[ITrialIndex];
	}
	else
	{
		return 0;
	}

}

int UTrialPerformanceCounter::GetNPriorPerformanceCount(int NPrior)
{
	int CalcIndex = (PerformanceCount.Num() - (NPrior + 1));
	if (CalcIndex >= 0)
	{
		return PerformanceCount[CalcIndex];
	}
	else
	{
		return 0;
	}
		
}

int UTrialPerformanceCounter::GetLastPerformanceCount()
{
	if (PerformanceCount.Num() > 0)
	{
		return PerformanceCount[PerformanceCount.Num() - 1];
	}
	else
	{
		return 0;
	}
	
}

int UTrialPerformanceCounter::GetNumElements()
{
	return PerformanceCount.Num();
}

//UTrialPerformanceCounter::~TrialPerformanceCounter()
//{
//}
