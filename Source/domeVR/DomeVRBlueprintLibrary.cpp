// Fill out your copyright notice in the Description page of Project Settings.

#include "DomeVRBlueprintLibrary.h"

FString UDomeVRBlueprintLibrary::RandomObjectID(FRandomStream RandStream, int IDLength)
{
	std::string ActString;

	static const char AlphaNum[] =
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < IDLength; ++i) {

		int32 RandIndex = RandStream.RandRange(0, 61);
		char ActChar = AlphaNum[RandIndex];
		ActString += ActChar;


	}
	FString ObjectID(ActString.c_str());
	return ObjectID;
}

FString UDomeVRBlueprintLibrary::GetUE4ObjectID(UObject *ActObject)
{
	uint32 ObjID = ActObject->GetUniqueID();
	std::string IDString = std::to_string(ObjID);
	FString UE4IDString = FString(IDString.c_str());
	return UE4IDString;
}

TArray<int> UDomeVRBlueprintLibrary::BoolToIntArray(TArray<bool> BoolArray)
{
	int ArrayLength = BoolArray.Num();
	TArray<int> IntArray;
	for (int i = 0; i < ArrayLength; i++)
	{
		IntArray.Add(BoolArray[i]);
	}

	return IntArray;
}

FString UDomeVRBlueprintLibrary::GetCurrentTimeAsString()
{
	FDateTime ActTime = FDateTime().Now();
	FString TimeString = FString::Printf(TEXT("%02d:%02d:%02d"), ActTime.GetHour(), ActTime.GetMinute(), ActTime.GetSecond());
	return TimeString;
}


FString UDomeVRBlueprintLibrary::GetCurrentDateAsString()
{
	FDateTime ActTime = FDateTime().Now();
	FString TimeString = FString::Printf(TEXT("%d%02d%02d"), ActTime.GetYear(), ActTime.GetMonth(), ActTime.GetDay());
	return TimeString;
}

TArray<FString> UDomeVRBlueprintLibrary::GetAllMapNames() {
	auto ObjectLibrary = UObjectLibrary::CreateLibrary(UWorld::StaticClass(), false, true);
	ObjectLibrary->LoadAssetDataFromPath(TEXT("/Game/Maps"));
	TArray<FAssetData> AssetDatas;
	ObjectLibrary->GetAssetDataList(AssetDatas);

	UE_LOG(LogTemp, Warning, TEXT("Found maps in /Game/Maps: %d"), AssetDatas.Num());

	TArray<FString> Names = TArray<FString>();

	for (int32 i = 0; i < AssetDatas.Num(); ++i)
	{
		FAssetData& AssetData = AssetDatas[i];
		FString PathString = AssetData.GetFullName();
		
		if (PathString.Contains(TEXT("Layer")))
		{
			continue;
		}

		auto name = AssetData.AssetName.ToString();
		Names.Add(name);
	}

	ObjectLibrary->LoadAssetDataFromPath(TEXT("/Game/Blueprints/StateMachines/Users"));
	ObjectLibrary->GetAssetDataList(AssetDatas);

	for (int32 i = 0; i < AssetDatas.Num(); ++i)
	{
		FAssetData& AssetData = AssetDatas[i];
		FString PathString = AssetData.GetFullName();

		if (PathString.Contains(TEXT("Layer")))
		{
			continue;
		}

		auto name = AssetData.AssetName.ToString();
		Names.Add(name);
	}

	return Names;
}

TArray<FString> UDomeVRBlueprintLibrary::GetAllMapNamesInUserFolder(FString UserFolder)
{
	if (UserFolder.IsEmpty())
	{
		return GetAllMapNames();
	}

	FString PathName = TEXT("/Game/Blueprints/StateMachines/Users/");
	PathName.Append(UserFolder);
	auto ObjectLibrary = UObjectLibrary::CreateLibrary(UWorld::StaticClass(), false, true);
	ObjectLibrary->LoadAssetDataFromPath(PathName);
	TArray<FAssetData> AssetDatas;
	ObjectLibrary->GetAssetDataList(AssetDatas);

	UE_LOG(LogTemp, Warning, TEXT("Found maps in /Game/Maps: %d"), AssetDatas.Num());

	TArray<FString> Names = TArray<FString>();

	for (int32 i = 0; i < AssetDatas.Num(); ++i)
	{
		FAssetData& AssetData = AssetDatas[i];
		FString PathString = AssetData.GetFullName();

		if (PathString.Contains(TEXT("Layer")))
		{
			continue;
		}

		auto name = AssetData.AssetName.ToString();
		Names.Add(name);
	}

	return Names;
}

