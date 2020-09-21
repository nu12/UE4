# Unreal Engine Notes

## Definitions

* Actor: anything that can be placed in a scene.

## Standards

* Camel Case for variable names
* Prefix bool variables with `b`


## Data Types

| C++ | UE  |
|-----|-----|
|std::string| FString|
| int | int32 |


## Loading an array of strings from a text file

```cpp
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
// ...
const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordList/HiddenWordList.txt");
FFileHelper::LoadFileToStringArray(TArrayOfFStringsVariable, *WordListPath);
```

Note: Include the path to the file in Additional Non-Asset Directories to Package option.

## Random

Generating random numbers, see `FMath::RandRange()`.

## TEXT

Working with string literals, use the `TEXT` macro to ensure cross compatibility.

```cpp
TEXT("Enter string literal here.")
```

## Out paramenters (a.k.a. by reference)

Using a function to modify the value of variables out of its scope can be an useful way to "return" multiple values from it.

This is possible passing the variables by reference.

```cpp
int32 VarA, VarB;
void ClassName::FunctionName(int32& ValueA, int32& ValueB){ /* ... */ };
```

## Structs

An alternative form the above is creating your own data type (struct) to be returned from the function.

```cpp
struct FMyStruct{
    int32 VarA, VarB;
};

FMyStruc ClassName::FunctionName(){ /* ... */ };

```

## UE_LOG

```cpp
UE_LOG(LogTemp, Error|Warning|Display, TEXT("Text!"));
```

## FVector & FTransform

FVector inclides X, Y and Z regarding position/location/transform information.

FTransform includes position, rotation and scale of the object.

## Importing assets

Always import using the editor (no copy and paste inside COntent folder directly).

Content color scheme:
* Blue:
* Light Green: material base, changes affect material instances. Create editing parameters.
* Dark Green: material instance, doesn't affect other instances. Edit parameters.
* Red: texture (image).

## Exposing Parameter to the editor

Using the macro `UPROPERTY(EditAnywhere)` a variable will be available to be manipulated in the editor.

## Change game mode and default pawn

### Create Blueprint Pawn

* Start the game
* Eject
* Select the default Pawn
* Select Blueprints > Convert Selection to Blueprint Class..

### Create Blueprint Game Mode

* Go to C++ Classes > GameName folder in the Content list
* Right click and select Create Blueprint class based on ...

### Project settings

Change Default GameMode and Default Pawn Class in Project Settings > Maps & Modes

## DrawDebugLine

Example of debug line from Player View Point, 1 meter in length.

```cpp
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

/* ... */
FVector PlayerViewPointLocation;
FRotator PlayerViewPointRotation;

// Using out parameters
GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

FVector LineEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * 100.f; // 100.f == 1 meter

DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineEnd, FColor::Red, false, 1.f, 1, 10.f);
```

## Grabbing objects in the scenario

```cpp
// Set Components
void UGrabberComponent::SetupComponents() 
{
	PhysicsHandler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(!PhysicsHandler) UE_LOG(LogTemp, Error, TEXT("Physics handler component not found!"));

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent) UE_LOG(LogTemp, Error, TEXT("Input component not found!"));
}

// Bind inputs
void UGrabberComponent::SetupInputBinding() const 
{
	InputComponent->BindAction(
		FName(TEXT("GrabInput")),	// This is the same name assigned to the input in Project Settings > Input
		IE_Pressed,					// Type of input: pressed, release, etc
		this,						// References self
		&UGrabberComponent::Grab	// Method to be executed
	);

	InputComponent->BindAction(
		FName(TEXT("GrabInput")),
		IE_Released,
		this,
		&UGrabberComponent::Release
	);
}

// Create Grab and Release methods mentioned above
void UGrabberComponent::Grab() 
{
	UE_LOG(LogTemp, Warning, TEXT("Grab!"));
	FHitResult HitResult = GetObjectInReach();

	if (!HitResult.GetActor()) return;

	PhysicsHandler->GrabComponentAtLocation( // Grab method
		HitResult.GetComponent(),
		NAME_None,
		HitResult.GetActor()->GetActorLocation()
	);
}

void UGrabberComponent::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release!"));
	PhysicsHandler->ReleaseComponent();
}

// LineTrace to find object in reach (used in Grab method)
FHitResult UGrabberComponent::GetObjectInReach() const {
	
	FVector PlayerViewPointLocation; // Out parameter
	FRotator PlayerViewPointRotation;// Out parameter
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	FVector LineEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach; // Same as draw debug line

	FHitResult Hit; // Out parameter
	GetWorld()->LineTraceSingleByChannel(
		Hit,
		PlayerViewPointLocation,
		LineEnd,
		ECollisionChannel::ECC_PhysicsBody, // PhysicsBody
		FCollisionQueryParams(
			FName(TEXT("")),	// This is not using Tag
			false,				// No complex collision (visibility vs collision)
			GetOwner()			// Ignore self as the line tracing begins inside the pawn
		)
	);
	return Hit;
}

// Update the position of the grabbed component every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandler->GetGrabbedComponent()) {
		FVector PlayerViewPointLocation; // Out parameter
        FRotator PlayerViewPointRotation;// Out parameter
        GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
        FVector LineEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach; // Same as draw debug line
		PhysicsHandler->SetTargetLocation(LineEnd);
	}
}
```
Note: PhysicsHandler needs to be added in Actor menu. InputComponent is set by default.

