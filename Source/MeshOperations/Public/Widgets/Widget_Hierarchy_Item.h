// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"

#include "HAL/PlatformApplicationMisc.h"
#include "Kismet/GameplayStatics.h"

#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/RetainerBox.h"
#include "Components/ExpandableArea.h"

#include "MeshOperationsBPLibrary.h"
#include "Widgets/Widget_Hierarchy_Metadata.h"

#include "Widget_Hierarchy_Item.generated.h"

// Forward Declarations.
class UWidget_Hierarchy;

/*
* Widget Order
* Main_Canvas > ExpandableArea > 
	Header > Header_Canvas > Button (Size to Content) > Title (Left Align Horizontally + Center Align Vertically)
	Body > Size_Box > Retainer_Box > Children
*/
UCLASS()
class MESHOPERATIONS_API UWidget_Hierarchy_Item : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UFUNCTION()
	virtual bool Hierarchy_Generator();

	UFUNCTION()
	virtual void On_Select();

public:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* Main_Canvas = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UExpandableArea* Main_Expandable = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* Header_Canvas = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UExpandableArea* Metadata_Expandable = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* Metadata_List = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Button_Comp = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Title_Comp = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* Children = nullptr;

	/*
	* We use this to prevent "The requested size for SRetainerWidget is 0." error. Min desired height and width should be greater than 0.
	*/
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USizeBox* Size_Box = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	URetainerBox* Retainer_Box = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	USceneComponent* Target = nullptr;

	UPROPERTY(BlueprintReadWrite)
	UWidget_Hierarchy* Main_Parent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hierarchy", meta = (ExposeOnSpawn = true))
	TSubclassOf<UWidget_Hierarchy_Metadata> Hierarchy_Metadata_Class;

};

USTRUCT(BlueprintType)
struct FHierarchy_Item_Struct
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FString Name;

	UPROPERTY(BlueprintReadWrite)
	UWidget_Hierarchy_Item* Widget = nullptr;

};