TMap<FString, TSoftClassPtr<USMInstance>> UDomeVRBlueprintLibrary::GetAllStateMachineClasses(FString UserFolder) {

	UClass* Base = USMInstance::StaticClass();
	FString Path = TEXT("/Game/Blueprints/StateMachines");

	if (!UserFolder.IsEmpty())
	{
		Path = Path + "/Users/" + UserFolder;
	}
	TMap<FString, TSoftClassPtr<USMInstance>> StateMachineDictionary;
	TArray<FString> StateMachineNames;
	// Load the asset registry module
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked< FAssetRegistryModule >(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	// The asset registry is populated asynchronously at startup, so there's no guarantee it has finished.
	// This simple approach just runs a synchronous scan on the entire content directory.
	// Better solutions would be to specify only the path to where the relevant blueprints are,
	// or to register a callback with the asset registry to be notified of when it's finished populating.
	TArray< FString > ContentPaths;
	ContentPaths.Add(Path);
	AssetRegistry.ScanPathsSynchronous(ContentPaths);

	FName BaseClassName = Base->GetFName();

	// Use the asset registry to get the set of all class names deriving from Base
	TSet< FName > DerivedNames;
	{
		TArray< FName > BaseNames;
		BaseNames.Add(BaseClassName);

		TSet< FName > Excluded;
		AssetRegistry.GetDerivedClassNames(BaseNames, Excluded, DerivedNames);
	}

	FARFilter Filter;
	Filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
	Filter.bRecursiveClasses = true;
	if (!Path.IsEmpty())
	{
		Filter.PackagePaths.Add(*Path);
	}
	Filter.bRecursivePaths = true;

	TArray< FAssetData > AssetList;
	AssetRegistry.GetAssets(Filter, AssetList);

	// Iterate over retrieved blueprint assets
	for (auto const& Asset : AssetList)
	{
		// Get the the class this blueprint generates (this is stored as a full path)
		if (auto GeneratedClassPathPtr = Asset.TagsAndValues.Find(TEXT("GeneratedClass")))
		{
			// Convert path to just the name part
			const FString ClassObjectPath = FPackageName::ExportTextPathToObjectPath(*GeneratedClassPathPtr);
			const FString ClassName = FPackageName::ObjectPathToObjectName(ClassObjectPath);

			// Check if this class is in the derived set
			if (!DerivedNames.Contains(*ClassName))
			{
				continue;
			}
			int32 StrIndex;
			if (!StateMachineNames.Find(ClassName, StrIndex))
			{
				const TCHAR* ClassObjectPathTChar = *ClassObjectPath;
				TSoftClassPtr<USMInstance> TheClassLooking = TSoftClassPtr<USMInstance>(FSoftClassPath(ClassObjectPathTChar));
				StateMachineDictionary.Add(ClassName, TheClassLooking);
			}

		}
	}

	return StateMachineDictionary;
}

TArray<FString> UDomeVRBlueprintLibrary::GetAllBlockGameModes() {

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetData;
	//const UClass* Class = ADomeVRBaseGameMode::StaticClass();
	//const UClass* Class = AGameMode::StaticClass();
	AssetRegistryModule.Get().GetAssetsByPath(FName(TEXT("/Game/Blueprints/BlockGameModes")), AssetData);
	//AssetRegistryModule.Get().GetAssetsByClass(FName(*Class->GetName()), AssetData);

	TArray<FString> Names = TArray<FString>();

	for (FAssetData asset : AssetData)
	{
		auto name =	asset.GetFullName();
		Names.Add(name);
	}
	return Names;
}

TArray<FString> UDomeVRBlueprintLibrary::GetAllBlockGameModePaths() {

	TArray<FString> BlockGameModes = UDomeVRBlueprintLibrary::GetAllBlockGameModes();
	TArray<FString> BlockGameModePaths = TArray<FString>();

	for (FString BlockGameMode : BlockGameModes) {

		TArray<FString> Out;
		BlockGameMode.ParseIntoArray(Out, TEXT(" "));
		BlockGameModePaths.Add(Out.Last());
	}

	return BlockGameModePaths;

}

TArray<FString> UDomeVRBlueprintLibrary::GetAllBlockGameModeNames() {

	TArray<FString> BlockGameModesPaths = UDomeVRBlueprintLibrary::GetAllBlockGameModePaths();
	TArray<FString> BlockGameModeNames = TArray<FString>();

	for (FString BlockGameMode : BlockGameModesPaths) {

		TArray<FString> Out;
		TArray<FString> Out2;
		BlockGameMode.ParseIntoArray(Out, TEXT("/"));
		Out.Last().ParseIntoArray(Out2, TEXT("."));
		BlockGameModeNames.Add(Out2[0]);
	}

	return BlockGameModeNames;

}

void UDomeVRBlueprintLibrary::DomeVRLoadLevel(FString LevelName, FString GameModePath) {
	FString PathOption = "?game=";
	PathOption.Append(GameModePath);
	UWorld* World = GEngine->GetWorld();
	UGameplayStatics::OpenLevel(World, *LevelName, false, PathOption);
}

void UDomeVRBlueprintLibrary::SetWidgetNavigation(bool AllowAnalog, bool AllowKey, bool AllowTab)
{
	auto& SlateApplication = FSlateApplication::Get();
	auto& NavigationConfig = *SlateApplication.GetNavigationConfig();

	NavigationConfig.bAnalogNavigation = AllowAnalog;
	NavigationConfig.bKeyNavigation = AllowKey;
	NavigationConfig.bTabNavigation = AllowTab;
}

AActor* UDomeVRBlueprintLibrary::CloneActor(AActor* actorToClone, const FTransform transforms, ESpawnActorCollisionHandlingMethod collisionMethod)
{
	if (actorToClone)
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = collisionMethod;
		return GEngine->GetWorld()->SpawnActorAbsolute<AActor>(actorToClone->GetClass(), transforms, params);
	}
	return 0;
}

