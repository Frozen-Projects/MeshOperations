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

	UPROPERTY()
	bool bIsExpanded = false;

};

UCLASS()
class MESHOPERATIONS_API UWidget_TreeView_Item : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

private:

	UFUNCTION()
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION()
	virtual void On_Expand_Children();

public:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* Main_Canvas = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* Content_Box = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Button_Expand = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TITLE_Product = nullptr;

	UPROPERTY(BlueprintReadWrite)
	double IndentationPerLevel = 32.0;

	UPROPERTY(BlueprintReadWrite)
	FSlateColor HighlightedColor = FSlateColor(FLinearColor::Yellow);
	
	UPROPERTY(BlueprintReadWrite)
	FSlateColor DefaultColor = FSlateColor(FLinearColor::Black);

};