#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"

#include "Widgets/Expandable/Widget_Expandable_Metadata.h"

#include "Widget_Expandable_Item.generated.h"

// Forward Declarations.
class UWidget_Expandable;

/*
* Widget Order
* Main_Canvas > ExpandableArea > 
	Header > Header_Canvas > Button (Size to Content) > Title (Left Align Horizontally + Center Align Vertically)
	Body > Size_Box > Retainer_Box > Children
*/
UCLASS()
class MESHOPERATIONS_API UWidget_Expandable_Item : public UUserWidget
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
	UWidget_Expandable* Main_Parent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hierarchy", meta = (ExposeOnSpawn = true))
	TSubclassOf<UWidget_Expandable_Metadata> Hierarchy_Metadata_Class;

};

USTRUCT(BlueprintType)
struct FExpandableItemStruct
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FString Object_Name;

	UPROPERTY(BlueprintReadWrite)
	FString Product_Name;

	UPROPERTY(BlueprintReadWrite)
	FString Instance_Name;

	UPROPERTY(BlueprintReadWrite)
	UWidget_Expandable_Item* Widget = nullptr;

};