// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Planar_Placement.h"

// Sets default values.
APlanar_Placement::APlanar_Placement()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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

bool APlanar_Placement::IsPointInsideSpline(USplineComponent* BoundarySpline, const FVector& Point)
{
	// Find the closest point on the spline to the current grid point.
	const FVector ClosestSplinePoint = BoundarySpline->FindLocationClosestToWorldLocation(Point, ESplineCoordinateSpace::World);

	// Use the cross product and distance to determine if the point is inside or outside the spline.
	const FVector DirectionToSpline = (ClosestSplinePoint - Point).GetSafeNormal();
	const FVector SplineTangent = BoundarySpline->FindDirectionClosestToWorldLocation(Point, ESplineCoordinateSpace::World);
	const FVector CrossProduct = FVector::CrossProduct(FVector(0, 0, 1), SplineTangent);

	// Use the dot product to determine whether the point is inside or outside.
	const float DotProduct = FVector::DotProduct(DirectionToSpline, CrossProduct);

	// If the dot product is negative, the point is inside the spline.
	return DotProduct < 0;
}

bool APlanar_Placement::Grid_Generate(TArray<FTransform>& Out_Vertices, USplineComponent* BoundarySpline, FVector Size, double GridSize, int32 Layer, double Height)
{
	if (!IsValid(BoundarySpline))
	{
		UE_LOG(LogTemp, Warning, TEXT("APlanar_Placement::Grid_Generate -> Boundry spline is not valid."));
		return false;
	}

	if (GridSize <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("APlanar_Placement::Grid_Generate -> Grid size have to be bigger than 0."));
		return false;
	}

	if (Layer <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("APlanar_Placement::Grid_Generate -> Layer size have to be bigger than 0."));
		return false;
	}

	if (Height <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("APlanar_Placement::Grid_Generate -> Height have to be bigger than 0."));
		return false;
	}

	// Get the bounding box of the spline.
	const FBox SplineBounds = BoundarySpline->Bounds.GetBox();
	const FVector Origin = SplineBounds.GetCenter();
	const FVector Extent = SplineBounds.GetExtent();

	// Calculate the number of points to generate along each axis.
	const int32 NumPointsX = FMath::CeilToInt(Extent.X * 2 / GridSize);
	const int32 NumPointsY = FMath::CeilToInt(Extent.Y * 2 / GridSize);

	TArray<FTransform> Temp_Vertices;

	for (int32 i = -NumPointsX / 2; i <= NumPointsX / 2; i++)
	{
		for (int32 j = -NumPointsY / 2; j <= NumPointsY / 2; j++)
		{
			// Calculate the position of the current grid point.
			const FVector FloorLocation = Origin + FVector(i * GridSize, j * GridSize, 0);

			if (APlanar_Placement::IsPointInsideSpline(BoundarySpline, FloorLocation))
			{
				for (int32 Index_Layer = 0; Index_Layer < Layer; Index_Layer++)
				{
					const FVector EachLocation = {FloorLocation.X, FloorLocation.Y, Index_Layer * Height + FloorLocation.Z };

					FTransform EachTransform;
					EachTransform.SetLocation(EachLocation);
					EachTransform.SetScale3D(Size);

					Temp_Vertices.Add(EachTransform);
				}
			}
		}
	}

	Out_Vertices = Temp_Vertices;
	return true;
}

void APlanar_Placement::Grid_Debug(FVector Point, bool bInside, bool bIsPersistant, double Time)
{
	const FColor Color = bInside ? FColor::Green : FColor::Red;
	const float PointSize = 10.0f;

	DrawDebugPoint(GetWorld(), Point, PointSize, Color, bIsPersistant, Time);
}