UTrialPerformanceCounter * UDomeVRBlueprintLibrary::CreateTrialPerformanceCounterInstance(int FirstTrial, int FirstPerformanceCount)
{
	UTrialPerformanceCounter * TrialPerformanceCounterInstance = NewObject<UTrialPerformanceCounter>();
	TrialPerformanceCounterInstance->Init(FirstTrial, FirstPerformanceCount);
	return TrialPerformanceCounterInstance;
}

USavedTrialPerformanceCounter * UDomeVRBlueprintLibrary::CreateSavedTrialPerformanceCounterInstance(TMap<FName, UTrialPerformanceCounter*> CounterToSave)
{
	USavedTrialPerformanceCounter * SavedTrialPerformanceCounterInstance = NewObject<USavedTrialPerformanceCounter>();
	SavedTrialPerformanceCounterInstance->Init(CounterToSave);
	return SavedTrialPerformanceCounterInstance;
}

UTrialPerformanceChart * UDomeVRBlueprintLibrary::CreateTrialPerformanceChartInstance(int FirstTrial, float FirstPerformance)
{
	UTrialPerformanceChart * TrialPerformanceChartInstance = NewObject<UTrialPerformanceChart>();
	TrialPerformanceChartInstance->AddDataPoint(FirstTrial, FirstPerformance);
	return TrialPerformanceChartInstance;
}

FString UDomeVRBlueprintLibrary::FormatFloat(FString FormatString, float FloatNumber)
{
	const TCHAR * Test = *FormatString;
	FString OutString = FString::Printf(Test, FloatNumber);
	return OutString;
}

