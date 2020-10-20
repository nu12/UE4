// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "SplineBase.h"
#include "AdvancedSpline.generated.h"

/**
 * The Advanced Spline Mesh contains Roll and Scale (Width And Thickness) setup for each spline segment.
 */
UCLASS()
class AAdvancedSpline : public ASplineBase
{
	GENERATED_BODY()

public:
	void OnConstruction(const FTransform& Transform) override;

/* Setup */
protected:
	UPROPERTY(EditAnywhere, Category = "Setup|StaticMesh")
	UStaticMesh* MainMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Setup|StaticMesh|Transform")
	TArray<float> Roll;

	UPROPERTY(EditAnywhere, Category = "Setup|StaticMesh|Transform")
	TArray<FVector2D> WidthAndThickness;

/* Helper functions */
protected:
	UFUNCTION()
	void SetupAdvancedSpline(UStaticMesh* MeshToSet, int32 Index);

};
