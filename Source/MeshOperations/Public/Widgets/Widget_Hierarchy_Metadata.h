#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Widget_Hierarchy_Metadata.generated.h"

UCLASS()
class MESHOPERATIONS_API UWidget_Hierarchy_Metadata : public UUserWidget
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