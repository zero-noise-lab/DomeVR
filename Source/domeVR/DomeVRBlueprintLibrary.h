// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Math/RandomStream.h"
#include "Engine/Texture2DDynamic.h"
#include "Engine/Texture2D.h"
#include "Math/Color.h"
#include "Misc/DateTime.h"
#include "Engine/ObjectLibrary.h"
#include "AssetRegistryModule.h"
#include "Engine/World.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DomeVRBaseGameMode.h"
#include "Framework/Application/SlateApplication.h"
#include "SMInstance.h"
#include "TrialPerformanceCounter.h"
#include "TrialPerformanceChart.h"
#include "SavedTrialPerformanceCounter.h"
#include "TrialNumbers.h"
#include "StateMachineVariableEditor.h"
#include "Style/KantanSeriesStyle.h"
#include <string>
#define _USE_MATH_DEFINES
#include <math.h> 
#include "DomeVRBlueprintLibrary.generated.h"


USTRUCT(Blueprintable, BlueprintType)
struct FSphericalCoordinates
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
	float Azimuth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
	float Elevation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
	float Radius;
};

USTRUCT(Blueprintable, BlueprintType)
struct FTrialPerformanceDataStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
	int TrialNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
	TMap<FName, int> CounterToIncrement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
	TMap<FName, UTrialPerformanceCounter*> TrialPerformanceCounter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
	TMap<FName, UTrialPerformanceChart*> TrialPerformanceChart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
	TMap<FName, FKantanSeriesStyle> TrialPerformanceChartStyles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
	TMap<int, FName> ID_Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
	TArray<int> TotalPerformanceCounts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
	int TotalPerformanceCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
	TArray<FKantanSeriesStyle> DefaultChartStyles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
	FKantanSeriesStyle DefaultChartStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
	bool IsInitialized;
};

USTRUCT(Blueprintable, BlueprintType)
struct FTrialLevelPerformanceDataStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
		FTrialPerformanceDataStruct TrialPerformanceData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
		FTrialPerformanceDataStruct TrialPerformanceDataCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
		FTrialPerformanceDataStruct TrialPerformanceDataBlock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
		TMap<FName, FTrialPerformanceDataStruct> SavedTrialPerformanceDataBlock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
		TMap<FName, FTrialPerformanceDataStruct> SavedTrialPerformanceDataCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
		FName CurrentConditionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
		FName CurrentBlockName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
		TArray<FName> CounterNames;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
		bool IsInitialized;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
		TArray<FName> TrialConditionNames;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DomeVR Data Types")
		TArray<FName> TrialBlockNames;
};




/**
 * Blueprint helper function library of DomeVR project
 */
