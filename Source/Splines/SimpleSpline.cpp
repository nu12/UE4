// Copyright nu12 2020.


#include "SimpleSpline.h"
#include "Engine/StaticMesh.h"

void ASimpleSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	for (int32 Index = 0; Index <= SplineComponent->GetNumberOfSplinePoints() - GetOffset(); Index++)
	{
		USplineMeshComponent* SplineMeshComponent = AddSplineMeshComponent();
		SetStaticMeshWithStartAndEnd(SplineMeshComponent, Index, StaticMesh);
	}
}

