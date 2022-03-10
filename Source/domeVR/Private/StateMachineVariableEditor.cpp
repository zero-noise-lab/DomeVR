// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachineVariableEditor.h"

UStateMachineVariableEditor::UStateMachineVariableEditor()
{

}
void UStateMachineVariableEditor::init(USMInstance* NewStateMachine, FString NewVariablePrefix, float NewRefreshRate)
{
	VariablePrefix = NewVariablePrefix;

	this->SetStateMachine(NewStateMachine);

	if (NewRefreshRate < 0)
	{
		bIsRefreshing = false;
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(RefreshHandle, this, &UStateMachineVariableEditor::UpdateVariableValues, NewRefreshRate, true);
		bIsRefreshing = true;
		RefreshRate = NewRefreshRate;
	}
}

void UStateMachineVariableEditor::SetStateMachine(USMInstance* NewStateMachine)
{
	if (IsValid(StateMachine))
	{
		return;
	}
	else if (IsValid(NewStateMachine))
	{
		StateMachine = NewStateMachine;
		InitializeVariableValues();
		return;
	}
	else
	{
		return;
	}

}

void UStateMachineVariableEditor::SetVariablePrefix(FString NewVariablePrefix)
{
	if (bIsRefreshing)
	{
		RefreshValues(false);
		VariablePrefix = NewVariablePrefix;
		InitializeVariableValues();
		RefreshValues(true);
	}
	else
	{
		VariablePrefix = NewVariablePrefix;
		InitializeVariableValues();
	}
	
}

void UStateMachineVariableEditor::SetRefreshRate(float NewRefreshRate)
{
	RefreshRate = FMath::Clamp(NewRefreshRate, 0.0f, 60.0f);
	if (RefreshRate > 0 && bIsRefreshing)
	{
		GetWorld()->GetTimerManager().ClearTimer(RefreshHandle);
		GetWorld()->GetTimerManager().SetTimer(RefreshHandle, this, &UStateMachineVariableEditor::UpdateVariableValues, RefreshRate, true);
		bIsRefreshing = true;
	}
	else if (RefreshRate > 0 && !bIsRefreshing)
	{
		GetWorld()->GetTimerManager().SetTimer(RefreshHandle, this, &UStateMachineVariableEditor::UpdateVariableValues, RefreshRate, true);
		bIsRefreshing = true;
	}
	else if (RefreshRate <= 0 && bIsRefreshing)
	{
		GetWorld()->GetTimerManager().ClearTimer(RefreshHandle);
		bIsRefreshing = false;
	}
	else
	{
		bIsRefreshing = false;
	}
}

void UStateMachineVariableEditor::SetFloatVariable(FName VariableName, float FloatValue)
{
	UFloatProperty* FloatProperty = FindFieldChecked<UFloatProperty>(StateMachine->GetClass(), VariableName);

	if (FloatProperty)
	{
		FloatProperty->SetPropertyValue_InContainer(StateMachine, FloatValue);
	}
}

void UStateMachineVariableEditor::SetIntVariable(FName VariableName, int IntValue)
{
	UIntProperty* IntProperty = FindFieldChecked<UIntProperty>(StateMachine->GetClass(), VariableName);

	if (IntProperty)
	{
		IntProperty->SetPropertyValue_InContainer(StateMachine, IntValue);
	}
}

void UStateMachineVariableEditor::SetBoolVariable(FName VariableName, bool BoolValue)
{
	UBoolProperty* BoolProperty = FindFieldChecked<UBoolProperty>(StateMachine->GetClass(), VariableName);

	if (BoolProperty)
	{
		BoolProperty->SetPropertyValue_InContainer(StateMachine, BoolValue);
	}
}

void UStateMachineVariableEditor::SetVectorVariable(FName VariableName, FVector VectorValue)
{

	UProperty* VectorProperty = FindFieldChecked<UProperty>(StateMachine->GetClass(), VariableName);

	if (VectorProperty)
	{
		void * VectorAddress = VectorProperty->ContainerPtrToValuePtr<void>(StateMachine);

		if (UStructProperty * StructProperty = Cast<UStructProperty>(VectorProperty))
		{
			FVector *v = StructProperty->ContainerPtrToValuePtr<FVector>(StateMachine);
			v->X = VectorValue.X;
			v->Y = VectorValue.Y;
			v->Z = VectorValue.Z;
		}
	}
}

void UStateMachineVariableEditor::SetStringVariable(FName VariableName, FString StringValue)
{
	UProperty* StringProperty = FindFieldChecked<UProperty>(StateMachine->GetClass(), VariableName);

	if (StringProperty)
	{
		FString * StringAddress = StringProperty->ContainerPtrToValuePtr<FString>(StateMachine);
		*StringAddress = StringValue;
	}
}

