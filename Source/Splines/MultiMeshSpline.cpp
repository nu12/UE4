// Copyright nu12 2020.


#include "MultiMeshSpline.h"

void AMultiMeshSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetArrayLength<FVector, UStaticMesh>(&Offset, FVector(0.f), &Meshes);
	SetArrayLength<FRotator, UStaticMesh>(&Rotation, FRotator(0.f), &Meshes);

	for (int32 IndexSplinePoints = 0; IndexSplinePoints <= SplineComponent->GetNumberOfSplinePoints() - GetOffset(); IndexSplinePoints++)
	{
		for (int32 IndexMesh = 0; IndexMesh <= Meshes.Num() - 1; IndexMesh++)
		{
			USplineMeshComponent* SplineMeshComponent = AddSplineMeshComponent();
			SetStaticMeshWithStartAndEnd(SplineMeshComponent, IndexSplinePoints, Meshes[IndexMesh]);
			SplineMeshComponent->AddLocalOffset(Offset[IndexMesh]);
			SplineMeshComponent->AddLocalRotation(Rotation[IndexMesh]);
		}
	}
}