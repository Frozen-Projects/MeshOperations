#pragma once

#include "CoreMinimal.h"

#include "Widgets/Widget_TreeView_Enums.h"

#include "Widget_TreeView_Data.generated.h"

UCLASS(BlueprintType)
class MESHOPERATIONS_API UTreeView_Data : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	USceneComponent* Target_Component;

	UPROPERTY(BlueprintReadWrite)
	int32 Padding_Depth = 0;

	UPROPERTY(BlueprintReadWrite)
	bool bIsHighlighted = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCurrentHighlight = false;

	UPROPERTY(BlueprintReadWrite)
	EHierarchyNames NameType = EHierarchyNames::Product;

};