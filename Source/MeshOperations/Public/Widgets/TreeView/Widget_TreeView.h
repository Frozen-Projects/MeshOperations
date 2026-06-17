#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"

#include "Widgets/TreeView/Widget_TreeView_Item.h"

#include "Widget_TreeView.generated.h"

UCLASS()
class MESHOPERATIONS_API UWidget_TreeView : public UUserWidget
{
	GENERATED_BODY()
	
private:

	TMap<USceneComponent*, TArray<USceneComponent*>> MatchingComponents;

	UPROPERTY()
	TMap<USceneComponent*, UTreeView_Data*> DataCache;

	UPROPERTY()
	int32 Current_Index = 0;

	UPROPERTY()
	int32 Max_Index = 0;

	UFUNCTION()
	UTreeView_Data* GetOrCreateData(USceneComponent* InComponent, int32 InDepth);

	UFUNCTION()
	virtual void HandleGetChildren(UObject* Item, TArray<UObject*>& OutChildren);

	UFUNCTION()
	virtual bool Hierarchy_Generator();
	
	UFUNCTION()
	virtual void ResetHighlights();

	UFUNCTION()
	virtual void On_Search_Committed(const FText& SearchText, ETextCommit::Type CommitMethod);

	UFUNCTION()
	virtual void On_Search_Next();

	UFUNCTION()
	void On_Search_Previous();

public:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* Canvas_Panel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* Search_Box = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTreeView* Hierarchy = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Title_Index = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Search_Next = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Search_Previous = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	USceneComponent* Root = nullptr;

};
