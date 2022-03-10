#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/SplineComponent.h"
#include "BPSplineLibrary.generated.h"

/**
 *
 */
UCLASS()
class Uspline_extensions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/* Sets the rotation on a spline point Local rotation only.*/
	UFUNCTION(BlueprintCallable, Category = "Splines") //make the function usable in blueprints. BlueprintPure doesn't require exec; BlueprintCallable does.
		static void set_rotation_at_spline_point(USplineComponent* target, const int32 point_index, const FRotator rotation); //define inputs and outputs. Const for inputs, and & for outputs.


};