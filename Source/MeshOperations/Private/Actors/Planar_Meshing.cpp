#include "Actors/Planar_Meshing.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlanarMeshing, Log, All);

namespace PlanarMeshing
{
	constexpr double PointTolerance = 0.001;

	double Cross(const FVector2D& A, const FVector2D& B)
	{
		return A.X * B.Y - A.Y * B.X;
	}

	bool IsOnSegment(const FVector2D& Point, const FVector2D& A, const FVector2D& B)
	{
		const FVector2D Edge = B - A;
		return FMath::Abs(Cross(Edge, Point - A)) <= PointTolerance * Edge.Size()
			&& Point.X >= FMath::Min(A.X, B.X) - PointTolerance && Point.X <= FMath::Max(A.X, B.X) + PointTolerance
			&& Point.Y >= FMath::Min(A.Y, B.Y) - PointTolerance && Point.Y <= FMath::Max(A.Y, B.Y) + PointTolerance;
	}

	bool SegmentsIntersect(const FVector2D& A, const FVector2D& B, const FVector2D& C, const FVector2D& D)
	{
		if (FMath::Max(A.X, B.X) + PointTolerance < FMath::Min(C.X, D.X)
			|| FMath::Max(C.X, D.X) + PointTolerance < FMath::Min(A.X, B.X)
			|| FMath::Max(A.Y, B.Y) + PointTolerance < FMath::Min(C.Y, D.Y)
			|| FMath::Max(C.Y, D.Y) + PointTolerance < FMath::Min(A.Y, B.Y))
		{
			return false;
		}

		const double SideC = Cross(B - A, C - A);
		const double SideD = Cross(B - A, D - A);
		const double SideA = Cross(D - C, A - C);
		const double SideB = Cross(D - C, B - C);
		const bool bOppositeAB = (SideC < 0.0 && SideD > 0.0) || (SideC > 0.0 && SideD < 0.0);
		const bool bOppositeCD = (SideA < 0.0 && SideB > 0.0) || (SideA > 0.0 && SideB < 0.0);
		return (bOppositeAB && bOppositeCD) || IsOnSegment(A, C, D) || IsOnSegment(B, C, D)
			|| IsOnSegment(C, A, B) || IsOnSegment(D, A, B);
	}

	void RemoveRedundantPoints(TArray<FVector2D>& Boundary)
	{
		bool bRemoved = true;
		while (bRemoved && Boundary.Num() >= 3)
		{
			bRemoved = false;
			for (int32 Index = 0; Index < Boundary.Num(); ++Index)
			{
				const FVector2D& Previous = Boundary[(Index + Boundary.Num() - 1) % Boundary.Num()];
				const FVector2D& Current = Boundary[Index];
				const FVector2D& Next = Boundary[(Index + 1) % Boundary.Num()];
				if ((Current - Previous).SizeSquared() <= FMath::Square(PointTolerance)
					|| (IsOnSegment(Current, Previous, Next) && FVector2D::DotProduct(Current - Previous, Next - Current) >= 0.0))
				{
					Boundary.RemoveAt(Index, 1, EAllowShrinking::No);
					bRemoved = true;
					break;
				}
			}
		}
	}
}

APlanar_Meshing::APlanar_Meshing()
{
	PrimaryActorTick.bCanEverTick = false;
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(DefaultSceneRoot);
	Spline->bInputSplinePointsToConstructionScript = true;
	Spline->SetSplinePoints({FVector(0, 0, 0), FVector(500, 0, 0), FVector(500, 500, 0), FVector(0, 500, 0)}, ESplineCoordinateSpace::Local, false);
	for (int32 Index = 0; Index < 4; ++Index)
	{
		Spline->SetSplinePointType(Index, ESplinePointType::Linear, false);
	}
	Spline->SetClosedLoop(true);

	GeneratedMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	GeneratedMesh->SetupAttachment(DefaultSceneRoot);
	GeneratedMesh->SetCollisionProfileName(TEXT("BlockAll"));
	GeneratedMesh->bUseComplexAsSimpleCollision = true;
	GeneratedMesh->bUseAsyncCooking = true;
}

void APlanar_Meshing::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (bGenerateOnConstruction)
	{
		Generate_Mesh();
	}
}

void APlanar_Meshing::BeginPlay()
{
	Super::BeginPlay();
	if (bGenerateOnBeginPlay)
	{
		Generate_Mesh();
	}
}

void APlanar_Meshing::Clear_Mesh()
{
	if (IsValid(GeneratedMesh))
	{
		GeneratedMesh->ClearAllMeshSections();
	}
	GeneratedVertexCount = 0;
	GeneratedTriangleCount = 0;
	LastGenerationError.Reset();
}

bool APlanar_Meshing::FailGeneration(const TCHAR* Reason)
{
	Clear_Mesh();
	LastGenerationError = Reason;
	UE_LOG(LogPlanarMeshing, Warning, TEXT("%s: %s"), *GetName(), Reason);
	return false;
}

