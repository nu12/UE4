// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackGenerator.h"

void ATrackGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetArrayLength<bool>(&ShowLeftGuardRail, false);
	SetArrayLength<bool>(&ShowRightGuardRail, false);

	for (int32 Index = 0; Index <= SplineComponent->GetNumberOfSplinePoints() - GetOffset(); Index++)
	{
		if (ShowLeftGuardRail[Index])
			SetupAdvancedSpline(LeftGuardRail, Index);

		if (ShowRightGuardRail[Index])
			SetupAdvancedSpline(RightGuardRail, Index);
	}
}