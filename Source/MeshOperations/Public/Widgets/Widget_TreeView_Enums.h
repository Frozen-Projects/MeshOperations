#pragma once

#include "CoreMinimal.h"
#include "Widget_TreeView_Enums.generated.h"

UENUM(BlueprintType)
enum class EHierarchyNames : uint8
{
	None = 0		UMETA(DisplayName = "None"),
	Object = 1		UMETA(DisplayName = "Object"),
	Product = 2		UMETA(DisplayName = "Product"),
	Instance = 3	UMETA(DisplayName = "Instance"),
};
ENUM_CLASS_FLAGS(EHierarchyNames)