void UStateMachineVariableEditor::RefreshValues(bool bDoRefreshes)
{

	if (bDoRefreshes && !bIsRefreshing)
	{
		GetWorld()->GetTimerManager().SetTimer(RefreshHandle, this, &UStateMachineVariableEditor::UpdateVariableValues, RefreshRate, true);
		bIsRefreshing = bDoRefreshes;
	}
	else if (!bDoRefreshes && bIsRefreshing)
	{
		GetWorld()->GetTimerManager().ClearTimer(RefreshHandle);
		bIsRefreshing = bDoRefreshes;
	}
	else
	{
		bIsRefreshing = bDoRefreshes;
	}
}

void UStateMachineVariableEditor::InitializeVariableValues()
{
	if (!IsValid(StateMachine))
	{
		return;
	}

	FloatVariables.Empty();
	IntVariables.Empty();
	BoolVariables.Empty();
	VectorVariables.Empty();
	StringVariables.Empty();

	for (TFieldIterator<UProperty> PropIt(StateMachine->GetClass()); PropIt; ++PropIt)
	{
		FName VariableName = PropIt->GetFName();
		FString CheckVariableName = VariableName.ToString();
		if (!CheckVariableName.StartsWith(VariablePrefix))
		{
			continue;
		}

		if (PropIt->GetCPPType().Equals(TEXT("float")))
		{
			UFloatProperty* FloatProperty = FindFieldChecked<UFloatProperty>(StateMachine->GetClass(), VariableName);
			if (FloatProperty)
			{
				float FloatValue = FloatProperty->GetPropertyValue_InContainer(StateMachine);
				FloatVariables.Add(VariableName, FloatValue);
			}

		}

		else if (PropIt->GetCPPType().Equals(TEXT("int32")))
		{
			UIntProperty* IntProperty = FindFieldChecked<UIntProperty>(StateMachine->GetClass(), VariableName);
			if (IntProperty)
			{
				int IntValue = IntProperty->GetPropertyValue_InContainer(StateMachine);
				IntVariables.Add(VariableName, IntValue);

			}
		}

		else if (PropIt->GetCPPType().Equals(TEXT("FVector")))
		{

			UProperty* VectorProperty = FindFieldChecked<UProperty>(StateMachine->GetClass(), VariableName);
			if (VectorProperty)
			{
				void * VectorAddress = VectorProperty->ContainerPtrToValuePtr<void>(StateMachine);

				if (UStructProperty * StructProperty = Cast<UStructProperty>(VectorProperty))
				{	
					// Easier method for some reason not working here
					//FVector *v = StructProperty->ContainerPtrToValuePtr<FVector>(StateMachine);
					//float X = v->X;
					//float Y = v->Y;
					//float Z = v->Z;
					//FVector ActVector = FVector(X, Y, Z);
					//VectorVariables.Add(VariableName, ActVector);
					UScriptStruct* ScriptStruct = StructProperty->Struct;

					UProperty* XValue = ScriptStruct->FindPropertyByName(FName("X"));
					UProperty* YValue = ScriptStruct->FindPropertyByName(FName("Y"));
					UProperty* ZValue = ScriptStruct->FindPropertyByName(FName("Z"));
					UFloatProperty * ChildFloatX = Cast<UFloatProperty>(XValue);
					UFloatProperty * ChildFloatY = Cast<UFloatProperty>(YValue);
					UFloatProperty * ChildFloatZ = Cast<UFloatProperty>(ZValue);

					if (ChildFloatX && ChildFloatY && ChildFloatZ)
					{
						float X = ChildFloatX->GetFloatingPointPropertyValue(VectorAddress);
						float Y = ChildFloatY->GetFloatingPointPropertyValue(VectorAddress);
						float Z = ChildFloatZ->GetFloatingPointPropertyValue(VectorAddress);

						FVector ActVector = FVector(X, Y, Z);
						VectorVariables.Add(VariableName, ActVector);
					}
				}

			}
		}

		else if (PropIt->GetCPPType().Equals(TEXT("bool")))
		{
			UBoolProperty* BoolProperty = FindFieldChecked<UBoolProperty>(StateMachine->GetClass(), VariableName);
			if (BoolProperty)
			{
				bool BoolValue = BoolProperty->GetPropertyValue_InContainer(StateMachine);
				BoolVariables.Add(VariableName, BoolValue);

			}
		}

		else if (PropIt->GetCPPType().Equals(TEXT("FString")))
		{
			UProperty* StringProperty = FindFieldChecked<UProperty>(StateMachine->GetClass(), VariableName);
			if (StringProperty)
			{
				FString * StringAddress = StringProperty->ContainerPtrToValuePtr<FString>(StateMachine);
				FString StringValue = FString(*StringAddress);
				StringVariables.Add(VariableName, StringValue);
			}
		}

		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("HERE COMES THE TYPE"));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, PropIt->GetCPPType());
		}

	}
}

