#include "Widgets/TreeView/Widget_TreeView_Item.h"
#include "Widgets/TreeView/Widget_TreeView.h"

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

TSharedRef<SWidget> UWidget_TreeView_Item::RebuildWidget()
{
	return Super::RebuildWidget();
}

void UWidget_TreeView_Item::UpdateExpansionVisuals(bool bIsExpanded)
{
	if (!IsValid(this->Button_Expand))
	{
		return;
	}

	FButtonStyle ButtonStyle = Button_Expand->GetStyle();

	if (IsValid(this->Button_ExpandedMaterial) && IsValid(this->Button_CollapsedMaterial))
	{
		UMaterialInterface* NormalImg = bIsExpanded ? this->Button_ExpandedMaterial : this->Button_CollapsedMaterial;
		UMaterialInterface* PressedImg = bIsExpanded ? this->Button_CollapsedMaterial : this->Button_ExpandedMaterial;

		ButtonStyle.Normal.SetResourceObject(NormalImg);
		ButtonStyle.Hovered.SetResourceObject(NormalImg);
		ButtonStyle.Pressed.SetResourceObject(PressedImg);
	}

	ButtonStyle.Hovered.TintColor = this->Button_HoverColor;

	Button_Expand->SetStyle(ButtonStyle);
}

void UWidget_TreeView_Item::ApplyHighlightColor_Internal(UTreeView_Data* TreeView_Data)
{
	if (!IsValid(TreeView_Data) || !IsValid(this->Title))
	{
		return;
	}

	const FSlateColor NewColor = TreeView_Data->bIsHighlighted
		? (TreeView_Data->bIsCurrentHighlight ? this->Title_FirstHighlightColor : this->Title_HighlightColor)
		: this->Title_DefaultColor;

	this->Title->SetColorAndOpacity(NewColor);
}

void UWidget_TreeView_Item::UpdateTitle_Internal(UTreeView_Data* TreeView_Data)
{
	if (!IsValid(TreeView_Data) || !IsValid(TreeView_Data->Target_Component) || !IsValid(this->Title))
	{
		return;
	}

	switch (TreeView_Data->NameType)
	{
		case EHierarchyNames::Object:
		{
			const FString Object_Name = UMeshOperationsBPLibrary::GetObjectNameForPackage(TreeView_Data->Target_Component);
			this->Title->SetText(FText::FromString(Object_Name));
			break;
		}

		case EHierarchyNames::Product:
		{
			if (TreeView_Data->Target_Component->ComponentTags.IsEmpty())
			{
				this->Title->SetText(FText::FromString(UNNAMED_PRODUCT));
				break;
			}

			const FString FirstTag = TreeView_Data->Target_Component->ComponentTags[0].ToString();

			if (!FirstTag.Contains(FIELD_PRODUCT))
			{
				this->Title->SetText(FText::FromString(UNNAMED_PRODUCT));
				break;
			}

			this->Title->SetText(FText::FromString(FirstTag));
			break;
		}

		case EHierarchyNames::Instance:
		{
			if (TreeView_Data->Target_Component->ComponentTags.Num() < 2)
			{
				this->Title->SetText(FText::FromString(UNNAMED_INSTANCE));
				break;
			}

			const FString SecondTag = TreeView_Data->Target_Component->ComponentTags[1].ToString();

			if (!SecondTag.Contains(FIELD_INSTANCE))
			{
				this->Title->SetText(FText::FromString(UNNAMED_INSTANCE));
				break;
			}

			this->Title->SetText(FText::FromString(SecondTag));
			break;
		}

		default:
		{
			const FString Object_Name = UMeshOperationsBPLibrary::GetObjectNameForPackage(TreeView_Data->Target_Component);
			this->Title->SetText(FText::FromString(Object_Name));
			break;
		}
	}
}

void UWidget_TreeView_Item::ApplyHighlightColor()
{
	this->ApplyHighlightColor_Internal(Cast<UTreeView_Data>(this->GetListItem()));
}

void UWidget_TreeView_Item::UpdateTitle()
{
	this->UpdateTitle_Internal(Cast<UTreeView_Data>(this->GetListItem()));
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

void UWidget_TreeView_Item::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UTreeView_Data* TreeView_Data = Cast<UTreeView_Data>(ListItemObject);

	if (!IsValid(TreeView_Data) || !IsValid(TreeView_Data->Target_Component))
	{
		return;
	}

	this->UpdateTitle_Internal(TreeView_Data);

	this->ApplyHighlightColor_Internal(TreeView_Data);

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

	// We hide expand button after padding else, padding won't work. Also, we need to hide it instead collapse because we need its layout.

	const bool bHasChildren = TreeView_Data->Target_Component->GetNumChildrenComponents() > 0;
	const ESlateVisibility NewVisibility = bHasChildren ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	this->Button_Expand->SetVisibility(NewVisibility);
}

void UWidget_TreeView_Item::NativeOnItemExpansionChanged(bool bIsExpanded)
{
	IUserObjectListEntry::NativeOnItemExpansionChanged(bIsExpanded);
	this->UpdateExpansionVisuals(bIsExpanded);
}