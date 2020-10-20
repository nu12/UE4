// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "SplineBase.generated.h"

struct LocationAndTangentFromSplinePoint
{
	FVector Location;
	FVector Tangent;
	LocationAndTangentFromSplinePoint(int32 Index, USplineComponent* Spline)
	{
		Spline->GetLocalLocationAndTangentAtSplinePoint(Index, Location, Tangent);
	}
};

/** 
* The Spline Base class provides standard implementation and helpers for common tasks related to splines.
*/
UCLASS()
class ASplineBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASplineBase();

	void OnConstruction(const FTransform& Transform) override;

/* Spline-specific functions */
protected:
	UFUNCTION()
	USplineMeshComponent* AddSplineMeshComponent();

	UFUNCTION()
	UTextRenderComponent* AddTextRenderComponent(int32 Index);

	UFUNCTION()
	void SetStartAndEnd(USplineMeshComponent* SplineMeshComponent, int32 Index);
	
	UFUNCTION()
	void SetStaticMeshWithStartAndEnd(USplineMeshComponent* SplineMesh, int32 Index, UStaticMesh* MeshToSet);
	
	UFUNCTION()
	void SetRoll(USplineMeshComponent* SplineMesh, float Start, float End = -1.f);
	
	UFUNCTION()
	void SetScale(USplineMeshComponent* SplineMesh, FVector2D Start, FVector2D End = FVector2D(-1.f, -1.f));

/* Helper functions */
protected:
	UFUNCTION()
	int32 GetNextIndex(int32 CurrentIndex);

	UFUNCTION()
	int32 GetOffset();

	UFUNCTION()
	void ClearLabels();

	UFUNCTION()
	void CreateLabels();

/* Setup */
private:
	UPROPERTY(EditAnywhere, Category = "Setup")
	bool bIsLooping = false;

	UPROPERTY(EditAnywhere, Category = "Setup")
	bool bIsCollisionEnabled = false;

	UPROPERTY(EditAnywhere, Category = "Setup|Labels")
	bool bDisplayPointNumbers= false;

	UPROPERTY(EditAnywhere, Category = "Setup|Labels")
	FRotator PointNumbersRotation = FRotator(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Setup|Labels")
	FVector PointNumbersOffset = FVector(0.f, 0.f, 50.f);

/* Template functions */
protected:
	template<typename T, typename U = UObject>
	void SetArrayLength(TArray<T>* InArray, T DefaultValue, TArray<U*>* CompareArray = nullptr)
	{
		int32 DesiredArrayLength = SplineComponent->GetNumberOfSplinePoints();

		if (CompareArray)
			DesiredArrayLength = CompareArray->Num();

		while (InArray->Num() != DesiredArrayLength)
		{
			if (InArray->Num() > DesiredArrayLength)
				InArray->RemoveAt(InArray->Num() - 1);
			if (InArray->Num() < DesiredArrayLength)
				InArray->Add(DefaultValue);
		}
	}

/* Components */
protected:
	UPROPERTY(EditAnywhere, Category = " ")
	USplineComponent* SplineComponent;

};
