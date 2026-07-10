#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"

#include "Widgets/Widget_TreeView_Item.h"

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
	TArray<USceneComponent*> AssemblyRoots;

	UPROPERTY()
	int32 Current_Index = 0;

	UPROPERTY()
	int32 Max_Index = 0;

	static EHierarchyNames GetEnumValueByName(const FString& InName);

	UFUNCTION()
	static FString GetEnumDisplayName(EHierarchyNames In_Enum);

	UFUNCTION()
	UTreeView_Data* GetOrCreateData(USceneComponent* InComponent, int32 InDepth);

	UFUNCTION()
	virtual void HandleGetChildren(UObject* Item, TArray<UObject*>& OutChildren);
	
	UFUNCTION()
	virtual void ClearHighlights();

	UFUNCTION()
	virtual void SwitchHiglights();

	UFUNCTION()
	virtual void RefreshDisplayedTitles();

	UFUNCTION()
	virtual void ClearSearchResults();

	UFUNCTION()
	virtual void On_Search_Committed(const FText& SearchText, ETextCommit::Type CommitMethod);

	UFUNCTION()
	virtual void On_Search_Next();

	UFUNCTION()
	virtual void On_Search_Previous();

	UFUNCTION()
	virtual void On_Search_Type_Changed(FString SelectedItem, ESelectInfo::Type SelectionType);

public:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UFUNCTION(BlueprintCallable)
	virtual bool AddItemToHierarchy(USceneComponent* InComponent);

	UFUNCTION(BlueprintCallable)
	virtual bool Hierarchy_Generator(TArray<USceneComponent*> InComponents);

	UFUNCTION(BlueprintPure)
	virtual bool IsHierarchyEmpty() const;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* Canvas_Panel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* Search_Area = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* Search_Type = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* Search_Box = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Title_Index = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Search_Next = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Search_Previous = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTreeView* Hierarchy = nullptr;
	
};
