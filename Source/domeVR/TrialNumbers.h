// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrialNumbers.generated.h"
/**
 * 
 */
UCLASS(Blueprintable)
class DOMEVR_API UTrialNumbers : public UObject
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
	TArray<int> TrialNumbers;

	UTrialNumbers();	
	
	UFUNCTION(BlueprintCallable, Category = "DomeVR Data Types")
	void AddTrialNumber(int TrialNumber);

};
