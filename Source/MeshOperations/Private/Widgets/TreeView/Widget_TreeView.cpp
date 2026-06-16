#include "Widgets/TreeView/Widget_TreeView.h"

void UWidget_TreeView::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UWidget_TreeView::NativeConstruct()
{
	Super::NativeConstruct();

	if (this->Hierarchy)
	{
		this->DataCache.Empty();
		this->MatchingComponents.Empty();

		this->Hierarchy->SetOnGetItemChildren(this, &UWidget_TreeView::HandleGetChildren);
		this->Hierarchy_Generator();

		this->Search_Box->OnTextCommitted.AddDynamic(this, &UWidget_TreeView::On_Search_Committed);
	}
}

void UWidget_TreeView::NativeDestruct()
{
	Super::NativeDestruct();
}

void UWidget_TreeView::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

TSharedRef<SWidget> UWidget_TreeView::RebuildWidget()
{
	return Super::RebuildWidget();
}

UTreeView_Data* UWidget_TreeView::GetOrCreateData(USceneComponent* InComponent, int32 InDepth)
{
	if (!IsValid(InComponent))
	{
		return nullptr;
	}

	if (UTreeView_Data** FoundData = DataCache.Find(InComponent))
	{
		return *FoundData;
	}

	UTreeView_Data* NewData = NewObject<UTreeView_Data>(this);
	NewData->Target_Component = InComponent;
	NewData->Padding_Depth = InDepth;

	DataCache.Add(InComponent, NewData);
	return NewData;
}

void UWidget_TreeView::HandleGetChildren(UObject* Item, TArray<UObject*>& OutChildren)
{
	UTreeView_Data* ParentData = Cast<UTreeView_Data>(Item);

	if (!IsValid(ParentData) || !IsValid(ParentData->Target_Component))
	{
		return;
	}

	TArray<USceneComponent*> Children;
	ParentData->Target_Component->GetChildrenComponents(false, Children);

	for (USceneComponent* Child : Children)
	{
		if (UTreeView_Data* ChildData = this->GetOrCreateData(Child, ParentData->Padding_Depth + 1))
		{
			OutChildren.Add(ChildData);
		}
	}
}

bool UWidget_TreeView::Hierarchy_Generator()
{
	if (!IsValid(Root) || !IsValid(this->Hierarchy))
	{
		return false;
	}

	this->Hierarchy->ClearListItems();

	UTreeView_Data* RootData = this->GetOrCreateData(Root, 0);
	this->Hierarchy->AddItem(RootData);
	
	return true;
}

void UWidget_TreeView::ResetHighlights()
{
	TArray<UTreeView_Data*> Data_Items;
	this->DataCache.GenerateValueArray(Data_Items);

	for (UTreeView_Data* Each_Data : Data_Items)
	{
		Each_Data->bIsHighlighted = false;
	}

	this->Hierarchy->RequestRefresh();
}

void UWidget_TreeView::On_Search_Committed(const FText& SearchText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod != ETextCommit::OnEnter)
	{
		return;
	}

	if (SearchText.IsEmpty() || !IsValid(this->Root))
	{
		this->MatchingComponents.Empty();
		this->ResetHighlights();
		return;
	}

	TArray<USceneComponent*> AllComponents;
	this->Root->GetChildrenComponents(true, AllComponents);

	if (AllComponents.IsEmpty())
	{
		this->MatchingComponents.Empty();
		this->ResetHighlights();
		return;
	}
	
	this->ResetHighlights();

	for (USceneComponent* Component : AllComponents)
	{
		if (Component->ComponentTags.Num() > 0 && Component->ComponentTags[0].ToString().Contains(SearchText.ToString()))
		{
			TArray<USceneComponent*> Temp_Parents;
			Component->GetParentComponents(Temp_Parents);
			this->MatchingComponents.Add(Component, Temp_Parents);
		}
	}

	bool bIsFirstReceived = false;

	for (TPair<USceneComponent*, TArray<USceneComponent*>> Pair : this->MatchingComponents)
	{
		Algo::Reverse(Pair.Value);

		for (int32 Parent_Index = 0; Parent_Index < Pair.Value.Num(); Parent_Index++)
		{
			// +1 Because of root.
			UTreeView_Data* Parent_Data = this->GetOrCreateData(Pair.Value[Parent_Index], Parent_Index + 1);
			Parent_Data->bIsExpanded = true;
			this->Hierarchy->SetItemExpansion(Parent_Data, Parent_Data->bIsExpanded);
		}

		UTreeView_Data* Each_Matched = this->GetOrCreateData(Pair.Key, Pair.Value.Num());
		Each_Matched->bIsHighlighted = true;

		if (!bIsFirstReceived)
		{
			this->Hierarchy->RequestScrollItemIntoView(Each_Matched);
			bIsFirstReceived = true;
		}
	}

	this->Hierarchy->RequestRefresh();	
}