UStateMachineVariableEditor * UDomeVRBlueprintLibrary::CreateStateMachineVariableEditor(USMInstance* StateMachine, FString VariablePrefix, float RefreshRate, UObject* Outer)
{
	UStateMachineVariableEditor * StateMachineVariableEditorInstance = NewObject<UStateMachineVariableEditor>(Outer);
	StateMachineVariableEditorInstance->init(StateMachine, VariablePrefix, RefreshRate);
	return StateMachineVariableEditorInstance;
}

TArray<UClass*> UDomeVRBlueprintLibrary::GetClasses(UClass* ParentClass)
{
	TArray<UClass*> Results;

	// get our parent blueprint class
	const FString ParentClassName = ParentClass->GetName();
	UObject* ClassPackage = ANY_PACKAGE;
	UClass* ParentBPClass = FindObject<UClass>(ClassPackage, *ParentClassName);

	// iterate over UClass, this might be heavy on performance, so keep in mind..
	// better suggestions for a check are welcome
	for (TObjectIterator<UClass> It; It; ++It)
	{
		if (It->IsChildOf(ParentBPClass))
		{
			// It is a child of the Parent Class
			// make sure we don't include our parent class in the array (weak name check, suggestions welcome)
			if (It->GetName() != ParentClassName)
			{
				Results.Add(*It);
			}
		}
	}

	return Results;
}

TArray<UObject*> UDomeVRBlueprintLibrary::GetRunningStateMachines(UClass* ParentStateMachine)
{
	UClass* Base = USMInstance::StaticClass();
	TArray<UObject*> StateMachines;

	if (!ParentStateMachine->IsChildOf(Base))
	{
		return StateMachines;
	}
	// get our parent blueprint class
	const FString ParentClassName = ParentStateMachine->GetName();
	UObject* ClassPackage = ANY_PACKAGE;
	UClass* ParentBPClass = FindObject<UClass>(ClassPackage, *ParentClassName);

	// iterate over UClass, this might be heavy on performance, so keep in mind..
	// better suggestions for a check are welcome
	for (TObjectIterator<UObject> It; It; ++It)
	{
		if (It->GetClass()->IsChildOf(ParentBPClass))
		{
			// It is a child of the Parent Class
			// make sure we don't include our parent class in the array (weak name check, suggestions welcome)
			if (It->GetName() != ParentClassName)
			{
				USMInstance* CurrentStateMachine = Cast<USMInstance>(*It);
				if (IsValid(CurrentStateMachine) && CurrentStateMachine->IsActive())
				{
					StateMachines.Add(*It);
				}
				
			}
		}
	}

	return StateMachines;
}

