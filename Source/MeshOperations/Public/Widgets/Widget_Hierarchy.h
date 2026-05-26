// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Runtime/UMG/Public/UMG.h"

#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"

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
	virtual TArray<UWidget_Hierarchy_Item*> Find_Widgets(const FString& In_Name);

	UFUNCTION()
	void On_Search_Commited(const FText& In_Text, ETextCommit::Type In_Commit_Type);

	UFUNCTION()
	void On_Search_Next();

	UFUNCTION()
	void On_Search_Previous();

public:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(BlueprintReadOnly)
	UWidget_Hierarchy_Item* Root_Item_Widget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hierarchy", meta = (ExposeOnSpawn = true))
	TSubclassOf<UWidget_Hierarchy_Item> Hierarchy_Item_Class;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* Canvas_Panel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* Hierarchy = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* Search_Box = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Current_Index_Text = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Max_Index_Text = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Search_Next = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Search_Previous = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	USceneComponent* Root = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<FHierarchy_Item_Struct> Hierarchy_Items;

};
