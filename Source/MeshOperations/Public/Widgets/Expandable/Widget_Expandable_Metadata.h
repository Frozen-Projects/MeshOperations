#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"

#include "Widgets/Expandable/Widget_Expandable_Includes.h"

#include "Widget_Expandable_Metadata.generated.h"

UCLASS()
class MESHOPERATIONS_API UWidget_Expandable_Metadata : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* Main_Canvas = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Button_Metadata = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Title_Metadata = nullptr;
};