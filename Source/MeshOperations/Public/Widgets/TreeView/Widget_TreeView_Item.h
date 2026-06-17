#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"

#include "Widgets/TreeView/Widget_TreeView_Includes.h"

#include "Widget_TreeView_Item.generated.h"

// Forward Declarations.
class UWidget_TreeView;

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

};

UCLASS()
class MESHOPERATIONS_API UWidget_TreeView_Item : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

private:

	UFUNCTION()
	virtual void ApplyHighlightColor_Internal(UTreeView_Data* TreeView_Data);

	UFUNCTION()
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION()
	virtual void NativeOnItemExpansionChanged(bool bIsExpanded) override;

	UFUNCTION()
	virtual void On_Expand_Children();

	UFUNCTION()
	virtual void UpdateExpansionVisuals(bool bIsExpanded);

public:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void ApplyHighlightColor();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* Main_Canvas = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* Content_Box = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Button_Expand = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Button_Select = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Title = nullptr;

	UPROPERTY(BlueprintReadWrite)
	double IndentationPerLevel = 32.0;

	UPROPERTY(BlueprintReadWrite)
	FSlateColor Title_HighlightColor = FSlateColor(FLinearColor::Yellow);

	UPROPERTY(BlueprintReadWrite)
	FSlateColor Title_FirstHighlightColor = FSlateColor(FLinearColor::Blue);
	
	UPROPERTY(BlueprintReadWrite)
	FSlateColor Title_DefaultColor = FSlateColor(FLinearColor::Black);

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	UMaterialInterface* Button_ExpandedMaterial = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	UMaterialInterface* Button_CollapsedMaterial = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FSlateColor Button_HoverColor = FSlateColor(FLinearColor::Green);
};