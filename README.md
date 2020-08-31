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