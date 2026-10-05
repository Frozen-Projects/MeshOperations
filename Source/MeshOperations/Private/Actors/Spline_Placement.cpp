#include "Actors/Spline_Placement.h"

// Sets default values.
ASpline_Placement::ASpline_Placement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!this->RootComponent)
	{
		this->DefaultSceneRoot = this->CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
		this->RootComponent = this->DefaultSceneRoot;
	}

	this->Spline = this->CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	this->Spline->SetupAttachment(this->DefaultSceneRoot);
}

// Called when the game starts or when spawned.
void ASpline_Placement::BeginPlay()
{
	Super::BeginPlay();
}

// Called when the game ends or when destroyed.
void ASpline_Placement::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame.
void ASpline_Placement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpline_Placement::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (IsValid(this->Spline))
	{
		this->SetMaterialParameters();
		this->GenerateSplineMesh();
	}
}

bool ASpline_Placement::SetMaterialParameters()
{
	const int32 NumMaterials = this->Materials.Num();
	const int32 NumMaterialParameters = this->MaterialParameters.Num();

	if (NumMaterials != NumMaterialParameters)
	{
		return false;
	}

	for (int32 Index_Material = 0; Index_Material < NumMaterials; Index_Material++)
	{
		UMaterialInterface* Each_Material = this->Materials[Index_Material];

		if (!IsValid(Each_Material))
		{
			continue;
		}

		UMaterialInstanceDynamic* DynamicMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, Each_Material, NAME_None);

		if (!IsValid(DynamicMaterial))
		{
			continue;
		}

		const FMaterialParametersArray& Each_Material_Parameters = this->MaterialParameters[Index_Material];

		for (const FMaterialParameters& Each_Parameter : Each_Material_Parameters.MaterialParameters)
		{
			switch (Each_Parameter.Param_Type)
			{
				case ETextureParameterType::Scalar:
					DynamicMaterial->SetScalarParameterValue(Each_Parameter.Param_Name, Each_Parameter.Param_Scalar);
					break;

				case ETextureParameterType::Vectoral:
					DynamicMaterial->SetVectorParameterValue(Each_Parameter.Param_Name, Each_Parameter.Param_Vector);
					break;

				case ETextureParameterType::Texture:
					DynamicMaterial->SetTextureParameterValue(Each_Parameter.Param_Name, Each_Parameter.Param_Texture);
					break;
			}
		}

		this->DynamicMaterials.Add(DynamicMaterial);
	}

	return true;
}

void ASpline_Placement::GenerateSplineMesh()
{
	const double SplineLenght = this->Spline->GetSplineLength();
	const int32 NumSections = FMath::TruncToInt(SplineLenght / this->SectionLength);

	for (int32 Index_Spline_Points = 0; Index_Spline_Points < NumSections; Index_Spline_Points++)
	{
		USplineMeshComponent* SplineMeshComp = Cast<USplineMeshComponent>(AddComponentByClass(USplineMeshComponent::StaticClass(), true, FTransform::Identity, false));

		if (!IsValid(SplineMeshComp))
		{
			continue;
		}

		SplineMeshComp->SetMobility(this->Spline->GetMobility());

		if (!SplineMeshComp->AttachToComponent(this->Spline, FAttachmentTransformRules::SnapToTargetIncludingScale))
		{
			SplineMeshComp->DestroyComponent();
			continue;
		}

		SplineMeshComp->SetForwardAxis(this->ForwardAxis);
		
		if (IsValid(this->Mesh))
		{
			SplineMeshComp->SetStaticMesh(this->Mesh);
		}

		SplineMeshComp->SetCastShadow(this->bEnableShadow);

		const double Distance_Start = this->SectionLength * Index_Spline_Points;
		const FVector Pos_Start = this->Spline->GetLocationAtDistanceAlongSpline(Distance_Start, ESplineCoordinateSpace::Local);
		const FVector Tangent_Raw_Start = this->Spline->GetTangentAtDistanceAlongSpline(Distance_Start, ESplineCoordinateSpace::Local);
		const FVector Tangent_Start = UKismetMathLibrary::ClampVectorSize(Tangent_Raw_Start, 0.0, this->SectionLength);

		const double Distance_End = this->SectionLength * (Index_Spline_Points + 1);
		const FVector Pos_End = this->Spline->GetLocationAtDistanceAlongSpline(Distance_End, ESplineCoordinateSpace::Local);
		const FVector Tangent_Raw_End = this->Spline->GetTangentAtDistanceAlongSpline(Distance_End, ESplineCoordinateSpace::Local);
		const FVector Tangent_End = UKismetMathLibrary::ClampVectorSize(Tangent_Raw_End, 0.0, this->SectionLength);

		SplineMeshComp->SetStartAndEnd(Pos_Start, Tangent_Start, Pos_End, Tangent_End);
		SplineMeshComp->SetStartScale(FVector2D(this->Thickness), true);
		SplineMeshComp->SetEndScale(FVector2D(this->Thickness), true);

		auto SetMaterial = [SplineMeshComp]<typename T>(const TArray<T>&Materials)
		{
			const int32 NumMaterials = Materials.Num();

			for (int32 Index_Material = 0; Index_Material < NumMaterials; Index_Material++)
			{
				UMaterialInterface* Each_Material = Materials[Index_Material];
				
				if (IsValid(Each_Material))
				{
					SplineMeshComp->SetMaterial(Index_Material, Each_Material);
				}
			}
		};

		if (!this->DynamicMaterials.IsEmpty())
		{
			SetMaterial(this->DynamicMaterials);
		}

		else if (!this->Materials.IsEmpty())
		{
			SetMaterial(this->Materials);
		}
	}
}