// Fill out your copyright notice in the Description page of Project Settings.


#include "TrialPerformanceChart.h"

UTrialPerformanceChart::UTrialPerformanceChart()
{
}

UTrialPerformanceChart::~UTrialPerformanceChart()
{
}

void UTrialPerformanceChart::AddDataPoint(float Trial, float Performance)
{
	FKantanCartesianDatapoint Act_Datapoint = FKantanCartesianDatapoint();
	Act_Datapoint.Coords.X = Trial;
	Act_Datapoint.Coords.Y = Performance;
	ChartDatapoints.Add(Act_Datapoint);
}

FKantanCartesianDatapoint UTrialPerformanceChart::GetDataPoint(int Trial)
{
	return ChartDatapoints[Trial - 1];

}

TArray<FKantanCartesianDatapoint> UTrialPerformanceChart::GetDataPoints()
{
	return ChartDatapoints;
}

FKantanCartesianDatapoint UTrialPerformanceChart::GetLastDatapoint()
{
	return ChartDatapoints.Last();
}