bool APlanar_Meshing::Generate_Mesh()
{
	using namespace PlanarMeshing;
	USplineComponent* BoundarySpline = SourceSpline ? SourceSpline.Get() : Spline.Get();
	if (!IsValid(BoundarySpline) || !IsValid(GeneratedMesh))
	{
		return FailGeneration(TEXT("The boundary spline or generated mesh component is invalid."));
	}
	if (!BoundarySpline->IsClosedLoop() || BoundarySpline->GetNumberOfSplinePoints() < 3)
	{
		return FailGeneration(TEXT("Use a closed loop spline with at least three points."));
	}
	if (!FMath::IsFinite(SampleSpacing) || SampleSpacing < 0.1 || !FMath::IsFinite(UVTileSize) || UVTileSize < 0.1
		|| !FMath::IsFinite(HeightOffset) || UVOffset.ContainsNaN() || MaxBoundaryVertices < 3 || MaxBoundaryVertices > 8192)
	{
		return FailGeneration(TEXT("Invalid sampling, UV, height, or vertex-limit settings."));
	}

	BoundarySpline->UpdateSpline();
	const FTransform SplineTransform = BoundarySpline->GetComponentTransform();
	const FTransform MeshTransform = GeneratedMesh->GetComponentTransform();
	if (!SplineTransform.IsValid() || !MeshTransform.IsValid()
		|| SplineTransform.GetScale3D().GetAbsMin() < UE_SMALL_NUMBER || MeshTransform.GetScale3D().GetAbsMin() < UE_SMALL_NUMBER)
	{
		return FailGeneration(TEXT("Spline and mesh transforms must be finite and have nonzero scale."));
	}

	const FVector FirstPoint = BoundarySpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local);
	if (FirstPoint.ContainsNaN())
	{
		return FailGeneration(TEXT("The spline contains an invalid point."));
	}
	const FVector2D Origin(FirstPoint.X, FirstPoint.Y);
	const double PlaneHeight = FirstPoint.Z + HeightOffset;
	TArray<FVector2D> Boundary;
	const int32 NumSplinePoints = BoundarySpline->GetNumberOfSplinePoints();
	if (NumSplinePoints > MaxBoundaryVertices)
	{
		return FailGeneration(TEXT("The spline exceeds MaxBoundaryVertices."));
	}

	for (int32 Segment = 0; Segment < NumSplinePoints; ++Segment)
	{
		const ESplinePointType::Type PointType = BoundarySpline->GetSplinePointType(Segment);
		if (PointType == ESplinePointType::Constant)
		{
			return FailGeneration(TEXT("Constant spline segments are discontinuous; use Linear or Curve points."));
		}
		const double StartDistance = BoundarySpline->GetDistanceAlongSplineAtSplinePoint(Segment);
		const double EndDistance = BoundarySpline->GetDistanceAlongSplineAtSplinePoint(Segment + 1);
		const double SegmentLength = EndDistance - StartDistance;
		if (!FMath::IsFinite(SegmentLength) || SegmentLength < 0.0)
		{
			return FailGeneration(TEXT("The spline contains an invalid segment length."));
		}
		const double RequestedSamples = PointType == ESplinePointType::Linear ? 1.0 : FMath::Max(4.0, FMath::CeilToDouble(SegmentLength / SampleSpacing));
		if (RequestedSamples > MaxBoundaryVertices - Boundary.Num())
		{
			return FailGeneration(TEXT("Sampling exceeds MaxBoundaryVertices; increase SampleSpacing or the vertex limit."));
		}
		const int32 NumSamples = static_cast<int32>(RequestedSamples);
		for (int32 Sample = 0; Sample < NumSamples; ++Sample)
		{
			// Always retain control points, including sharp corners and zero-length segments.
			const FVector Position = Sample == 0 ? BoundarySpline->GetLocationAtSplinePoint(Segment, ESplineCoordinateSpace::Local)
				: BoundarySpline->GetLocationAtDistanceAlongSpline(StartDistance + SegmentLength * Sample / NumSamples, ESplineCoordinateSpace::Local);
			if (Position.ContainsNaN())
			{
				return FailGeneration(TEXT("The spline contains an invalid sampled position."));
			}
			const FVector2D Point = FVector2D(Position.X, Position.Y) - Origin;
			if (Boundary.IsEmpty() || (Point - Boundary.Last()).SizeSquared() > FMath::Square(PointTolerance))
			{
				Boundary.Add(Point);
			}
		}
	}

	RemoveRedundantPoints(Boundary);
	if (Boundary.Num() < 3)
	{
		return FailGeneration(TEXT("The projected boundary needs at least three distinct, non-collinear points."));
	}

	double SignedArea2 = 0.0;
	for (int32 Index = 0; Index < Boundary.Num(); ++Index)
	{
		const int32 Next = (Index + 1) % Boundary.Num();
		const FVector2D Incoming = Boundary[Index] - Boundary[(Index + Boundary.Num() - 1) % Boundary.Num()];
		const FVector2D Outgoing = Boundary[Next] - Boundary[Index];
		if (FMath::Abs(Cross(Incoming, Outgoing)) <= PointTolerance * FMath::Max(Incoming.Size(), Outgoing.Size())
			&& FVector2D::DotProduct(Incoming, Outgoing) < 0.0)
		{
			return FailGeneration(TEXT("The projected boundary contains overlapping adjacent edges."));
		}
		SignedArea2 += Cross(Boundary[Index], Boundary[Next]);
		for (int32 Other = Index + 1; Other < Boundary.Num(); ++Other)
		{
			const int32 OtherNext = (Other + 1) % Boundary.Num();
			if (Next != Other && OtherNext != Index && SegmentsIntersect(Boundary[Index], Boundary[Next], Boundary[Other], Boundary[OtherNext]))
			{
				return FailGeneration(TEXT("The projected boundary crosses or touches itself; use a simple closed loop."));
			}
		}
	}
	if (!FMath::IsFinite(SignedArea2) || FMath::Abs(SignedArea2) <= FMath::Square(PointTolerance))
	{
		return FailGeneration(TEXT("The projected boundary has zero or negligible area."));
	}

	TArray<UE::Geometry::FIndex3i> PolygonTriangles;
	PolygonTriangulation::TriangulateSimplePolygon(Boundary, PolygonTriangles, false);
	if (PolygonTriangles.Num() != Boundary.Num() - 2)
	{
		return FailGeneration(TEXT("Boundary triangulation did not produce a complete surface."));
	}

	const FVector AxisX = MeshTransform.InverseTransformVector(SplineTransform.TransformVector(FVector::ForwardVector));
	const FVector AxisY = MeshTransform.InverseTransformVector(SplineTransform.TransformVector(FVector::RightVector));
	const bool bMirrored = (SplineTransform.GetDeterminant() < 0.0) != (MeshTransform.GetDeterminant() < 0.0);
	const bool bReverseFacing = bMirrored != bFlipNormals;
	const FVector Normal = FVector::CrossProduct(AxisX, AxisY).GetSafeNormal() * (bReverseFacing ? -1.0 : 1.0);
	const FProcMeshTangent Tangent(AxisX.GetSafeNormal(), bReverseFacing);
	if (Normal.IsNearlyZero())
	{
		return FailGeneration(TEXT("The transformed plane is degenerate."));
	}

	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<int32> Triangles;
	Vertices.Reserve(Boundary.Num());
	UVs.Reserve(Boundary.Num());
	Normals.Init(Normal, Boundary.Num());
	Tangents.Init(Tangent, Boundary.Num());
	for (const FVector2D& Point : Boundary)
	{
		const FVector Position(Point.X + Origin.X, Point.Y + Origin.Y, PlaneHeight);
		const FVector MeshPosition = MeshTransform.InverseTransformPosition(SplineTransform.TransformPosition(Position));
		const FVector2D UV = (Point + Origin) / UVTileSize + UVOffset;
		if (MeshPosition.ContainsNaN() || UV.ContainsNaN())
		{
			return FailGeneration(TEXT("The generated positions or UVs are not finite."));
		}
		Vertices.Add(MeshPosition);
		UVs.Add(UV);
	}

	double TriangleArea2 = 0.0;
	Triangles.Reserve(PolygonTriangles.Num() * 3);
	for (const UE::Geometry::FIndex3i& Triangle : PolygonTriangles)
	{
		const double Area2 = Cross(Boundary[Triangle.B] - Boundary[Triangle.A], Boundary[Triangle.C] - Boundary[Triangle.A]);
		if (!FMath::IsFinite(Area2) || Area2 * FMath::Sign(SignedArea2) <= UE_DOUBLE_SMALL_NUMBER)
		{
			return FailGeneration(TEXT("Triangulation produced a degenerate or inverted triangle."));
		}
		TriangleArea2 += FMath::Abs(Area2);
		// Procedural meshes use clockwise indices for a +Z front face.
		const bool bSwap = (Area2 > 0.0) != bReverseFacing;
		Triangles.Add(Triangle.A);
		Triangles.Add(bSwap ? Triangle.C : Triangle.B);
		Triangles.Add(bSwap ? Triangle.B : Triangle.C);
	}
	if (!FMath::IsNearlyEqual(TriangleArea2, FMath::Abs(SignedArea2), FMath::Max(1.0, FMath::Abs(SignedArea2)) * 1.e-8))
	{
		return FailGeneration(TEXT("Triangulation does not cover the boundary area correctly."));
	}

	GeneratedMesh->bUseAsyncCooking = bUseAsyncCooking;
	GeneratedMesh->bUseComplexAsSimpleCollision = true;
	GeneratedMesh->SetCollisionEnabled(bCreateCollision ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	GeneratedMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, bCreateCollision);
	GeneratedMesh->SetMaterial(0, Material);
	GeneratedVertexCount = Vertices.Num();
	GeneratedTriangleCount = Triangles.Num() / 3;
	LastGenerationError.Reset();
	return true;
}
