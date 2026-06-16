#include "Widgets/TreeView/Widget_TreeView_Item.h"
#include "Widgets/TreeView/Widget_TreeView.h"
#include "MeshOperationsBPLibrary.h"

void UWidget_TreeView_Item::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UWidget_TreeView_Item::NativeConstruct()
{
	Super::NativeConstruct();

	this->Button_Expand->OnClicked.AddDynamic(this, &UWidget_TreeView_Item::On_Expand_Children);
}

void UWidget_TreeView_Item::NativeDestruct()
{
	Super::NativeDestruct();
}

void UWidget_TreeView_Item::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

TSharedRef<SWidget> UWidget_TreeView_Item::RebuildWidget()
{
	return Super::RebuildWidget();
}

void UWidget_TreeView_Item::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UTreeView_Data* TreeView_Data = Cast<UTreeView_Data>(ListItemObject);

	if (!IsValid(TreeView_Data) || !IsValid(TreeView_Data->Target_Component))
	{
		return;
	}

	this->TITLE_Product->SetText(TreeView_Data->Target_Component->ComponentTags.IsEmpty() ? FText::FromString("Unnamed Product") : FText::FromName(TreeView_Data->Target_Component->ComponentTags[0]));
	this->TITLE_Product->SetColorAndOpacity(TreeView_Data->bIsHighlighted ? this->HighlightedColor : this->DefaultColor);

	if (!IsValid(this->Button_Expand))
	{
		return;
	}

	// Actual widget item is not moving, but we are adjusting the padding of the button that is located in the "horizontal box" to give the illusion of movement.
	if (UHorizontalBoxSlot* ButtonSlot = Cast<UHorizontalBoxSlot>(this->Button_Expand->Slot))
	{
		FMargin NewPadding = ButtonSlot->GetPadding();
		NewPadding.Left = TreeView_Data->Padding_Depth * this->IndentationPerLevel;
		ButtonSlot->SetPadding(NewPadding);
	}

	this->UpdateExpansionVisuals(IsListItemExpanded());
}

void UWidget_TreeView_Item::NativeOnItemExpansionChanged(bool bIsExpanded)
{
	IUserObjectListEntry::NativeOnItemExpansionChanged(bIsExpanded);
	this->UpdateExpansionVisuals(bIsExpanded);
}

void UWidget_TreeView_Item::On_Expand_Children()
{
	UTreeView* OwningTreeView = Cast<UTreeView>(GetOwningListView());

	if (!IsValid(OwningTreeView))
	{
		return;
	}

	UTreeView_Data* TreeView_Data = Cast<UTreeView_Data>(this->GetListItem());

	if (!IsValid(TreeView_Data))
	{
		return;
	}

	OwningTreeView->SetItemExpansion(TreeView_Data, !IsListItemExpanded());
}

void UWidget_TreeView_Item::UpdateExpansionVisuals(bool bIsExpanded)
{
	if (!IsValid(this->Button_Expand))
	{
		return;
	}

	FButtonStyle ButtonStyle = Button_Expand->GetStyle();

	if (IsValid(this->ExpandedMaterial) && IsValid(this->CollapsedMaterial))
	{
		UMaterialInterface* NormalImg = bIsExpanded ? this->ExpandedMaterial : this->CollapsedMaterial;
		UMaterialInterface* PressedImg = bIsExpanded ? this->CollapsedMaterial : this->ExpandedMaterial;

		ButtonStyle.Normal.SetResourceObject(NormalImg);
		ButtonStyle.Hovered.SetResourceObject(NormalImg);
		ButtonStyle.Pressed.SetResourceObject(PressedImg);
	}

	ButtonStyle.Hovered.TintColor = HoverColor;

	Button_Expand->SetStyle(ButtonStyle);
}
