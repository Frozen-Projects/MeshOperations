#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"

#include "Widgets/Expandable/Widget_Expandable_Item.h"

#include "Widget_Expandable.generated.h"

UCLASS()
class MESHOPERATIONS_API UWidget_Expandable : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UPROPERTY()
	TArray<UWidget_Expandable_Item*> Found_Widgets;

	UPROPERTY()
	int32 Current_Index = 0;

	UPROPERTY()
	int32 Max_Index = 0;

	UFUNCTION()
	virtual bool Hierarchy_Generator();
	
	UFUNCTION()
	virtual const FString Get_Search_Type(FExpandableItemStruct In_Item) const;

	UFUNCTION()
	virtual TArray<UWidget_Expandable_Item*> Find_Widgets(const FString& In_Name);

	UFUNCTION()
	void On_Search_Committed(const FText& In_Text, ETextCommit::Type In_Commit_Type);

	UFUNCTION()
	void On_Search_Next();

	UFUNCTION()
	void On_Search_Previous();

	UFUNCTION()
	bool Toggle_Frame(UCanvasPanel* In_Canvas, bool bDisable);

public:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* Canvas_Panel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* Hierarchy = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* Search_Area = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* Search_Type = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* Search_Box = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Index_Text = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Search_Next = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Search_Previous = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* Item_Frame = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UWidget_Expandable_Item* Root_Item_Widget = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<FExpandableItemStruct> Hierarchy_Items;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	USceneComponent* Root = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hierarchy", meta = (ExposeOnSpawn = true))
	TSubclassOf<UWidget_Expandable_Item> Hierarchy_Item_Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hierarchy", meta = (ExposeOnSpawn = true))
	TSubclassOf<UWidget_Expandable_Metadata> Hierarchy_Metadata_Class;

};
