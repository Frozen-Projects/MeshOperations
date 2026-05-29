#include "Widgets/Widget_Hierarchy_Item.h"
#include "Widgets/Widget_Hierarchy.h"

void UWidget_Hierarchy_Item::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UWidget_Hierarchy_Item::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (this->Hierarchy_Generator())
	{
		this->Button_Comp->OnClicked.AddDynamic(this, &UWidget_Hierarchy_Item::On_Select);
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : failed"), TEXT(__FUNCTION__));
	}
}

void UWidget_Hierarchy_Item::NativeDestruct()
{
	Super::NativeDestruct();
}

void UWidget_Hierarchy_Item::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

TSharedRef<SWidget> UWidget_Hierarchy_Item::RebuildWidget()
{
	return Super::RebuildWidget();
}

bool UWidget_Hierarchy_Item::Hierarchy_Generator()
{
	if (!IsValid(this->Target))
	{
		return false;
	}

	FString ObjectName;
	if (!UMeshOperationsBPLibrary::GetObjectNameForPackage(ObjectName, this->Target, false) || ObjectName.IsEmpty())
	{
		ObjectName = TEXT("NULL");
	}

	this->Title_Comp->SetText(FText::FromString(ObjectName));

	if (!this->Target->ComponentTags.IsEmpty())
	{
		for (const FName Each_Metadata : this->Target->ComponentTags)
		{
			UWidget_Hierarchy_Metadata* Each_Meta_Widget = CreateWidget<UWidget_Hierarchy_Metadata>(this, this->Hierarchy_Metadata_Class);

			if (IsValid(Each_Meta_Widget))
			{
				const FString Metadata_String = Each_Metadata.ToString();
				Each_Meta_Widget->Title_Metadata->SetText(FText::FromString(Metadata_String));
				this->Metadata_List->AddChild(Each_Meta_Widget);
				Each_Meta_Widget->SetPadding(FMargin(75.f, 5.f, 5.f, 5.f));
			}
		}
	}
	
	if (IsValid(this->Main_Parent))
	{
		FHierarchy_Item_Struct Hierarchy_Item_Struct;
		Hierarchy_Item_Struct.Name = ObjectName.ToLower();
		Hierarchy_Item_Struct.Widget = this;
		this->Main_Parent->Hierarchy_Items.Add(MoveTemp(Hierarchy_Item_Struct));
	}

	TArray<USceneComponent*> Children_Components;
	this->Target->GetChildrenComponents(false, Children_Components);
	
	for (USceneComponent* Child_Comp : Children_Components)
	{
		UWidget_Hierarchy_Item* Child_Widget = CreateWidget<UWidget_Hierarchy_Item>(this, this->GetClass());

		if (IsValid(Child_Widget))
		{
			Child_Widget->Target = Child_Comp;
			Child_Widget->Main_Parent = this->Main_Parent;
			Child_Widget->Hierarchy_Metadata_Class = this->Hierarchy_Metadata_Class;
			this->Children->AddChild(Child_Widget);
			Child_Widget->SetPadding(FMargin(5.f));
		}
	}

	return true;
}

void UWidget_Hierarchy_Item::On_Select()
{
	if (!IsValid(this->Target))
	{
		return;
	}

	// Implement gizmo system.
}