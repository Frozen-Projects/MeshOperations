#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"

#include "Widgets/Widget_TreeView_Includes.h"
#include "Widgets/Widget_TreeView_Data.h"

#include "Widget_TreeView_Item.generated.h"

// Forward Declarations.
class UWidget_TreeView;

UCLASS(Abstract, meta = (DisableNativeTick))
class MESHOPERATIONS_API UWidget_TreeView_Item : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

private:

	UFUNCTION()
	virtual void UpdateExpansionVisuals(bool bIsExpanded);
	
	UFUNCTION()
	virtual void ApplyHighlightColor_Internal(UTreeView_Data* TreeView_Data);

	UFUNCTION()
	virtual void UpdateTitle_Internal(UTreeView_Data* TreeView_Data);

	UFUNCTION()
	virtual void On_Expand_Children();

	UFUNCTION()
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION()
	virtual void NativeOnItemExpansionChanged(bool bIsExpanded) override;

public:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UFUNCTION()
	virtual void ApplyHighlightColor();

	UFUNCTION()
	virtual void UpdateTitle();

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