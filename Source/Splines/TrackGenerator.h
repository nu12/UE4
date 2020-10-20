// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedSpline.h"
#include "TrackGenerator.generated.h"

/**
 * This class is the C++ version of the TrackGenerator of the UE4 spline & spline components tutorial in this address:
 * https://www.youtube.com/watch?v=wR0fH6O9jD8&t=4s
 * Some of its functionalities are implemented in the parend classes for reusability.
 */
UCLASS()
class SPLINES_API ATrackGenerator : public AAdvancedSpline
{
	GENERATED_BODY()

public:
	void OnConstruction(const FTransform& Transform) override;

/* Setup */
protected:
	UPROPERTY(EditAnywhere, Category = "Setup|StaticMesh")
	UStaticMesh* LeftGuardRail = nullptr;

	UPROPERTY(EditAnywhere, Category = "Setup|StaticMesh")
	UStaticMesh* RightGuardRail = nullptr;

	UPROPERTY(EditAnywhere, Category = "Setup|StaticMesh|GuardRail")
	TArray<bool> ShowLeftGuardRail;

	UPROPERTY(EditAnywhere, Category = "Setup|StaticMesh|GuardRail")
	TArray<bool> ShowRightGuardRail;

};
