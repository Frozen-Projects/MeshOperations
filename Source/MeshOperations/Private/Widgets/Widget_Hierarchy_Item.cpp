#include "Widgets/Widget_Hierarchy_Item.h"
#include "Widgets/Widget_Hierarchy.h"

void UWidget_Hierarchy_Item::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UWidget_Hierarchy_Item::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!this->Hierarchy_Generator())
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
			this->Children->AddChild(Child_Widget);
			Child_Widget->SetPadding(FMargin(5.f));
		}
	}

	return true;
}