#include "Actors/Planar_Placement.h"

// Sets default values.
APlanar_Placement::APlanar_Placement()
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
void APlanar_Placement::BeginPlay()
{
	Super::BeginPlay();
}

// Called when the game ends or when destroyed.
void APlanar_Placement::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame.
void APlanar_Placement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlanar_Placement::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (IsValid(this->Spline))
	{
		this->Generate_Grid();
	}
}

bool APlanar_Placement::IsPointInsideSpline(const FVector& Point) const 
{
	// Find the closest point on the spline to the current grid point.
	const FVector ClosestSplinePoint = this->Spline->FindLocationClosestToWorldLocation(Point, ESplineCoordinateSpace::World);

	// Use the cross product and distance to determine if the point is inside or outside the spline.
	const FVector DirectionToSpline = (ClosestSplinePoint - Point).GetSafeNormal();
	const FVector SplineTangent = this->Spline->FindDirectionClosestToWorldLocation(Point, ESplineCoordinateSpace::World);
	const FVector CrossProduct = FVector::CrossProduct(FVector(0, 0, 1), SplineTangent);

	// Use the dot product to determine whether the point is inside or outside.
	const float DotProduct = FVector::DotProduct(DirectionToSpline, CrossProduct);

	// If the dot product is negative, the point is inside the spline.
	return DotProduct < 0;
}

TArray<FTransform> APlanar_Placement::GetGridVertices() const
{
	return this->Grid_Vertices;
}

bool APlanar_Placement::Generate_Grid()
{
	if (!IsValid(this->Spline))
	{
		UE_LOG(LogTemp, Warning, TEXT("APlanar_Placement::Generate_Grid -> Boundry spline is not valid."));
		return false;
	}

	if (this->GridSize <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("APlanar_Placement::Generate_Grid -> Grid size have to be bigger than 0."));
		return false;
	}

	if (this->Layer <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("APlanar_Placement::Generate_Grid -> Layer size have to be bigger than 0."));
		return false;
	}

	if (this->Layer_Height <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("APlanar_Placement::Generate_Grid -> Layer_Height have to be bigger than 0."));
		return false;
	}

	// Get the bounding box of the spline.
	const FBox SplineBounds = this->Spline->Bounds.GetBox();
	const FVector Origin = SplineBounds.GetCenter();
	const FVector Extent = SplineBounds.GetExtent();

	// Calculate the number of points to generate along each axis.
	const int32 NumPointsX = FMath::CeilToInt(Extent.X * 2 / this->GridSize);
	const int32 NumPointsY = FMath::CeilToInt(Extent.Y * 2 / this->GridSize);

	this->Grid_Vertices.Empty();

	for (int32 i = -NumPointsX / 2; i <= NumPointsX / 2; i++)
	{
		for (int32 j = -NumPointsY / 2; j <= NumPointsY / 2; j++)
		{
			// Calculate the position of the current grid point.
			const FVector FloorLocation = Origin + FVector(i * this->GridSize, j * this->GridSize, 0);

			if (this->IsPointInsideSpline(FloorLocation))
			{
				for (int32 Index_Layer = 0; Index_Layer < this->Layer; Index_Layer++)
				{
					const FVector EachLocation = {FloorLocation.X, FloorLocation.Y, Index_Layer * this->Layer_Height + FloorLocation.Z };

					FTransform EachTransform;
					EachTransform.SetLocation(EachLocation);
					EachTransform.SetScale3D(this->ObjectSize);

					this->Grid_Vertices.Add(EachTransform);
				}
			}
		}
	}

	return true;
}