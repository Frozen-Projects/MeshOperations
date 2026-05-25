#include "Widgets/Widget_Hierarchy_Item.h"

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

	TArray<USceneComponent*> Children_Components;
	this->Target->GetChildrenComponents(false, Children_Components);
	
	for (USceneComponent* Child_Comp : Children_Components)
	{
		UWidget_Hierarchy_Item* Child_Widget = CreateWidget<UWidget_Hierarchy_Item>(this, this->GetClass());

		if (IsValid(Child_Widget))
		{
			Child_Widget->Target = Child_Comp;
			this->Children->AddChild(Child_Widget);
		}

		else
		{
			continue;
		}
	}

	return true;
}