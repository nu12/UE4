// Copyright nu12 2020.

#include "SplineBase.h"

ASplineBase::ASplineBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComponent->SetupAttachment(RootComponent);
}

void ASplineBase::OnConstruction(const FTransform& Transform)
{
	ClearLabels();
	if (bDisplayPointNumbers) CreateLabels();
}

USplineMeshComponent* ASplineBase::AddSplineMeshComponent()
{ // https://freelancerlife.info/en/blog/unreal-engine-421-convert-spline-actor-bp-c-code/
	USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
	SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	SplineMeshComponent->SetMobility(EComponentMobility::Movable);
	SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::Type::X, true);
	SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
	return SplineMeshComponent;
}

UTextRenderComponent* ASplineBase::AddTextRenderComponent(int32 Index)
{ 
	UTextRenderComponent* TextRender = NewObject<UTextRenderComponent>(this, UTextRenderComponent::StaticClass());
	TextRender->RegisterComponentWithWorld(GetWorld());
	TextRender->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TextRender->SetWorldLocation(SplineComponent->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World));
	TextRender->AddLocalOffset(PointNumbersOffset);
	TextRender->AddLocalRotation(PointNumbersRotation);

	return TextRender;
}

void ASplineBase::SetStaticMeshWithStartAndEnd(USplineMeshComponent* SplineMesh, int32 Index, UStaticMesh* MeshToSet)
{
	SetStartAndEnd(SplineMesh, Index);
	SplineMesh->SetStaticMesh(MeshToSet);

	if(bIsCollisionEnabled)
		SplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ASplineBase::SetStartAndEnd(USplineMeshComponent* SplineMesh, int32 Index)
{
	LocationAndTangentFromSplinePoint Start = LocationAndTangentFromSplinePoint(Index, SplineComponent);
	LocationAndTangentFromSplinePoint End = LocationAndTangentFromSplinePoint(GetNextIndex(Index), SplineComponent);

	SplineMesh->SetStartAndEnd(Start.Location, Start.Tangent, End.Location, End.Tangent);
}

void ASplineBase::SetRoll(USplineMeshComponent* SplineMesh, float Start, float End)
{
	SplineMesh->SetStartRoll(Start);
	SplineMesh->SetEndRoll((End == -1.f) ? Start : End);
}

void ASplineBase::SetScale(USplineMeshComponent* SplineMesh, FVector2D Start, FVector2D End)
{
	SplineMesh->SetStartScale(Start);
	SplineMesh->SetEndScale((End.Equals(FVector2D(-1.f, -1.f))) ? Start : End);
}

void ASplineBase::ClearLabels()
{
	TArray<UActorComponent*> Labels = GetComponentsByClass(UTextRenderComponent::StaticClass());
	for (auto Label : Labels)
		Label->DestroyComponent();
}

void ASplineBase::CreateLabels()
{
	for (int32 Index = 0; Index <= SplineComponent->GetNumberOfSplinePoints() - 1; Index++)
	{
		UTextRenderComponent* TextRender = AddTextRenderComponent(Index);
		TextRender->SetText(FText::FromString(FString::FromInt(Index)));
	}
}

int32 ASplineBase::GetNextIndex(int32 CurrentIndex)
{
	if ((bIsLooping) && (CurrentIndex + 1 == SplineComponent->GetNumberOfSplinePoints()))
		return 0;
	return CurrentIndex + 1;
}

int32 ASplineBase::GetOffset()
{
	return (bIsLooping) ? 1 : 2;
}
