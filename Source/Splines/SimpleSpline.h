// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "SplineBase.h"
#include "SimpleSpline.generated.h"

class UStaticMesh;

/**
 * The Simple Spline is a basic implementation of a Spline with a Mesh.
 * If you need to controll mesh transformation, use AdvancedSpline.
 */
UCLASS()
class ASimpleSpline : public ASplineBase
{
	GENERATED_BODY()

public:
	void OnConstruction(const FTransform& Transform) override;

/* Setup */
private:
	UPROPERTY(EditAnywhere, Category = "Setup")
	UStaticMesh* StaticMesh = nullptr;

};