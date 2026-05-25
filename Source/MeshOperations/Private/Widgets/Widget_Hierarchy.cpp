#include "Widgets/Widget_Hierarchy.h"

void UWidget_Hierarchy::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UWidget_Hierarchy::NativeConstruct()
{
	Super::NativeConstruct();

	if (!this->Hierarchy_Generator())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : failed"), TEXT(__FUNCTION__));
	}
}

void UWidget_Hierarchy::NativeDestruct()
{
	Super::NativeDestruct();
}

void UWidget_Hierarchy::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

TSharedRef<SWidget> UWidget_Hierarchy::RebuildWidget()
{
	return Super::RebuildWidget();
}

bool UWidget_Hierarchy::Hierarchy_Generator()
{
	if (!IsValid(this->Root))
	{
		return false;
	}

	TArray<USceneComponent*> Children_Components;
	this->Root->GetChildrenComponents(false, Children_Components);
	
	for (USceneComponent* Child_Comp : Children_Components)
	{
		this->Root_Item_Widget = CreateWidget<UWidget_Hierarchy_Item>(this, this->Hierarchy_Item_Class);
		this->Root_Item_Widget->Target = Child_Comp;
		this->Hierarchy->AddChild(this->Root_Item_Widget);
	}

	return true;
}