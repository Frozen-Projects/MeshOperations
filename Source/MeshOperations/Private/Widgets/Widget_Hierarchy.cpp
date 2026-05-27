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

	this->Search_Box->OnTextCommitted.AddDynamic(this, &UWidget_Hierarchy::On_Search_Committed);
	this->Search_Next->OnClicked.AddDynamic(this, &UWidget_Hierarchy::On_Search_Next);
	this->Search_Previous->OnClicked.AddDynamic(this, &UWidget_Hierarchy::On_Search_Previous);}

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
		this->Root_Item_Widget->Main_Parent = this;
		this->Root_Item_Widget->Target = Child_Comp;
		this->Hierarchy->AddChild(this->Root_Item_Widget);
		this->Root_Item_Widget->SetPadding(FMargin(5.f));
	}

	return true;
}

TArray<UWidget_Hierarchy_Item*> UWidget_Hierarchy::Find_Widgets(const FString& In_Name)
{
	if (In_Name.IsEmpty())
	{
		return TArray<UWidget_Hierarchy_Item*>();
	}
	
	if (this->Hierarchy_Items.IsEmpty())
	{
		return TArray<UWidget_Hierarchy_Item*>();
	}

	const FString Name_Lower = In_Name.ToLower();

	TArray<UWidget_Hierarchy_Item*> Result;
	for (const FHierarchy_Item_Struct& Each_Item : this->Hierarchy_Items)
	{
		if (Each_Item.Name.Contains(Name_Lower, ESearchCase::CaseSensitive))
		{
			Result.Add(Each_Item.Widget);
		}

		else
		{
			continue;
		}
	}

	return Result;
}

void UWidget_Hierarchy::On_Search_Committed(const FText& In_Text, ETextCommit::Type In_Commit_Type)
{
	TArray<UWidget*> Children = this->Hierarchy->GetAllChildren();

	if (Children.IsEmpty())
	{
		return;
	}

	else if (In_Text.IsEmpty())
	{
		this->Current_Index = 0;
		this->Max_Index = 0;
		this->Found_Widgets.Empty();
		
		const FString Index_String = FString::Printf(TEXT("%d of %d"), 0, 0);
		this->Index_Text->SetText(FText::FromString(Index_String));

		this->Toggle_Frame(this->Canvas_Panel, true);

		return;
	}

	else
	{
		AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, In_Text]()
			{
				TArray<UWidget_Hierarchy_Item*> Temp_Found = this->Find_Widgets(In_Text.ToString());

				AsyncTask(ENamedThreads::GameThread, [this, Temp_Found]() mutable
					{
						this->Current_Index = 0;
						
						if (!Temp_Found.IsEmpty())
						{
							this->Max_Index = Temp_Found.Num() - 1;
							
							UWidget_Hierarchy_Item* First_Item = Temp_Found[0];
							
							this->Hierarchy->ScrollWidgetIntoView(First_Item);
							this->Found_Widgets = MoveTemp(Temp_Found);

							const FString Index_String = FString::Printf(TEXT("%d of %d"), this->Current_Index + 1, this->Max_Index + 1);
							this->Index_Text->SetText(FText::FromString(Index_String));

							this->Toggle_Frame(First_Item->Header_Canvas, false);
						}

						else
						{
							this->Max_Index = 0;
							this->Found_Widgets.Empty();
	
							const FString Index_String = FString::Printf(TEXT("%d of %d"), 0, 0);
							this->Index_Text->SetText(FText::FromString(Index_String));

							this->Toggle_Frame(this->Canvas_Panel, true);
						}
					}
				);
			}
		);
	}
}

void UWidget_Hierarchy::On_Search_Next()
{
	if (this->Found_Widgets.IsEmpty())
	{
		return;
	}

	if (this->Current_Index < 0 || this->Current_Index >= this->Max_Index)
	{
		this->Current_Index = 0;
	}

	else
	{
		this->Current_Index++;
	}

	const FString Index_String = FString::Printf(TEXT("%d of %d"), this->Current_Index + 1, this->Max_Index + 1);
	this->Index_Text->SetText(FText::FromString(Index_String));
	
	UWidget_Hierarchy_Item* Current_Item = this->Found_Widgets[this->Current_Index];

	if (IsValid(Current_Item))
	{
		this->Toggle_Frame(Current_Item->Header_Canvas, false);
		this->Hierarchy->ScrollWidgetIntoView(Current_Item);
	}
}

void UWidget_Hierarchy::On_Search_Previous()
{
	if (this->Found_Widgets.IsEmpty())
	{
		return;
	}

	if (this->Current_Index <= 0 || this->Current_Index > this->Max_Index)
	{
		this->Current_Index = this->Max_Index;
	}

	else
	{
		this->Current_Index--;
	}

	const FString Index_String = FString::Printf(TEXT("%d of %d"), this->Current_Index + 1, this->Max_Index + 1);
	this->Index_Text->SetText(FText::FromString(Index_String));
	UWidget_Hierarchy_Item* Current_Item = this->Found_Widgets[this->Current_Index];
	
	if (IsValid(Current_Item))
	{
		this->Toggle_Frame(Current_Item->Header_Canvas, false);
		this->Hierarchy->ScrollWidgetIntoView(Current_Item);
	}
}

bool UWidget_Hierarchy::Toggle_Frame(UCanvasPanel* In_Canvas, bool bDisable)
{
	if (!IsValid(In_Canvas))
	{
		return false;
	}

	In_Canvas->AddChild(this->Item_Frame);
	this->Item_Frame->SetVisibility(bDisable ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	UCanvasPanelSlot* Canvas_Slot = Cast<UCanvasPanelSlot>(this->Item_Frame->Slot);

	if (!IsValid(Canvas_Slot))
	{
		return false;
	}
	
	FAnchors FullAnchors;

	if (!bDisable)
	{
		FullAnchors.Minimum = FVector2D(0.0f, 0.0f);
		FullAnchors.Maximum = FVector2D(1.0f, 1.0f);

		Canvas_Slot->SetAnchors(FullAnchors);
		Canvas_Slot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
		Canvas_Slot->SetAlignment(FVector2D(0.0f, 0.0f));

		return true;
	}

	else
	{
		FullAnchors.Minimum = FVector2D(0.0f, 0.0f);
		FullAnchors.Maximum = FVector2D(0.0f, 0.0f);

		Canvas_Slot->SetAnchors(FullAnchors);
		Canvas_Slot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
		Canvas_Slot->SetAlignment(FVector2D(0.0f, 0.0f));

		return true;
	}
}