## Event binding & AddDynamic (event callback)

Bind an event (such as OnTakeAnyDamage or OnComponentHit) to a method. Notice that the callback (bind) method needs to have the same signature as the Dynamic Delegate it is representing.

Example: 
```cpp
// For GetOwner()->OnTakeAnyDamage, this is the delegate macro:
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_FiveParams( FTakeAnyDamageSignature, AActor, OnTakeAnyDamage, AActor*, DamagedActor, float, Damage, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser );

// Notice it has five arguments after 'OnTakeAnyDamage', separeted by comas.
// The delegate function should have the folloowing signature:
void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

// And the binding to make it work:
GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);

// With this, every time the game fires OnTakeAnyDamage event, the method above is executed.
```

Another example:
```cpp
// The delegate macro for reference
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_FiveParams( FComponentHitSignature, UPrimitiveComponent, OnComponentHit, UPrimitiveComponent*, HitComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, FVector, NormalImpulse, const FHitResult&, Hit );

/* ... */

/* BeginPlay */
StaticMeshComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

/* ... */

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	/* ... */
}

```

## UGameplayStatics

UGameplayStatics is a very useful class to get objects related to the player, controller, world, etc. Useful methods include:

* GetPlayerPawn
* GetPlayerController
* GetPlayerCameraManager
* GetGameMode
* PlaySoundAtLocation: play sound
* SpawnEmitterAtLocation: spawn particle

## Create and call Blueprint event C++

Using a `UFUNCTION(BlueprintImplementableEvent)` in the header file will create an event with the method name to be called in the Blueprint editor. Calling the method in C++ fires the event in the Blueprint.

Example:
```cpp
// Header file
UFUNCTION(BlueprintImplementableEvent)
void GameStart();

// Implementation
void SomeOtherFunction()
{
	GameStart(); // An GameStart event will be fired in Blueprint
}

```
Note: the `GameStart()` doesn't need to be implemented in C++. The IDE may complain, but it compiles with no problem.

## Make a C++ method callable in Blueprint

Add `UFUNCTION(BlueprintCallable)` macro before the method signature in the header file.

## SetTimerHandle

Useful for delaying the execution of a method, or to execute it repeteadly in a specified interval. 

Simple delegate method:

```cpp
FTimerHandle FireRateTimerHandle;
GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);

void APawnTurret::CheckFireCondition()

```

Delegate method with argument:

```cpp
FTimerHandle EnableControllerTimerHandle;
FTimerDelegate EnableControllerDelegate = FTimerDelegate::CreateUObject(PlayerController, &APlayerControllerBase::SetControllerEnabled, true);
GetWorld()->GetTimerManager().SetTimer(
	EnableControllerTimerHandle,
	EnableControllerDelegate,
	StartDelay,
	false
);

void APlayerControllerBase::SetControllerEnabled(bool ControllerEnabled)
```

## Pawn movement

* Axis: APawn::AddActorLocalOffset(FVector MoveDirection)

* Axis: APawn::AddActorLocalRotation(FQuat/FRotator Rotation)

* Axis: APawn::AddMovementInput(FVector MoveDirection, float Value)

* Axis: APawn::AddControllerPitchInput(float Value)

* Axis: APawn::AddControllerYawInput(float Value)

* Action: ACharacter::Jump

Note: See Input_Example.


### Useful
* FVector AActor::GetActorForwardVector();
* FVector AActor::GetActorRightVector()