UCLASS()
class DOMEVR_API UDomeVRBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	* Function for generating a random alphanumeric ID of length IDLength
	* @return Random alphanumeric ID
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Tools")
	static FString RandomObjectID(FRandomStream RandStream, int IDLength);

	/**
	* Function to expose interal UE4 object IDs to blueprints as string 
	(UE4 object IDs are not exposed to natively BPs since unsigned integers are not supported there..)
	* @return UE4 Object ID as string
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Tools")
	static FString GetUE4ObjectID(UObject *ActObject);

	/**
	* Function to convert an array of bools to an array of integers.
	* @return Array of integers
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Tools")
	static TArray<int> BoolToIntArray(TArray<bool> BoolArray);
	
	/**
	* Function that returns the current time as string.
	* @return Time as string
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Tools")
	static FString GetCurrentTimeAsString();


	/**
	* Function that returns the current date as string.
	* @return Time as string
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Tools")
	static FString GetCurrentDateAsString();

	/**
	* Function that returns all map names in /Content/Maps and /Content/Blueprints/StateMachines/Users without
	* "Layer" in its path name.
	* @return String array of map names
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Assets")
	static TArray<FString> GetAllMapNames();

	/**
	* Function that returns all map names in /Content/Blueprints/StateMachines/"UserFolder" without
	* "Layer" in its path name.
	* @return String array of map names
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Assets")
	static TArray<FString> GetAllMapNamesInUserFolder(FString UserFolder);


	/**
	* Function that returns a dictionary with state machine names as keys and their soft class pointers as values.
	* @return <FString,TSoftClassPtr<USMInstance>> dictionary
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Assets")
	static TMap<FString, TSoftClassPtr<USMInstance>> GetAllStateMachineClasses(FString UserFolder);


	/**
	* Function that returns all assets in BlockGameModes folder.
	* @return Assets in BlockGameModes folder
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Assets")
	static TArray<FString> GetAllBlockGameModes();
	
	/**
	* Function that returns all paths of assets in BlockGameModes folder.
	* @return Paths to all assets in BlockGameModes folder
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Assets")
	static TArray<FString> GetAllBlockGameModePaths();

	/**
	* Function that returns all names of assets in BlockgameModes folder.
	* @return Names of assets in BlockgameModes folder
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Assets")
	static TArray<FString> GetAllBlockGameModeNames();

	/**
	* Function that loads a level using the Game Mode in the path given as GameModePath.
	* @param LevelName - Name of level to load
	* @param GameModePath - Path of GameMode to load
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Tools")
	static void DomeVRLoadLevel(FString LevelName, FString GameModePath);

	/**
	* Function that enables to turn on/off switching between UI elements by the 
	Gamepad analog stick, Keyboard arrows and Keyboard tabs.
	* @param AllowAnalog - Allow analog stick input to control UI elements
	* @param transform - Transform where cloned actor should be spawned
	* @param collisionMethod - How to handle collisions
	* @return Cloned actor
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library")
	static void SetWidgetNavigation(bool AllowAnalog, bool AllowKey, bool AllowTab);

	/**
	* Function to spawn a clone of a given actor instance.
	* @param actorToClone - Actor that should be cloned
	* @param transform - Transform where cloned actor should be spawned
	* @param collisionMethod - How to handle collisions
	* @return Cloned actor
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Constructors")
	static AActor * CloneActor(AActor * actorToClone, const FTransform transforms, ESpawnActorCollisionHandlingMethod collisionMethod);

	/**
	* Helper function to create a TrialPerformanceCounter instance
	* @param FirstTrial - First trial of the counter
	* @param FirstPerformanceCount - Performance count of the FirstTrial
	* @return Initialized TrialPerformanceCounter instance
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Constructors")
	static UTrialPerformanceCounter * CreateTrialPerformanceCounterInstance(int FirstTrial, int FirstPerformanceCount);

	/**
	* Helper function to create a SavedTrialPerformanceCounter instance
	* @param CounterToSave - TrialPerformanceCounter to save
	* @return Initialized SavedTrialPerformanceCounter instance
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Constructors")
	static USavedTrialPerformanceCounter * CreateSavedTrialPerformanceCounterInstance(TMap<FName, UTrialPerformanceCounter*> CounterToSave);

	/**
	* Helper function to create a TrialPerformanceChart instance
	* @param FirstTrial - Trial of the first data point
	* @param FirstPerformanceCount - Performance value of the first data point
	* @return Initialized TrialPerformanceChart instance
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Constructors")
	static UTrialPerformanceChart * CreateTrialPerformanceChartInstance(int FirstTrial, float FirstPerformance);

	/**
	* Helper function to run printf on a float number
	* @param FormatString - printf arguments for formatting float number
	* @param FloatNumber - Float number to put out as formatted string
	* @return Float number as formatted string
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Tools")
	static FString FormatFloat(FString FormatString, float FloatNumber);

	/**
	* Helper function that constructs a StateMachineVariableEditor instance based on the given parameters
	* @param StateMachine - Reference to the state machine
	* @param VariablePrefix - Prefix for variables that should be edited
	* @param VariablePrefix - Refresh rate to read variable values in seconds
	* @param VariablePrefix - Outer object that references this StateMachineVariableEditor
	* @return StateMachineVariableEditor instance
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Constructors")
	static UStateMachineVariableEditor * CreateStateMachineVariableEditor(USMInstance* StateMachine, FString VariablePrefix, float RefreshRate, UObject* Outer);

	/**
	* Function that returns all child classes of a given parent class
	* @param ParentClass - Parent class of child classes that are looked for
	* @return Array of found child classes
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Child Classes", Keywords = "Get Child Classes"), Category = "DomeVR Blueprint Library|Tools")
	static TArray<UClass*> GetClasses(UClass* ParentClass);

	/**
	* Function that returns all child classes of a given parent class
	* @param ParentClass - Parent class of child classes that are looked for
	* @return Array of found child classes
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Running State Machines", Keywords = "Get Running State Machines"), Category = "DomeVR Blueprint Library|Tools")
	static TArray<UObject*> GetRunningStateMachines(UClass* ParentStateMachineClass);

	/**
	* Function that returns float, int, bool, string and vector variables for a state machine and a given prefix
	* @param SomeStateMachine - State machine reference to read variables from
	* @param VariablePrefix - Variable name prefix of variables that should be read
	* @return true if state machine reference pointed to valid state machine object false otherwise. Different dictionaries containing variable names and their values
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get State Machine Properties", Keywords = "Get State Machine Properties"), Category = "DomeVR Blueprint Library|Tools")
	static bool GetStateMachineProperties(UObject* SomeStateMachine, FString VariablePrefix, TMap<FName, float> & FloatVariables, TMap<FName, int> & IntVariables, TMap<FName, bool> & BoolVariables, TMap<FName, FVector> & VectorVariables, TMap<FName, FString> & StringVariables);

	/**
	* Function that sets float, int, bool, string and vector variables of a state machine based on the given dictionaries
	* @param SomeStateMachine - State machine reference to read variables from
	* @param FloatVariables - Dictionary containing float variable names and values
	* @param IntVariables - Dictionary containing int variable names and values
	* @param BoolVariables - Dictionary containing bool variable names and values
	* @param VectorVariables - Dictionary containing vector variable names and values
	* @param StringVariables - Dictionary containing string variable names and values
	* @return true if state machine reference pointed to valid state machine object false otherwise
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set State Machine Properties", Keywords = "Set State Machine Properties"), Category = "DomeVR Blueprint Library|Tools")
	static bool SetStateMachineProperties(UObject* SomeStateMachine, TMap<FName, float> FloatVariables, TMap<FName, int> IntVariables, TMap<FName, bool> BoolVariables, TMap<FName, FVector> VectorVariables, TMap<FName, FString> StringVariables);

	/**
	* Function to destroy an arbitrary UObject.
	* @param Object - Given UObject
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Tools", meta = (DefaultToSelf = Object))
	static void DestroyObject(UObject* Object);

	/**
	* Function to convert Cartesian coordinates to our spherical coordinate system (in radians) centered at the horizon.
	* @param X - X-coordinate
	* @param Y - Y-coordinate
	* @param Z - Z-coordinate
	* @return Spherical coordinates in radians
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Coordinate Conversions")
	static FSphericalCoordinates CartesianToSpherical(float X, float Y, float Z);

	/**
	* Function to convert Cartesian coordinates to our spherical coordinate system (in degrees) centered at the horizon.
	* @param X - X-coordinate
	* @param Y - Y-coordinate
	* @param Z - Z-coordinate
	* @return Spherical coordinates in degrees
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Coordinate Conversions")
	static FSphericalCoordinates CartesianToSphericald(float X, float Y, float Z);


	/**
	* Function to convert Cartesian coordinates to our spherical coordinate system (in degrees) centered at the horizon.
	* @param Elevation - Elevation-coordinate
	* @param Azimuth - Azimuth-coordinate
	* @param Radius - Radius-coordinate
	* @return Cartesian coordinates
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Coordinate Conversions")
	static FVector SphericalToCartesian(float Azimuth, float Elevation, float Radius);

	/**
	* Function to convert Cartesian coordinates to our spherical coordinate system (in degrees) centered at the horizon.
	* @param Elevation - Elevation-coordinate
	* @param Azimuth - Azimuth-coordinate
	* @param Radius - Radius-coordinate
	* @return Cartesian coordinates
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Coordinate Conversions")
	static FVector SphericalToCartesiand(float Azimuth, float Elevation, float Radius);

	/**
	* Function to convert retinal coordinates (in radians) to spherical coordinates (in radians).
	* @param RetinalCoordinates - Retinal coordinates
	* @return Spherical coordinates in radians
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Coordinate Conversions")
	static FSphericalCoordinates RetinalToSpherical(FSphericalCoordinates RetinalCoordinates);


	/**
	* Function to convert retinal coordinates (in degrees) to spherical coordinates (in degrees).
	* @param RetinalCoordinates - Retinal coordinates
	* @return Spherical coordinates in degrees
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Coordinate Conversions")
	static FSphericalCoordinates RetinalToSphericald(FSphericalCoordinates RetinalCoordinates);

	/**
	* Function to convert spherical coordinates (in radians) to retinal coordinates (in radians).
	* @param SphericalCoordinates - Spherical coordinates
	* @return Retinal coordinates in radians
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Coordinate Conversions")
	static FSphericalCoordinates SphericalToRetinal(FSphericalCoordinates SphericalCoordinates);


	/**
	* Function to convert retinal coordinates (in degrees) to spherical coordinates (in degrees).
	* @param SphericalCoordinates - Spherical coordinates
	* @return Retinal coordinates in degrees
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Coordinate Conversions")
	static FSphericalCoordinates SphericalToRetinald(FSphericalCoordinates SphericalCoordinates);

	/**
	* Function to get a density value sample from a texture serving as density map.
	* @param MyTexture2D - Reference to texture serving as density map
	* @param X - Relative X-coordinate
	* @param Y - Relative Y-coordinate
	* @return Sampled density
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Tools")
	static float GetDensityFromMap(UTexture2D *MyTexture2D, float X, float Y);

	/**
	* Function to get a density value sample from a texture serving as density map.
	* @param MyTexture2D - Reference to texture serving as density map
	* @param X - Relative X-coordinate
	* @param Y - Relative Y-coordinate
	* @return Sampled density
	*/
	UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library|Random")
	static FVector GetRandomPointInBoundingBox(FVector Origin, FVector BoxExtent, UPARAM(ref) FRandomStream &RandomStream);

	/**
	 * Shuffle (randomize) the elements of an array
	 *
	 *@param	TargetArray - The array to shuffle
	 *@param	ArrayProperty -	Type of the array
	 *@param	FRandomStream -	Random stream
	*/

	// Native functions that will be called by the below custom thunk layers, which read off the property address, and call the appropriate native handler
	static void ShuffleArrayWithStream_impl(void* TargetArray, const UArrayProperty* ArrayProperty, const FRandomStream& Stream); // Real implementation


	//// Returns the width of a given texture.
	//UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library")
	//static int32 GetTextureWidth(UTexture2D *ActTexture);

	//// Returns the height of a given texture
	//UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library")
	//static int32 GetTextureHeight(UTexture2D *ActTexture);

	//// Returns the aspect ratio of a given texture
	//UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library")
	//static float GetTextureAspectRatio(UTexture2D *ActTexture);

	//// Returns the aspect ratio of a given texture
	//UFUNCTION(BlueprintCallable, Category = "DomeVR Blueprint Library")
	//static float GetDynamicTextureAspectRatio(UTexture2DDynamic *ActTexture);


public:

	UFUNCTION(BlueprintCallable, CustomThunk, meta = (DisplayName = "Shuffle by Stream", CompactNodeTitle = "SHUFFLESTREAM", ArrayParm = "TargetArray|ArrayProperty"), Category = "DomeVR Blueprint Library|Random")
	static void ShuffleArrayWithStream(const TArray<int32>& TargetArray, const FRandomStream& Stream); // Stub function

	DECLARE_FUNCTION(execShuffleArrayWithStream)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<UArrayProperty>(NULL);
		void* ArrayAddr = Stack.MostRecentPropertyAddress;
		UArrayProperty* ArrayProperty = Cast<UArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		P_GET_STRUCT_REF(FRandomStream, Stream);

		P_FINISH;
		P_NATIVE_BEGIN;
		ShuffleArrayWithStream_impl(ArrayAddr, ArrayProperty, Stream);
		P_NATIVE_END;
	}
};
