// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "SplineBase.h"
#include "MultiMeshSpline.generated.h"

/**
 * The Multi Mesh Spline can place multiple meshes at the same time.
 * This class can controll Location and Rotation offsets.
 */
UCLASS()
class AMultiMeshSpline : public ASplineBase
{
	GENERATED_BODY()


public:
	void OnConstruction(const FTransform& Transform) override;

/* Setup */
protected:
	UPROPERTY(EditAnywhere, Category = "Setup|StaticMesh")
		TArray<UStaticMesh*> Meshes;

	UPROPERTY(EditAnywhere, Category = "Setup|StaticMesh|Transform")
		TArray<FVector> Offset;

	UPROPERTY(EditAnywhere, Category = "Setup|StaticMesh|Transform")
		TArray<FRotator> Rotation;
	
};
