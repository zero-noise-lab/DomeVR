// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMInstance.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/NoExportTypes.h"
#include "StateMachineVariableEditor.generated.h"

/**
 * StateMachineVariableEditor object that enables to read and write variables to a referenced
 * state machine
 */
UCLASS(Blueprintable, BlueprintType)
class DOMEVR_API UStateMachineVariableEditor : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
	
public:

	// State machine to read variables from and write variables to
	UPROPERTY(BlueprintReadOnly, Category = "DomeVR SM Variable Editor")
	USMInstance* StateMachine;


	// Dictionaries holding current variable values for different data
	// types
	UPROPERTY(BlueprintReadOnly, Category = "DomeVR SM Variable Editor")
	TMap<FName, float> FloatVariables;

	UPROPERTY(BlueprintReadOnly, Category = "DomeVR SM Variable Editor")
	TMap<FName, int> IntVariables;

	UPROPERTY(BlueprintReadOnly, Category = "DomeVR SM Variable Editor")
	TMap<FName, bool> BoolVariables;

	UPROPERTY(BlueprintReadOnly, Category = "DomeVR SM Variable Editor")
	TMap<FName, FVector> VectorVariables;

	UPROPERTY(BlueprintReadOnly, Category = "DomeVR SM Variable Editor")
	TMap<FName, FString> StringVariables;


	// RefreshRate sets how often the variable values in the dictionaries
	// should be refreshed
	UPROPERTY(BlueprintReadOnly, Category = "DomeVR SM Variable Editor")
	float RefreshRate = 1.0f;

	// Timer handle for doing the refreshes
	UPROPERTY()
	FTimerHandle RefreshHandle;

	// Sets if variables values in dictionaries should be refreshed at all
	UPROPERTY(BlueprintReadOnly, Category = "DomeVR SM Variable Editor")
	bool bIsRefreshing;

	// Variable prefix specifying which variables can be read and written
	UPROPERTY(BlueprintReadOnly, Category = "DomeVR SM Variable Editor")
	FString VariablePrefix = "EDIT_";

	UStateMachineVariableEditor();
	//~StateMachineVariableEditor();

	void init(USMInstance* NewStateMachine, FString NewVariablePrefix, float NewRefreshRate);


	UFUNCTION(BlueprintCallable, Category = "DomeVR SM Variable Editor")
	void SetStateMachine(USMInstance* NewStateMachine);

	UFUNCTION(BlueprintCallable, Category = "DomeVR SM Variable Editor")
	void SetVariablePrefix(FString NewVariablePrefix);

	UFUNCTION(BlueprintCallable, Category = "DomeVR SM Variable Editor")
	void SetRefreshRate(float NewRefreshRate);

	UFUNCTION(BlueprintCallable, Category = "DomeVR SM Variable Editor")
	void SetFloatVariable(FName VariableName, float FloatValue);

	UFUNCTION(BlueprintCallable, Category = "DomeVR SM Variable Editor")
	void SetIntVariable(FName VariableName, int IntValue);

	UFUNCTION(BlueprintCallable, Category = "DomeVR SM Variable Editor")
	void SetBoolVariable(FName VariableName, bool BoolValue);

	UFUNCTION(BlueprintCallable, Category = "DomeVR SM Variable Editor")
	void SetVectorVariable(FName VariableName, FVector VectorValue);

	UFUNCTION(BlueprintCallable, Category = "DomeVR SM Variable Editor")
	void SetStringVariable(FName VariableName, FString StringValue);

	UFUNCTION(BlueprintCallable, Category = "DomeVR SM Variable Editor")
	void RefreshValues(bool bDoRefreshes);

	void InitializeVariableValues();

	UFUNCTION(BlueprintCallable, Category = "DomeVR SM Variable Editor")
	void UpdateVariableValues();

	UFUNCTION(BlueprintCallable, Category = "DomeVR SM Variable Editor")
	bool IsStateMachineActive();

	UFUNCTION(BlueprintCallable, Category = "DomeVR SM Variable Editor")
	void PrintVariables();

	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;

	UWorld* GetWorld() const override;

	float TestCounter;
};
