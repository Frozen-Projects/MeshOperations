#pragma once

#include "CoreMinimal.h"
#include "MeshOps_Enums.generated.h"

UENUM(BlueprintType)
enum class ETextureParameterType : uint8
{
	Scalar		UMETA(DisplayName = "Scalar"),
	Vectoral	UMETA(DisplayName = "Vectoral"),
	Texture		UMETA(DisplayName = "Texture"),
};
ENUM_CLASS_FLAGS(ETextureParameterType)