bool UDomeVRBlueprintLibrary::GetStateMachineProperties(UObject* SomeStateMachine, FString VariablePrefix, TMap<FName, float> & FloatVariables, TMap<FName, int> & IntVariables, TMap<FName, bool> & BoolVariables, TMap<FName, FVector> & VectorVariables, TMap<FName, FString> & StringVariables)
{

	UClass* Base = USMInstance::StaticClass();

	if (!SomeStateMachine->GetClass()->IsChildOf(Base))
	{
		return false;
	}

	for (TFieldIterator<UProperty> PropIt(SomeStateMachine->GetClass()); PropIt; ++PropIt)
	{
		FName VariableName = PropIt->GetFName();
		FString CheckVariableName = VariableName.ToString();
		if (!CheckVariableName.StartsWith(VariablePrefix))
		{
			continue;
		}
		FName test = PropIt->GetFName();
		FString test2 = test.ToString();

		if (PropIt->GetCPPType().Equals(TEXT("float")))
		{
			UFloatProperty* FloatProperty = FindFieldChecked<UFloatProperty>(SomeStateMachine->GetClass(), VariableName);
			if (FloatProperty)
			{
				float FloatValue = FloatProperty->GetPropertyValue_InContainer(SomeStateMachine);
				FloatVariables.Add(VariableName, FloatValue);

			}

		}

		else if (PropIt->GetCPPType().Equals(TEXT("int32")))
		{
			UIntProperty* IntProperty = FindFieldChecked<UIntProperty>(SomeStateMachine->GetClass(), VariableName);
			if (IntProperty)
			{
				int IntValue = IntProperty->GetPropertyValue_InContainer(SomeStateMachine);
				IntVariables.Add(VariableName, IntValue);

			}
		}

		else if (PropIt->GetCPPType().Equals(TEXT("FVector")))
		{

			UProperty* VectorProperty = FindFieldChecked<UProperty>(SomeStateMachine->GetClass(), VariableName);
			if (VectorProperty)
			{
				void * VectorAddress = VectorProperty->ContainerPtrToValuePtr<void>(SomeStateMachine);

				if (UStructProperty * StructProperty = Cast<UStructProperty>(VectorProperty))
				{
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
			UBoolProperty* BoolProperty = FindFieldChecked<UBoolProperty>(SomeStateMachine->GetClass(), VariableName);
			if (BoolProperty)
			{
				bool BoolValue = BoolProperty->GetPropertyValue_InContainer(SomeStateMachine);
				BoolVariables.Add(VariableName, BoolValue);

			}
		}

		else if (PropIt->GetCPPType().Equals(TEXT("FString")))
		{
			UProperty* StringProperty = FindFieldChecked<UProperty>(SomeStateMachine->GetClass(), VariableName);
			if (StringProperty)
			{				
				FString * StringAddress = StringProperty->ContainerPtrToValuePtr<FString>(SomeStateMachine);
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

	return true;
}

bool UDomeVRBlueprintLibrary::SetStateMachineProperties(UObject* SomeStateMachine, TMap<FName, float> FloatVariables, TMap<FName, int> IntVariables, TMap<FName, bool> BoolVariables, TMap<FName, FVector> VectorVariables, TMap<FName, FString> StringVariables)
{
	UClass* Base = USMInstance::StaticClass();

	if (!SomeStateMachine->GetClass()->IsChildOf(Base))
	{
		return false;
	}

	for (auto& Element : FloatVariables)
	{
		FName VariableName = Element.Key;
		UFloatProperty* FloatProperty = FindFieldChecked<UFloatProperty>(SomeStateMachine->GetClass(), VariableName);

		if (FloatProperty)
		{
			float FloatValue = Element.Value;			
			FloatProperty->SetPropertyValue_InContainer(SomeStateMachine, FloatValue);
		}
	}
	
	for (auto& Element : IntVariables)
	{
		FName VariableName = Element.Key;
		UIntProperty* IntProperty = FindFieldChecked<UIntProperty>(SomeStateMachine->GetClass(), VariableName);

		if (IntProperty)
		{
			int IntValue = Element.Value;
			IntProperty->SetPropertyValue_InContainer(SomeStateMachine, IntValue);
		}
	}

	for (auto& Element : BoolVariables)
	{
		FName VariableName = Element.Key;
		UBoolProperty* BoolProperty = FindFieldChecked<UBoolProperty>(SomeStateMachine->GetClass(), VariableName);

		if (BoolProperty)
		{
			bool BoolValue = Element.Value;
			BoolProperty->SetPropertyValue_InContainer(SomeStateMachine, BoolValue);
		}
	}

	for (auto& Element : VectorVariables)
	{
		FName VariableName = Element.Key;
		UProperty* VectorProperty = FindFieldChecked<UProperty>(SomeStateMachine->GetClass(), VariableName);
		
		if (VectorProperty)
		{
			void * VectorAddress = VectorProperty->ContainerPtrToValuePtr<void>(SomeStateMachine);

			if (UStructProperty * StructProperty = Cast<UStructProperty>(VectorProperty))
			{
				FVector *v = StructProperty->ContainerPtrToValuePtr<FVector>(SomeStateMachine);
				FVector VectorValue = Element.Value;
				v->X = VectorValue.X;
				v->Y = VectorValue.Y;
				v->Z = VectorValue.Z;
			}
		}
	}

	for (auto& Element : StringVariables)
	{

		FName VariableName = Element.Key;
		UProperty* StringProperty = FindFieldChecked<UProperty>(SomeStateMachine->GetClass(), VariableName);

		if (StringProperty)
		{
			FString StringValue = FString(Element.Value);
			FString * StringAddress = StringProperty->ContainerPtrToValuePtr<FString>(SomeStateMachine);
			*StringAddress = StringValue;
		}
	}

	return true;
}

void UDomeVRBlueprintLibrary::DestroyObject(UObject* Object) 
{
	if (Object)
	{
		Object->ConditionalBeginDestroy();
	}
}

FSphericalCoordinates UDomeVRBlueprintLibrary::CartesianToSpherical(float X, float Y, float Z)
{
		float Radius = sqrt((pow(X,2) + pow(Y,2) + pow(Z,2)));
		float Azimuth = atan2(Y, X);
		float Elevation = atan(sqrt(pow(X, 2) + pow(Y, 2)) / Z);

		if (Elevation > 0)
		{
			Elevation = (M_PI / 2) - Elevation;
		}
		else if (Elevation < 0)
		{
			Elevation = (-M_PI / 2) - Elevation;
		}
		else
		{
			Elevation = 0;
		}

		FSphericalCoordinates SphericalCoordinates = { Azimuth, Elevation, Radius };
		return SphericalCoordinates;
}

FSphericalCoordinates UDomeVRBlueprintLibrary::CartesianToSphericald(float X, float Y, float Z)
{
	FSphericalCoordinates SphericalCoordinates = CartesianToSpherical(X, Y, Z);
	SphericalCoordinates.Radius = SphericalCoordinates.Radius;
	SphericalCoordinates.Azimuth = (SphericalCoordinates.Azimuth * 180) / M_PI;
	SphericalCoordinates.Elevation = (SphericalCoordinates.Elevation * 180) / M_PI;

	return SphericalCoordinates;
}

FVector UDomeVRBlueprintLibrary::SphericalToCartesian(float Azimuth, float Elevation, float Radius)
{
	float X = (cos(Azimuth) * cos(Elevation)) * Radius;
	float Y = (sin(Azimuth) * cos(Elevation)) * Radius;
	float Z = sin(Elevation) * Radius;
	FVector CartesianCoordinates = { X, Y, Z };

	return CartesianCoordinates;
}

FVector UDomeVRBlueprintLibrary::SphericalToCartesiand(float Azimuth, float Elevation, float Radius)
{
	Azimuth = Azimuth * (M_PI / 180);
	Elevation = Elevation * (M_PI / 180);
	FVector CartesianCoordinates = SphericalToCartesian(Azimuth, Elevation, Radius);

	return CartesianCoordinates;
}

FSphericalCoordinates UDomeVRBlueprintLibrary::RetinalToSpherical(FSphericalCoordinates RetinalCoordinates)
{
	// Given angular coordinates in retinal space, angular coordinates in spherical space are obtained by 
	// first calculating the Cartesian coordinates of the retinal coordinates as if they would be in spherical space
	FVector CartesianCoordinates = SphericalToCartesian(RetinalCoordinates.Azimuth, 
														RetinalCoordinates.Elevation, 
														RetinalCoordinates.Radius);

	// then rotate the points in Cartesian space by 90 around the Y-axis
	FVector RotateAxis = { 0.0, 1.0, 0.0 };
	float RotateAngle = 90.0;
	CartesianCoordinates = UKismetMathLibrary::RotateAngleAxis(CartesianCoordinates, RotateAngle, RotateAxis);

	// Finally obtain the coordinates in spherical space by converting back from Cartesian to spherical coordinates
	FSphericalCoordinates SphericalCoordinates = CartesianToSpherical(CartesianCoordinates.X, 
																	  CartesianCoordinates.Y, 
																	  CartesianCoordinates.Z);
	return SphericalCoordinates;
}

FSphericalCoordinates UDomeVRBlueprintLibrary::RetinalToSphericald(FSphericalCoordinates RetinalCoordinates)
{
	// Given angular coordinates in retinal space, angular coordinates in spherical space are obtained by 
	// first calculating the Cartesian coordinates of the retinal coordinates as if they would be in spherical space
	FVector CartesianCoordinates = SphericalToCartesiand(RetinalCoordinates.Azimuth * -1,
														 RetinalCoordinates.Elevation + 90,
														 RetinalCoordinates.Radius);
	
	// then rotate the points in Cartesian space by 90 around the Y-axis
	FVector RotateAxis = { 0.0, 1.0, 0.0 };
	float RotateAngle = 90.0;
	CartesianCoordinates = UKismetMathLibrary::RotateAngleAxis(CartesianCoordinates, RotateAngle, RotateAxis);
	
	// Finally obtain the coordinates in spherical space by converting back from Cartesian to spherical coordinates
	FSphericalCoordinates SphericalCoordinates = CartesianToSphericald(CartesianCoordinates.X,
																	   CartesianCoordinates.Y,
																	   CartesianCoordinates.Z);
	return SphericalCoordinates;
}

FSphericalCoordinates UDomeVRBlueprintLibrary::SphericalToRetinal(FSphericalCoordinates SphericalCoordinates)
{

	// Given angular coordinates in spherical space, angular coordinates in retinal space are obtained by 
	// first calculating the Cartesian coordinates of the spherical coordinates
	FVector CartesianCoordinates = SphericalToCartesian(SphericalCoordinates.Azimuth,
														SphericalCoordinates.Elevation,
														SphericalCoordinates.Radius);

	// then rotate the points in Cartesian space by 90 back around the Y-axis
	FVector RotateAxis = { 0.0, 1.0, 0.0 };
	float RotateAngle = -90.0;
	CartesianCoordinates = UKismetMathLibrary::RotateAngleAxis(CartesianCoordinates, RotateAngle, RotateAxis);

	// Finally obtain the coordinates in retinal space by converting back from Cartesian to spherical coordinates
	FSphericalCoordinates RetinalCoordinates = CartesianToSpherical(CartesianCoordinates.X,
																	CartesianCoordinates.Y,
																	CartesianCoordinates.Z);

	return RetinalCoordinates;
}

FSphericalCoordinates UDomeVRBlueprintLibrary::SphericalToRetinald(FSphericalCoordinates SphericalCoordinates)
{
	// Given angular coordinates in spherical space, angular coordinates in retinal space are obtained by 
	// first calculating the Cartesian coordinates of the spherical coordinates
	FVector CartesianCoordinates = SphericalToCartesiand(SphericalCoordinates.Azimuth,
														 SphericalCoordinates.Elevation,
														 SphericalCoordinates.Radius);

	// then rotate the points in Cartesian space by 90 back around the Y-axis
	FVector RotateAxis = { 0.0, 1.0, 0.0 };
	float RotateAngle = -90.0;
	CartesianCoordinates = UKismetMathLibrary::RotateAngleAxis(CartesianCoordinates, RotateAngle, RotateAxis);

	// Finally obtain the coordinates in retinal space by converting back from Cartesian to spherical coordinates
	FSphericalCoordinates RetinalCoordinates = CartesianToSphericald(CartesianCoordinates.X,
																	 CartesianCoordinates.Y,
																	 CartesianCoordinates.Z);
	RetinalCoordinates.Azimuth = RetinalCoordinates.Azimuth;// +270; //* -1;
	RetinalCoordinates.Elevation = RetinalCoordinates.Elevation +90;//+ 90;

	return RetinalCoordinates;
}

float UDomeVRBlueprintLibrary::GetDensityFromMap(UTexture2D *MyTexture2D, float X, float Y)
{
	if (MyTexture2D == nullptr)
		return 1;

#if WITH_EDITORONLY_DATA
	TextureCompressionSettings OldCompressionSettings = MyTexture2D->CompressionSettings;
	TextureMipGenSettings OldMipGenSettings = MyTexture2D->MipGenSettings;
	bool OldSRGB = MyTexture2D->SRGB;

	MyTexture2D->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	MyTexture2D->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	MyTexture2D->SRGB = false;
	MyTexture2D->UpdateResource();
#endif

	const FColor* FormatedImageData = static_cast<const FColor*>(MyTexture2D->PlatformData->Mips[0].BulkData.LockReadOnly());

	int32 XPixel = round(float(MyTexture2D->GetSizeX()) * X);
	int32 YPixel = round(float(MyTexture2D->GetSizeY()) * Y);
	FColor PixelColor = FormatedImageData[YPixel * MyTexture2D->GetSizeX() + XPixel];
	float Density = (PixelColor.R + PixelColor.G + PixelColor.B) / 3;
	Density = Density / 255;
	//float Density = ((PixelColor.R + PixelColor.G + PixelColor.B) / 3) / 255;

#if WITH_EDITORONLY_DATA
	MyTexture2D->PlatformData->Mips[0].BulkData.Unlock();

	MyTexture2D->CompressionSettings = OldCompressionSettings;
	MyTexture2D->MipGenSettings = OldMipGenSettings;
	MyTexture2D->SRGB = OldSRGB;
	MyTexture2D->UpdateResource();
#endif

	return Density;
}

FVector UDomeVRBlueprintLibrary::GetRandomPointInBoundingBox(FVector Origin, FVector BoxExtent, FRandomStream &RandomStream)
{
	FVector RandomPoint;
	RandomPoint.X = (RandomStream.FRand() * 2 * BoxExtent.X) - BoxExtent.X;
	RandomPoint.Y = (RandomStream.FRand() * 2 * BoxExtent.Y) - BoxExtent.Y;
	RandomPoint.Z = (RandomStream.FRand() * 2 * BoxExtent.Z) - BoxExtent.Z;

	RandomPoint = RandomPoint + Origin;

	return RandomPoint;
	
}

void UDomeVRBlueprintLibrary::ShuffleArrayWithStream(const TArray<int32>& TargetArray, const FRandomStream& Stream)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the _impl equivalent instead
	UE_LOG(LogTemp, Error, TEXT("Stub shuffle func called - should not happen"));
	check(0);
}

void UDomeVRBlueprintLibrary::ShuffleArrayWithStream_impl(void* TargetArray, const UArrayProperty* ArrayProp, const FRandomStream& Stream)
{
	if (TargetArray)
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
		int32 LastIndex = ArrayHelper.Num() - 1;

		for (int32 i = 0; i < LastIndex; ++i)
		{
			int32 Index = Stream.RandRange(i, LastIndex);
			if (i != Index)
			{
				ArrayHelper.SwapValues(i, Index);
			}
		}
	}
}

//FSphericalCoordinate CartesianToSpherical(float X, float Y, float Z)
//{
//	
//	float Radius = sqrt((pow(X,2) + pow(Y,2) + pow(Z,2)));
//	float Azimuth = atan2(Y, X);
//	float Elevation = atan(sqrt(pow(X, 2) + pow(Y, 2)) / Z);
//	//FSphericalCoordinates SphericalCoordinates = { Azimuth, Elevation, Radius };
//	FSphericalCoordinate SphericalCoordinates;
//	return SphericalCoordinates;
//}
//int32 UDomeVRBlueprintLibrary::GetTextureWidth(UTexture2D *ActTexture)
//{
//	int32 Width = ActTexture->GetSizeX();
//	return Width;
//}
//
//int32 UDomeVRBlueprintLibrary::GetTextureHeight(UTexture2D *ActTexture)
//{
//	int32 Height = ActTexture->GetSizeY();
//	return Height;
//}
//
//float UDomeVRBlueprintLibrary::GetTextureAspectRatio(UTexture2D *ActTexture)
//{
//	int32 Width = ActTexture->GetSizeX();
//	int32 Height = ActTexture->GetSizeY();
//
//	return (float) Width / (float) Height;
//}
//
//float UDomeVRBlueprintLibrary::GetDynamicTextureAspectRatio(UTexture2DDynamic *ActTexture)
//{
//	float Width = ActTexture->GetSurfaceWidth();
//	float Height = ActTexture->GetSurfaceHeight();
//
//	return Width / Height;
//}