void UStateMachineVariableEditor::UpdateVariableValues()
{
	if (!IsStateMachineActive())
	{
		return;
	}

	for (auto It = FloatVariables.CreateConstIterator(); It; ++It)
	{
		FName VariableName = It->Key;
		UFloatProperty* FloatProperty = FindFieldChecked<UFloatProperty>(StateMachine->GetClass(), VariableName);

		if (FloatProperty)
		{
			float FloatValue = FloatProperty->GetPropertyValue_InContainer(StateMachine);
			FloatVariables.Add(VariableName, FloatValue);
		}

	}

	for (auto It = IntVariables.CreateConstIterator(); It; ++It)
	{
		FName VariableName = It->Key;
		UIntProperty* IntProperty = FindFieldChecked<UIntProperty>(StateMachine->GetClass(), VariableName);

		if (IntProperty)
		{
			int IntValue = IntProperty->GetPropertyValue_InContainer(StateMachine);
			IntVariables.Add(VariableName, IntValue);
		}
	}

	for (auto It = BoolVariables.CreateConstIterator(); It; ++It)
	{
		FName VariableName = It->Key;
		UBoolProperty* BoolProperty = FindFieldChecked<UBoolProperty>(StateMachine->GetClass(), VariableName);
		if (BoolProperty)
		{
			bool BoolValue = BoolProperty->GetPropertyValue_InContainer(StateMachine);
			BoolVariables.Add(VariableName, BoolValue);
		}
	}

	for (auto It = VectorVariables.CreateConstIterator(); It; ++It)
	{
		FName VariableName = It->Key;
		UProperty* VectorProperty = FindFieldChecked<UProperty>(StateMachine->GetClass(), VariableName);
		if (VectorProperty)
		{
			void * VectorAddress = VectorProperty->ContainerPtrToValuePtr<void>(StateMachine);

			if (UStructProperty * StructProperty = Cast<UStructProperty>(VectorProperty))
			{
				FVector *v = StructProperty->ContainerPtrToValuePtr<FVector>(StateMachine);
				float X = v->X;
				float Y = v->Y;
				float Z = v->Z;
				FVector ActVector = FVector(X, Y, Z);
				VectorVariables.Add(VariableName, ActVector);
			}
		}
	}

	for (auto It = StringVariables.CreateConstIterator(); It; ++It)
	{
		FName VariableName = It->Key;
		UProperty* StringProperty = FindFieldChecked<UProperty>(StateMachine->GetClass(), VariableName);
		if (StringProperty)
		{
			FString * StringAddress = StringProperty->ContainerPtrToValuePtr<FString>(StateMachine);
			FString StringValue = FString(*StringAddress);
			StringVariables.Add(VariableName, StringValue);
		}
	}
}

bool UStateMachineVariableEditor::IsStateMachineActive()
{
	if (IsValid(StateMachine))
	{
		return StateMachine->IsActive();
	}
	else
	{
		return false;
	}
}

void UStateMachineVariableEditor::PrintVariables()
{
	for (auto& Element : FloatVariables)
	{
		FName VariableName = Element.Key;		
		float Value = Element.Value;		
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, FString::SanitizeFloat(Value));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, VariableName.ToString());
	}

	for (auto& Element : IntVariables)
	{
		FName VariableName = Element.Key;
		int Value = Element.Value;
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, FString::FromInt(Value));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, VariableName.ToString());
	}

	for (auto& Element : IntVariables)
	{
		FName VariableName = Element.Key;
		bool Value = Element.Value;
		FString ValueString;
		if (Value)
		{
			ValueString = "True";
		}
		else
		{
			ValueString = "False";
		}
			
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, ValueString);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, VariableName.ToString());
	}

	for (auto& Element : VectorVariables)
	{
		FName VariableName = Element.Key;
		FVector Value = Element.Value;
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, FString::Printf(TEXT("X: %f, Y: %f, Z: %f"), Value.X, Value.Y, Value.Z));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, VariableName.ToString());
	}

	for (auto& Element : StringVariables)
	{
		FName VariableName = Element.Key;
		FString Value = Element.Value;
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, Value);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, VariableName.ToString());
	}
}

void UStateMachineVariableEditor::Tick(float DeltaTime)
{

}

bool UStateMachineVariableEditor::IsTickable() const
{
	return true;
}

bool UStateMachineVariableEditor::IsTickableInEditor() const
{
	return false;
}

bool UStateMachineVariableEditor::IsTickableWhenPaused() const
{
	return true;
}

TStatId UStateMachineVariableEditor::GetStatId() const
{
	return TStatId();
}

UWorld* UStateMachineVariableEditor::GetWorld() const
{
	return GetOuter()->GetWorld();
}
