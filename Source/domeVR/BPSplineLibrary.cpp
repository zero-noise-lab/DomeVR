#include "BPSplineLibrary.h"
#include <cstdlib>

void Uspline_extensions::set_rotation_at_spline_point(USplineComponent* target, const int32 point_index, const FRotator rotation) //exactly the same as in .h but without 'static' and with Uspline_extensions:: in front of the function name
{
	FInterpCurveQuat& SplineRotInfo = target->GetSplinePointsRotation(); //get the array of rotation data in the spline component

	FInterpCurvePoint<FQuat>& EditedRotPoint = SplineRotInfo.Points[point_index]; //get the point to edit

	FQuat NewRot = rotation.Quaternion(); //convert the given rotation into a quaternion

	EditedRotPoint.OutVal = NewRot; //set the new rotation of the selected point

}