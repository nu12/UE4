// Copyright nu12 2020.


#include "AdvancedSpline.h"
#include "Engine/StaticMesh.h"

void AAdvancedSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetArrayLength<float>(&Roll, 0.f);
	SetArrayLength<FVector2D>(&WidthAndThickness, FVector2D(1.f, 1.f));
	
	for (int32 Index = 0; Index <= SplineComponent->GetNumberOfSplinePoints() - GetOffset(); Index++)
	{
		SetupAdvancedSpline(MainMesh, Index);
	}
}

void AAdvancedSpline::SetupAdvancedSpline(UStaticMesh* MeshToSet, int32 Index)
{
	USplineMeshComponent* SplineMeshComponent = AddSplineMeshComponent();
	SetStaticMeshWithStartAndEnd(SplineMeshComponent, Index, MeshToSet);
	SetRoll(SplineMeshComponent, Roll[Index], Roll[GetNextIndex(Index)]);
	SetScale(SplineMeshComponent, WidthAndThickness[Index], WidthAndThickness[GetNextIndex(Index)]);
}