// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "HAL/PlatformApplicationMisc.h"
#include "Kismet/GameplayStatics.h"

#include "Runtime/UMG/Public/UMG.h"

#include "Components/TextBlock.h"
#include "Components/ExpandableArea.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "Components/RetainerBox.h"
#include "Components/EditableTextBox.h"
#include "Components/Overlay.h"
#include "Components/ComboboxString.h"

#include "MeshOperationsBPLibrary.h"

#include "Widget_Hierarchy_Item.generated.h"

/*
* Widget Order
* CanvasPanel > ExpandableArea > 
	Header > Overlay > Button (Left Align Horizontally + Fill Vertically) > Title (Left Align Horizontally + Center Align Vertically)
	Body > RetainerBox > VerticalBox > Children
*/
UCLASS()
class MESHOPERATIONS_API UWidget_Hierarchy_Item : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UFUNCTION()
	virtual bool Hierarchy_Generator();

public:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* Canvas_Panel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UExpandableArea* Expandable_Area = nullptr;

	/*
	* We use this to align button based on title's width.
	*/
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* Align_Overlay = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Button_Comp = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Title_Comp = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* Children = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	URetainerBox* Retainer_Box = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	USceneComponent* Target = nullptr;
};
