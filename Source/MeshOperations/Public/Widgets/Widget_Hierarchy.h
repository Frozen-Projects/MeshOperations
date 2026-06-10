// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"

#include "Components/Border.h"
#include "Components/ScrollBox.h"
#include "Components/HorizontalBox.h"
#include "Components/EditableTextBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ComboBoxString.h"

#include "Widgets/Widget_Hierarchy_Item.h"

#include "Widget_Hierarchy.generated.h"

UCLASS()
class MESHOPERATIONS_API UWidget_Hierarchy : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UPROPERTY()
	TArray<UWidget_Hierarchy_Item*> Found_Widgets;

	UPROPERTY()
	int32 Current_Index = 0;

	UPROPERTY()
	int32 Max_Index = 0;

	UFUNCTION()
	virtual bool Hierarchy_Generator();
	
	UFUNCTION()
	virtual const FString Get_Search_Type(FHierarchy_Item_Struct In_Item) const;

	UFUNCTION()
	virtual TArray<UWidget_Hierarchy_Item*> Find_Widgets(const FString& In_Name);

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
	UWidget_Hierarchy_Item* Root_Item_Widget = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<FHierarchy_Item_Struct> Hierarchy_Items;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	USceneComponent* Root = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hierarchy", meta = (ExposeOnSpawn = true))
	TSubclassOf<UWidget_Hierarchy_Item> Hierarchy_Item_Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hierarchy", meta = (ExposeOnSpawn = true))
	TSubclassOf<UWidget_Hierarchy_Metadata> Hierarchy_Metadata_Class;

};
