// Copyright Epic Games, Inc. All Rights Reserved.

#include "MeshOperationsBPLibrary.h"
#include "MeshOperations.h"

UMeshOperationsBPLibrary::UMeshOperationsBPLibrary(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

FString UMeshOperationsBPLibrary::GetClassName(const UObject* Object)
{
    UClass* Class = Object->GetClass();
    return Class->GetName();
}

FString UMeshOperationsBPLibrary::GetObjectNameForPackage(USceneComponent* Object, bool bUseReadableName)
{
    if (!IsValid(Object))
    {
        return FString();
    }

    const FString ObjectName = bUseReadableName ? Object->GetReadableName() : Object->GetName();

#if WITH_EDITOR
   
    return ObjectName;

#else
   
    int32 LastUnderscoreIndex;

    // Packaged projects have "_SOMETHING" like suffixes on component names, so we need to remove those to get the original name of the component.
    if (ObjectName.FindLastChar(TEXT('_'), LastUnderscoreIndex))
    {
        return ObjectName.Left(LastUnderscoreIndex);
    }

    return ObjectName;

#endif
}

bool UMeshOperationsBPLibrary::GetComponentByName(FName InName, UObject* Owner, USceneComponent*& OutComponent)
{
    OutComponent = Cast<USceneComponent>(Owner->GetDefaultSubobjectByName(InName));

    if (IsValid(OutComponent) == true)
    {
        return true;
    }

    else
    {
        return false;
    }
}

UStaticMeshComponent* UMeshOperationsBPLibrary::AddStaticMeshCompWithName(AActor* Outer, UStaticMesh* In_Mesh, FTransform RelativeTransform, FName In_Name, bool Manual_Attachment, EAttachmentRule Attachment_Rule, EComponentMobility::Type Mobility)
{
    if (!IsValid(Outer))
    {
        return nullptr;
    }

	UStaticMeshComponent* StaticMeshComp = NewObject<UStaticMeshComponent>(Outer, In_Name.ToString().IsEmpty() ? NAME_None : In_Name);

    if (StaticMeshComp == nullptr)
    {
        return nullptr;
    }

    StaticMeshComp->SetMobility(Mobility);

    if (!Manual_Attachment)
    {
        USceneComponent* Root = Outer->GetRootComponent();

        if (IsValid(Root))
        {
            StaticMeshComp->AttachToComponent(Root, FAttachmentTransformRules(Attachment_Rule, true));
        }

        else
        {
            Outer->SetRootComponent(StaticMeshComp);
        }
    }

    if (IsValid(In_Mesh))
    {
        StaticMeshComp->SetStaticMesh(In_Mesh);
    }

    StaticMeshComp->SetRelativeTransform(RelativeTransform);
    StaticMeshComp->RegisterComponent();

    return StaticMeshComp;
}

USceneComponent* UMeshOperationsBPLibrary::AddSceneCompWithName(AActor* Outer, FTransform RelativeTransform, FName In_Name, bool Manual_Attachment, EAttachmentRule Attachment_Rule, EComponentMobility::Type Mobility)
{
    if (!IsValid(Outer))
    {
		return nullptr;
    }

    USceneComponent* SceneComp = NewObject<USceneComponent>(Outer, In_Name.ToString().IsEmpty() ? NAME_None : In_Name);

    if (!IsValid(SceneComp))
    {
        return nullptr;
    }

    SceneComp->SetMobility(Mobility);

    if (!Manual_Attachment)
    {
        USceneComponent* Root = Outer->GetRootComponent();

        if (IsValid(Root))
        {
            SceneComp->AttachToComponent(Root, FAttachmentTransformRules(Attachment_Rule, true));
        }

        else
        {
            Outer->SetRootComponent(SceneComp);
        }
    }

    SceneComp->SetRelativeTransform(RelativeTransform);
	return SceneComp;
}

UProceduralMeshComponent* UMeshOperationsBPLibrary::AddProcMeshCompWithName(AActor* Outer, FName In_Name, EAttachmentRule Attachment_Rule, bool Manual_Attachment, bool bUseAsyncCooking, bool bUseComplexCollisionAsSimple, FTransform Relative_Transform, EComponentMobility::Type Mobility)
{
    if (!IsValid(Outer))
    {
        return nullptr;
    }

    UProceduralMeshComponent* ProcMeshComp = NewObject<UProceduralMeshComponent>(Outer, In_Name.ToString().IsEmpty() ? NAME_None : In_Name);

    if (!IsValid(ProcMeshComp))
    {
        return nullptr;
    }

    ProcMeshComp->SetMobility(Mobility);
    ProcMeshComp->bUseAsyncCooking = bUseAsyncCooking;
    ProcMeshComp->bUseComplexAsSimpleCollision = bUseComplexCollisionAsSimple;

    if (!Manual_Attachment)
    {
        USceneComponent* Root = Outer->GetRootComponent();

        if (IsValid(Root))
        {
            ProcMeshComp->AttachToComponent(Root, FAttachmentTransformRules(Attachment_Rule, true));
        }

        else
        {
            Outer->SetRootComponent(ProcMeshComp);
        }
    }

    ProcMeshComp->SetRelativeTransform(Relative_Transform);
    ProcMeshComp->RegisterComponent();

    return ProcMeshComp;
}

void UMeshOperationsBPLibrary::GenerateBoxMeshAtBottom(FVector BoxRadius, TArray<FVector>&Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& ProcMeshTangents, TArray<FVector>& Tangents)
{
    FVector BoxVerts[8];
    // Top vertices (shifted upward)
    BoxVerts[0] = FVector(-BoxRadius.X, BoxRadius.Y, 2 * BoxRadius.Z);
    BoxVerts[1] = FVector(BoxRadius.X, BoxRadius.Y, 2 * BoxRadius.Z);
    BoxVerts[2] = FVector(BoxRadius.X, -BoxRadius.Y, 2 * BoxRadius.Z);
    BoxVerts[3] = FVector(-BoxRadius.X, -BoxRadius.Y, 2 * BoxRadius.Z);

    // Bottom vertices
    BoxVerts[4] = FVector(-BoxRadius.X, BoxRadius.Y, 0);
    BoxVerts[5] = FVector(BoxRadius.X, BoxRadius.Y, 0);
    BoxVerts[6] = FVector(BoxRadius.X, -BoxRadius.Y, 0);
    BoxVerts[7] = FVector(-BoxRadius.X, -BoxRadius.Y, 0);

    // Generate triangles (from quads)
    Triangles.Reset();
    const int32 NumVerts = 24; // 6 faces x 4 verts per face

    Vertices.Reset();
    Vertices.AddUninitialized(NumVerts);

    Normals.Reset();
    Normals.AddUninitialized(NumVerts);

    ProcMeshTangents.Reset();
    ProcMeshTangents.AddUninitialized(NumVerts);

    Tangents.Reset();
    Tangents.AddUninitialized(NumVerts);

    // Top face
    Vertices[0] = BoxVerts[0];
    Vertices[1] = BoxVerts[1];
    Vertices[2] = BoxVerts[2];
    Vertices[3] = BoxVerts[3];
    UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 0, 1, 2, 3);
    Normals[0] = Normals[1] = Normals[2] = Normals[3] = FVector(0, 0, 1);
    ProcMeshTangents[0] = ProcMeshTangents[1] = ProcMeshTangents[2] = ProcMeshTangents[3] = FProcMeshTangent(0.f, -1.f, 0.f);
    Tangents[0] = Tangents[1] = Tangents[2] = Tangents[3] = FVector(0.f, -1.f, 0.f);

    // Left face
    Vertices[4] = BoxVerts[4];
    Vertices[5] = BoxVerts[0];
    Vertices[6] = BoxVerts[3];
    Vertices[7] = BoxVerts[7];
    UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 4, 5, 6, 7);
    Normals[4] = Normals[5] = Normals[6] = Normals[7] = FVector(-1, 0, 0);
    ProcMeshTangents[4] = ProcMeshTangents[5] = ProcMeshTangents[6] = ProcMeshTangents[7] = FProcMeshTangent(0.f, -1.f, 0.f);
    Tangents[4] = Tangents[5] = Tangents[6] = Tangents[7] = FVector(0.f, -1.f, 0.f);

    // Front face
    Vertices[8] = BoxVerts[5];
    Vertices[9] = BoxVerts[1];
    Vertices[10] = BoxVerts[0];
    Vertices[11] = BoxVerts[4];
    UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 8, 9, 10, 11);
    Normals[8] = Normals[9] = Normals[10] = Normals[11] = FVector(0, 1, 0);
    ProcMeshTangents[8] = ProcMeshTangents[9] = ProcMeshTangents[10] = ProcMeshTangents[11] = FProcMeshTangent(-1.f, 0.f, 0.f);
    Tangents[8] = Tangents[9] = Tangents[10] = Tangents[11] = FVector(0.f, -1.f, 0.f);

    // Right face
    Vertices[12] = BoxVerts[6];
    Vertices[13] = BoxVerts[2];
    Vertices[14] = BoxVerts[1];
    Vertices[15] = BoxVerts[5];
    UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 12, 13, 14, 15);
    Normals[12] = Normals[13] = Normals[14] = Normals[15] = FVector(1, 0, 0);
    ProcMeshTangents[12] = ProcMeshTangents[13] = ProcMeshTangents[14] = ProcMeshTangents[15] = FProcMeshTangent(0.f, 1.f, 0.f);
    Tangents[12] = Tangents[13] = Tangents[14] = Tangents[15] = FVector(0.f, -1.f, 0.f);

    // Back face
    Vertices[16] = BoxVerts[7];
    Vertices[17] = BoxVerts[3];
    Vertices[18] = BoxVerts[2];
    Vertices[19] = BoxVerts[6];
    UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 16, 17, 18, 19);
    Normals[16] = Normals[17] = Normals[18] = Normals[19] = FVector(0, -1, 0);
    ProcMeshTangents[16] = ProcMeshTangents[17] = ProcMeshTangents[18] = ProcMeshTangents[19] = FProcMeshTangent(1.f, 0.f, 0.f);
    Tangents[16] = Tangents[17] = Tangents[18] = Tangents[19] = FVector(0.f, -1.f, 0.f);

    // Bottom face
    Vertices[20] = BoxVerts[7];
    Vertices[21] = BoxVerts[6];
    Vertices[22] = BoxVerts[5];
    Vertices[23] = BoxVerts[4];
    UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 20, 21, 22, 23);
    Normals[20] = Normals[21] = Normals[22] = Normals[23] = FVector(0, 0, -1);
    ProcMeshTangents[20] = ProcMeshTangents[21] = ProcMeshTangents[22] = ProcMeshTangents[23] = FProcMeshTangent(0.f, 1.f, 0.f);
    Tangents[20] = Tangents[21] = Tangents[22] = Tangents[23] = FVector(0.f, -1.f, 0.f);

    // UVs
    UVs.Reset();
    UVs.AddUninitialized(NumVerts);
    UVs[0] = UVs[4] = UVs[8] = UVs[12] = UVs[16] = UVs[20] = FVector2D(0.f, 0.f);
    UVs[1] = UVs[5] = UVs[9] = UVs[13] = UVs[17] = UVs[21] = FVector2D(0.f, 1.f);
    UVs[2] = UVs[6] = UVs[10] = UVs[14] = UVs[18] = UVs[22] = FVector2D(1.f, 1.f);
    UVs[3] = UVs[7] = UVs[11] = UVs[15] = UVs[19] = UVs[23] = FVector2D(1.f, 0.f);
}

void UMeshOperationsBPLibrary::GenerateCylinderMesh(double Radius, double ArcSize, TArray<FVector2D>& Vertices, int32& EdgeTriangles)
{
    TArray<FVector2D> TempVertices;
    TArray<int32> TempTriangles;

    const int32 NumSegments_Cap = FMath::RoundToInt(ArcSize / 2.0f);

    TempVertices.Add(FVector2D(0, 0));
    for (int32 Index = 0; Index < NumSegments_Cap + 2; ++Index)
    {
        const double CurrentAngle = FMath::DegreesToRadians(ArcSize * Index / NumSegments_Cap);
        const FVector2D EdgeDirection(FMath::Cos(CurrentAngle), FMath::Sin(CurrentAngle));
        const FVector2D OuterEdge(Radius * EdgeDirection);

        TempVertices.Add(FVector2D(0, 0) + FVector2D(OuterEdge.X, OuterEdge.Y));
    }

    Vertices = TempVertices;

    int32 TruncatedSize = UKismetMathLibrary::FTrunc(ArcSize);
    double Remainder = 0;
    UKismetMathLibrary::FMod(TruncatedSize, 2, Remainder);
    bool bIsEven = (Remainder / 2) == 0 ? true : false;
    EdgeTriangles = TruncatedSize + (bIsEven ? 3 : 4) + 2; // +2 is for inside triangles.   
}

bool UMeshOperationsBPLibrary::GenerateWave(bool bIsSin, double Amplitude, double RestHeight, double WaveLenght, TArray<FVector2D>& Out_Vertices, int32& EdgeTriangles)
{
    if (Amplitude > RestHeight)
    {
        UE_LOG(LogTemp, Warning, TEXT("Amplitude has to be bigger than Rest Height."));
        return false;
    }

    TArray<FVector2D> Vertices;
    int32 TruncatedLenght = UKismetMathLibrary::FTrunc(WaveLenght);
    for (int32 Index_Angles = 0; Index_Angles < TruncatedLenght; Index_Angles++)
    {
        double Value_Trigonometric = bIsSin ? UKismetMathLibrary::DegSin(Index_Angles) : UKismetMathLibrary::DegCos(Index_Angles);
        double Value_Y = (Value_Trigonometric * Amplitude) + RestHeight;
        FVector2D VertexLocation = FVector2D((double)Index_Angles, Value_Y);

        Vertices.Add(VertexLocation);
    }

    Algo::Reverse(Vertices);

    Vertices.Insert(FVector2D(0.f, 0.f), 0);           // Bottom start location.
    Vertices.Insert(FVector2D(WaveLenght, 0.f), 1);    // Bottom end location.
   
    Out_Vertices = Vertices;

    EdgeTriangles = (TruncatedLenght * 2) + 3;

    return true;
}

UStaticMesh* UMeshOperationsBPLibrary::GSM_Description(FName Mesh_Name, const TArray<FVector>& Vertices, const TArray<int32>& Indices, const TArray<FVector>& Normals, const TArray<FVector>& Tangents, const TArray<FVector2D>& UVs, bool bSupportRayTracing)
{
    if (Vertices.IsEmpty())
    {
        return nullptr;
    }

    // Create a new static mesh description.
    UStaticMeshDescription* StaticMeshDesc = UStaticMesh::CreateStaticMeshDescription();
   
    if (!StaticMeshDesc)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create StaticMeshDescription."));
        return nullptr;
    }

    FMeshDescriptionBuilder MeshDescBuilder;
    MeshDescBuilder.SetMeshDescription(&StaticMeshDesc->GetMeshDescription());
    MeshDescBuilder.EnablePolyGroups();
    MeshDescBuilder.SetNumUVLayers(1);

    // Create the base vertices.
    const int32 NumVertices = Vertices.Num();
    TArray<FVertexID> BaseVertexIDs;
    BaseVertexIDs.SetNum(NumVertices);

    for (int32 Index = 0; Index < NumVertices; Index++)
    {
        BaseVertexIDs[Index] = MeshDescBuilder.AppendVertex(Vertices[Index]);
    }

    // Create one polygon group (required for at least one material).
    const FPolygonGroupID PolygonGroup = MeshDescBuilder.AppendPolygonGroup();

    // Validate triangle indices.
    if (Indices.Num() % 3 != 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Triangle index count must be a multiple of 3."));
        return nullptr;
    }

    const int32 NumTriangles = Indices.Num() / 3;
    
    for (int32 Tri = 0; Tri < NumTriangles; Tri++)
    {
        TArray<FVertexInstanceID> TriangleVertexInstances;
        TriangleVertexInstances.Reserve(3);

        for (int32 Corner = 0; Corner < 3; Corner++)
        {
            const int32 IndexPos = Tri * 3 + Corner;
            const int32 VertexIndex = Indices[IndexPos];

            if (!BaseVertexIDs.IsValidIndex(VertexIndex))
            {
                UE_LOG(LogTemp, Error, TEXT("Invalid vertex index %d at triangle %d corner %d"), VertexIndex, Tri, Corner);
                return nullptr;
            }

            // Create a vertex instance for this triangle corner.
            FVertexInstanceID InstanceID = MeshDescBuilder.AppendInstance(BaseVertexIDs[VertexIndex]);

            // Set per-instance normal if provided.
            if (!Normals.IsEmpty() && Normals.IsValidIndex(VertexIndex))
            {
                MeshDescBuilder.SetInstanceNormal(InstanceID, Normals[VertexIndex]);
            }

            if (!Tangents.IsEmpty() && Tangents.IsValidIndex(VertexIndex))
            {
                // Use the corresponding normal if available; otherwise, use ZeroVector.
                FVector InstanceNormal = Normals.IsValidIndex(VertexIndex) ? Normals[VertexIndex] : FVector::ZeroVector;
                float TangentSign = 1.0f; // Default sign. Adjust if you have binormal data.
                MeshDescBuilder.SetInstanceTangentSpace(InstanceID, InstanceNormal, Tangents[VertexIndex], TangentSign);
            }

            if (!UVs.IsEmpty() && UVs.IsValidIndex(VertexIndex))
            {
                MeshDescBuilder.SetInstanceUV(InstanceID, UVs[VertexIndex], 0);
            }

            TriangleVertexInstances.Add(InstanceID);
        }

        // Use the underlying MeshDescription to create the polygon with vertex instances.
        StaticMeshDesc->GetMeshDescription().CreatePolygon(PolygonGroup, TriangleVertexInstances);
    }

    // Create a new UStaticMesh with the provided name.
    UStaticMesh* StaticMesh = NewObject<UStaticMesh>(GetTransientPackage(), Mesh_Name.IsNone() ? NAME_None : Mesh_Name, RF_Public | RF_Standalone);
    StaticMesh->GetStaticMaterials().Add(FStaticMaterial(nullptr, TEXT("DefaultMaterialSlot")));
    StaticMesh->bAllowCPUAccess = true;
    StaticMesh->NeverStream = true;
    StaticMesh->bSupportRayTracing = bSupportRayTracing;

    // Build parameters (for example, enabling simple collision).
    UStaticMesh::FBuildMeshDescriptionsParams MeshDescriptionsParams;
    MeshDescriptionsParams.bBuildSimpleCollision = true;

    // Build the mesh from the description.
    TArray<const FMeshDescription*> MeshDescriptions;
    MeshDescriptions.Emplace(&StaticMeshDesc->GetMeshDescription());
    StaticMesh->BuildFromMeshDescriptions(MeshDescriptions, MeshDescriptionsParams);
    
    StaticMesh->CreateBodySetup();
    StaticMesh->GetBodySetup()->CollisionTraceFlag = CTF_UseComplexAsSimple;
    StaticMesh->GetBodySetup()->InvalidatePhysicsData();
    StaticMesh->GetBodySetup()->CreatePhysicsMeshes();

    return StaticMesh;
}

UStaticMesh* UMeshOperationsBPLibrary::GSM_RenderData(FName Mesh_Name, const TArray<FVector>& Vertices, const TArray<int32>& Indices, const TArray<FVector>& Normals, const TArray<FVector>& Tangents, const TArray<FVector2D>& UVs, bool bSupportRayTracing)
{
    UStaticMesh* StaticMesh = NewObject<UStaticMesh>(GetTransientPackage(), Mesh_Name.IsNone() ? NAME_None : Mesh_Name, RF_Public | RF_Standalone);

    if (!StaticMesh)
    {
        return nullptr;
    }

    StaticMesh->bAllowCPUAccess = true;
    StaticMesh->NeverStream = true;
    StaticMesh->bSupportRayTracing = bSupportRayTracing;
    StaticMesh->GetStaticMaterials().Add(FStaticMaterial(nullptr, TEXT("DefaultMaterialSlot")));

    StaticMesh->SetRenderData(MakeUnique<FStaticMeshRenderData>());
    FStaticMeshRenderData* RenderData = StaticMesh->GetRenderData();

    if (!RenderData)
    {
        return nullptr;
    }

    const size_t NumElements = Indices.Num();
	TArray<uint32> U_Indices;
    U_Indices.SetNum(NumElements);

    ParallelFor(NumElements, [&Indices, &U_Indices](int32 Index)
        {
            U_Indices[Index] = static_cast<uint32>(Indices[Index]);
        }
    );

    RenderData->AllocateLODResources(1);
    FStaticMeshLODResources& LOD_Resource = RenderData->LODResources[0];

	// This allows us to use more than 65535 vertices.
    LOD_Resource.IndexBuffer.SetIndices(U_Indices, EIndexBufferStride::Force32Bit);

    // --- POSITION VERTEX BUFFER ---
  
    const int32 NumPositions = Vertices.Num();
    LOD_Resource.VertexBuffers.PositionVertexBuffer.Init(NumPositions);
    
    for (int32 PositionIndex = 0; PositionIndex < NumPositions; PositionIndex++)
    {
        LOD_Resource.VertexBuffers.PositionVertexBuffer.VertexPosition(PositionIndex) = (FVector3f)Vertices[PositionIndex];
    }

	// --- COLOR VERTEX BUFFER ---
    LOD_Resource.VertexBuffers.ColorVertexBuffer.InitFromSingleColor(FColor::White, NumPositions);

    // --- STATIC MESH VERTEX BUFFER ---

    uint32 NumVertices = Vertices.Num();
    LOD_Resource.VertexBuffers.StaticMeshVertexBuffer.SetUseFullPrecisionUVs(true);
    LOD_Resource.VertexBuffers.StaticMeshVertexBuffer.Init(NumVertices, 1);

    for (uint32 VertexIndex = 0; VertexIndex < NumVertices; VertexIndex++)
    {
        // Use input tangents and normals.
        FVector3f TangentF = (FVector3f)Tangents[VertexIndex];
        FVector3f NormalF = (FVector3f)Normals[VertexIndex];
        // Compute binormal (bitangent) from the cross product.
        FVector3f BinormalF = (FVector3f)(FVector::CrossProduct(Normals[VertexIndex], Tangents[VertexIndex])).GetSafeNormal();

        LOD_Resource.VertexBuffers.StaticMeshVertexBuffer.SetVertexTangents(VertexIndex, TangentF, BinormalF, NormalF);

        // Set the UV for channel 0.
        LOD_Resource.VertexBuffers.StaticMeshVertexBuffer.SetVertexUV(VertexIndex, 0, (FVector2f)UVs[VertexIndex]);
    }

    // --- MESH SECTIONS ---
    
    // Create one section covering the entire mesh.
    LOD_Resource.Sections.Empty();
    FStaticMeshSection NewSection;
    NewSection.FirstIndex = 0;
    NewSection.NumTriangles = Indices.Num() / 3;
    NewSection.MinVertexIndex = 0;
    NewSection.MaxVertexIndex = Vertices.Num() - 1;
    NewSection.MaterialIndex = 0;
    LOD_Resource.Sections.Add(NewSection);

    // --- BOUNDS ---
    FBox BoundingBox(Vertices);
    RenderData->Bounds = FBoxSphereBounds(BoundingBox);
   
#if RHI_RAYTRACING
    if (StaticMesh->bSupportRayTracing)
    {
        RenderData->InitializeRayTracingRepresentationFromRenderingLODs();
    }
#endif

    // Finalize render data.
    StaticMesh->InitResources();
    StaticMesh->CalculateExtendedBounds();

    // --- AGGREGATED COLLISION GEOMETRY (AggGeom) ---
    UBodySetup* BodySetup = NewObject<UBodySetup>(StaticMesh, NAME_None, RF_Public | RF_Standalone);
    StaticMesh->SetBodySetup(BodySetup);
    BodySetup->CollisionTraceFlag = CTF_UseSimpleAsComplex;
	BodySetup->bHasCookedCollisionData = true;

    // Clear any existing collision elements.
    BodySetup->AggGeom.BoxElems.Empty();
    BodySetup->AggGeom.SphereElems.Empty();
    BodySetup->AggGeom.SphylElems.Empty();
    BodySetup->AggGeom.ConvexElems.Empty();

    // 1. Create a box collision element from the bounding box.
    FKBoxElem BoxElem;
    FVector BoxCenter = BoundingBox.GetCenter();
    FVector BoxExtent = BoundingBox.GetExtent();
    BoxElem.Center = BoxCenter;
    BoxElem.X = BoxExtent.X * 2.0f;
    BoxElem.Y = BoxExtent.Y * 2.0f;
    BoxElem.Z = BoxExtent.Z * 2.0f;
    BodySetup->AggGeom.BoxElems.Add(BoxElem);

    // 2. Create a convex collision element using the input vertices.
    FKConvexElem ConvexElem;
    ConvexElem.VertexData = Vertices;
    ConvexElem.UpdateElemBox();
    BodySetup->AggGeom.ConvexElems.Add(ConvexElem);

    BodySetup->InvalidatePhysicsData();
    BodySetup->CreatePhysicsMeshes();

    return StaticMesh;
}

UStaticMesh* UMeshOperationsBPLibrary::GenerateStaticMesh(FName Mesh_Name, const TArray<FVector>& Vertices, const TArray<int32>& Indices, const TArray<FVector>& Normals, const TArray<FVector>& Tangents, const TArray<FVector2D>& UVs, bool bUseDescription, bool bSupportRayTracing)
{
    if (bUseDescription)
    {
       return UMeshOperationsBPLibrary::GSM_Description(Mesh_Name, Vertices, Indices, Normals, Tangents, UVs, bSupportRayTracing);
    }

    else
    {
		return UMeshOperationsBPLibrary::GSM_RenderData(Mesh_Name, Vertices, Indices, Normals, Tangents, UVs, bSupportRayTracing);
    }
}

void UMeshOperationsBPLibrary::DeleteEmptyRoots(USceneComponent* AssetRoot)
{
    // Initial while variable
    int32 ChildrenCount = AssetRoot->GetNumChildrenComponents();

    // Check if asset root has one direct child or not.
    while (ChildrenCount == 1)
    {
        // Array variable for children components.
        TArray<USceneComponent*> Children;

        // Get children components of asset root.
        AssetRoot->GetChildrenComponents(false, Children);

        // Get first and only child as middle parent.
        USceneComponent* MiddleParent = Children[0];

        // Get class name of middle parent.
        FString ClassName = MiddleParent->GetClass()->GetName();

        // If middle parent is a scene component, do operations.
        if (ClassName == TEXT("SceneComponent"))
        {
            // Create array variable for middle children.
            TArray<USceneComponent*> MiddleChildren;

            // Get middle children of middle parent.
            MiddleParent->GetChildrenComponents(false, MiddleChildren);

            // For Each Loop function for middle children. It attachs each middle child to asset root.
            for (int32 ChildID = 0; ChildID < MiddleChildren.Num(); ChildID++)
            {
                MiddleChildren[ChildID]->AttachToComponent(AssetRoot, FAttachmentTransformRules::KeepWorldTransform);
            }

            // Destroy middle parent after child attachment.
            MiddleParent->DestroyComponent(true);

            // Check asset root's children number after process. If it is equal one return to start.
            ChildrenCount = AssetRoot->GetNumChildrenComponents();
        }
    }
}

void UMeshOperationsBPLibrary::DEP_LevelActors(AActor* TargetActor)
{
#if WITH_EDITOR
    if (!IsValid(TargetActor))
    {
        return;
    }

    TArray<AActor*> AttachedActors;
    TargetActor->GetAttachedActors(AttachedActors, true, true);

    if (AttachedActors.IsEmpty())
    {
        return;
    }

    const FScopedTransaction Transaction(NSLOCTEXT("FFMesh", "DeleteEmptyParents", "Delete Empty Parent Actors"));

    bool bProcessFinished = false;
    TMap<AActor*, bool> AnyEmptyParentLeft;

    while (!bProcessFinished)
    {
        for (AActor* EachActor : AttachedActors)
        {
            if (!IsValid(EachActor))
            {
                continue;
            }

            AActor* MiddleParentActor = EachActor->GetAttachParentActor();
            if (!IsValid(MiddleParentActor))
            {
                continue;
            }

            TArray<AActor*> Siblings;
            MiddleParentActor->GetAttachedActors(Siblings, true, false);

            const bool bShouldDelete = EachActor->IsA<AStaticMeshActor>() && Siblings.Num() == 1;
            if (!bShouldDelete)
            {
                continue;
            }

            AActor* GrandParent = MiddleParentActor->GetAttachParentActor();

            if (!IsValid(GrandParent))
            {
                // Middle parent is a top-level actor (e.g. TargetActor itself) with
                // nowhere to promote into — leave it as the hierarchy root.
                continue;
            }

            // The Outliner shows the label, so that's the name to carry over.
            const FString NewMeshLabel = MiddleParentActor->GetActorLabel();

            EachActor->Modify();
            MiddleParentActor->Modify();

            // Promote first, then delete the now-childless grouping actor.
            EachActor->AttachToActor(GrandParent, FAttachmentTransformRules::KeepWorldTransform);
            EachActor->SetActorLabel(NewMeshLabel);
            MiddleParentActor->Destroy();

            TArray<AActor*> GrandParentChildren;
            GrandParent->GetAttachedActors(GrandParentChildren, true, false);
            if (GrandParentChildren.Num() == 1)
            {
                AnyEmptyParentLeft.Add(EachActor, true);
            }
        }

        TArray<bool> BoolValues;
        AnyEmptyParentLeft.GenerateValueArray(BoolValues);

        if (BoolValues.Contains(true))
        {
            AnyEmptyParentLeft.GenerateKeyArray(AttachedActors);
            AnyEmptyParentLeft.Empty();
            bProcessFinished = false;
        }

        else
        {
            AnyEmptyParentLeft.Empty();
            bProcessFinished = true;
        }
    }
#endif
}

void UMeshOperationsBPLibrary::DEP_Components_Runtime(USceneComponent* AssetRoot)
{   
    UObject* Owner = AssetRoot->GetOwner();

    TArray<USceneComponent*> ChildrenComps;
    AssetRoot->GetChildrenComponents(true, ChildrenComps);

    if (ChildrenComps.IsEmpty())
    {
        return;
    }

    bool IsProcessFinished = false;
    TMap<USceneComponent*, bool> AnyEmptyParentLeft;

    while (!IsProcessFinished)
    {
        for (USceneComponent* EachChild : ChildrenComps)
        {
			USceneComponent* MiddleParent = EachChild->GetAttachParent();
			const bool bShouldDeleted = (EachChild->GetClass() == UStaticMeshComponent::StaticClass()) && MiddleParent->GetNumChildrenComponents() == 1;

            if (!bShouldDeleted)
            {
                continue;
            }

            USceneComponent* GrandParent = MiddleParent->GetAttachParent();

            // We will use this name as static mesh's new name.
            const FString New_SMC_Name = MiddleParent->GetName();

            // Change middle parent's name and destroy it.
            FGuid MiddleParentSuffix = FGuid::NewGuid();
            const FString MiddleParentName = TEXT("DeletedSceneComp") + MiddleParentSuffix.ToString();
            MiddleParent->Rename(*MiddleParentName, Owner);
            MiddleParent->DestroyComponent(false);

            // Change target static mesh name.
            EachChild->AttachToComponent(GrandParent, FAttachmentTransformRules::KeepWorldTransform, NAME_None);
            EachChild->Rename(*New_SMC_Name, Owner);

            // Check if there is any empty parent left.
            if (EachChild->GetAttachParent()->GetNumChildrenComponents() == 1)
            {
                AnyEmptyParentLeft.Add(EachChild, true);
            }
        }

        // If there is an empty parent, get related component and start loop again. 
        TArray<bool> Array_Bool_Values;
        AnyEmptyParentLeft.GenerateValueArray(Array_Bool_Values);

        if (Array_Bool_Values.Contains(true))
        {
            AnyEmptyParentLeft.GenerateKeyArray(ChildrenComps);
            AnyEmptyParentLeft.Empty();
            IsProcessFinished = false;
        }

        else
        {
            AnyEmptyParentLeft.Empty();
            IsProcessFinished = true;
        }
    }
}

void UMeshOperationsBPLibrary::OptimizeCenter(USceneComponent* AssetRoot)
{
    if (!IsValid(AssetRoot))
    {
        return;
    }

    FVector Origin, Extent;
    AssetRoot->GetOwner()->GetActorBounds(true, Origin, Extent, true);
    const FVector ActorLocation = AssetRoot->GetOwner()->GetActorLocation();
    const FVector Offset = ActorLocation - Origin;

    TArray<USceneComponent*> Children;
    AssetRoot->GetChildrenComponents(false, Children);

    for (USceneComponent* Each_Child : Children)
    {
        Each_Child->AddRelativeLocation(Offset, false, nullptr, ETeleportType::None);
    }

#if WITH_EDITOR
    AActor* OwnerActor = AssetRoot->GetOwner();
    if (UBlueprint* Blueprint = UBlueprint::GetBlueprintFromClass(OwnerActor->GetClass()))
    {
        if (USimpleConstructionScript* SCS = Blueprint->SimpleConstructionScript)
        {
            Blueprint->Modify();

            for (USceneComponent* Each_Child : Children)
            {
                USCS_Node* Node = SCS->FindSCSNode(Each_Child->GetFName());
                if (!Node)
                {
                    UE_LOG(LogTemp, Warning, TEXT("OptimizeCenter: no SCS node for '%s' (native or renamed component?), template left unchanged."), *Each_Child->GetName());
                    continue;
                }

                if (USceneComponent* Template = Cast<USceneComponent>(Node->ComponentTemplate))
                {
                    Template->Modify();
                    Template->SetRelativeLocation_Direct(Each_Child->GetRelativeLocation());
                }
            }

            FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
            Blueprint->MarkPackageDirty();
        }
    }
#endif
}

void UMeshOperationsBPLibrary::OptimizeHeight(USceneComponent* AssetRoot, float Z_Offset)
{
    FVector Origin;
    FVector BoxExtent;
    AssetRoot->GetOwner()->GetActorBounds(false, Origin, BoxExtent, true);

    float NewHeight = BoxExtent.Z + Z_Offset;
    FVector NewLocation(0.f, 0.f, NewHeight);

    AssetRoot->AddWorldOffset(NewLocation, false, nullptr, ETeleportType::None);
}

void UMeshOperationsBPLibrary::RecordTransforms(USceneComponent* AssetRoot, TMap<USceneComponent*, FTransform>& MapTransform, TArray<USceneComponent*>& AllComponents, TArray<USceneComponent*>& ChildComponents)
{
    AssetRoot->GetChildrenComponents(true, ChildComponents);
    AllComponents = ChildComponents;
    AllComponents.Add(AssetRoot);

    for (int32 ChildID = 0; ChildID < AllComponents.Num(); ChildID++)
    {
        MapTransform.Add(AllComponents[ChildID], AllComponents[ChildID]->GetRelativeTransform());
    }
}

bool UMeshOperationsBPLibrary::RenameComponent(UPARAM(ref)UObject* Target, UObject* Outer, FName NewName)
{
    if (IsValid(Target) == false)
    {
        return false;
    }

    Target->Rename(*(NewName.ToString()), Outer);
    return true;
}

bool UMeshOperationsBPLibrary::ExportLevelGLTF(FGLTFExportMessages& OutMessages, FGLTFExportOptionsStruct Options, FString ExportPath, TSet<AActor*> TargetActors)
{
	FPaths::NormalizeFilename(ExportPath);
	const FString ExportDirectory = FPaths::GetPath(ExportPath);

    if (!FPaths::DirectoryExists(ExportDirectory))
    {
        return false;
    }
    
    UWorld* CurrentWorld = GEngine->GetCurrentPlayWorld();

    if (!CurrentWorld)
    {
        return false;
    }

    TArray<FVector> Array_Locations;
    TArray<FVector> Array_Scales;

    for (AActor* Actor : TargetActors)
    {
		USceneComponent* RootComp = Actor->GetRootComponent();

        if (Options.bResetLocation)
        {
            Array_Locations.Add(RootComp->GetComponentLocation());
            RootComp->SetWorldLocation(FVector(0.0f), false, nullptr, ETeleportType::None);
        }

        if (Options.bResetScale)
        {
            Array_Scales.Add(RootComp->GetComponentScale());
            RootComp->SetWorldScale3D(FVector(1.0f));
        }
    }

    UGLTFExportOptions* ExportOptions = NewObject<UGLTFExportOptions>();
    ExportOptions->ResetToDefault();
   
    // --- General Options ---
    ExportOptions->ExportUniformScale = Options.ExportUniformScale;
    ExportOptions->bExportPreviewMesh = Options.bExportPreviewMesh;
    ExportOptions->bSkipNearDefaultValues = Options.bSkipNearDefaultValues;
    ExportOptions->bIncludeCopyrightNotice = Options.bIncludeCopyrightNotice;
    ExportOptions->bExportProxyMaterials = Options.bExportProxyMaterials;
    ExportOptions->bUseImporterMaterialMapping = Options.bUseImporterMaterialMapping;

    // --- Material Shading Model Options ---
    ExportOptions->bExportUnlitMaterials = Options.bExportUnlitMaterials;
    ExportOptions->bExportClearCoatMaterials = Options.bExportClearCoatMaterials;
    ExportOptions->bExportClothMaterials = Options.bExportClothMaterials;
    ExportOptions->bExportThinTranslucentMaterials = Options.bExportThinTranslucentMaterials;
    ExportOptions->bExportSpecularGlossinessMaterials = Options.bExportSpecularGlossinessMaterials;
    ExportOptions->bExportEmissiveStrength = Options.bExportEmissiveStrength;

    // --- Material Bake Settings ---
    ExportOptions->BakeMaterialInputs = Options.BakeMaterialInputs;
    ExportOptions->DefaultMaterialBakeSize = Options.DefaultMaterialBakeSize;
    ExportOptions->DefaultMaterialBakeFilter = Options.DefaultMaterialBakeFilter;
    ExportOptions->DefaultMaterialBakeTiling = Options.DefaultMaterialBakeTiling;
    ExportOptions->DefaultInputBakeSettings = Options.DefaultInputBakeSettings;

    // --- Mesh Options ---
    ExportOptions->DefaultLevelOfDetail = Options.DefaultLevelOfDetail;
    ExportOptions->bExportSourceModel = Options.bExportSourceModel;
    ExportOptions->bExportVertexColors = Options.bExportVertexColors;
    ExportOptions->bExportVertexSkinWeights = Options.bExportVertexSkinWeights;
    ExportOptions->bMakeSkinnedMeshesRoot = Options.bMakeSkinnedMeshesRoot;
    ExportOptions->bUseMeshQuantization = Options.bUseMeshQuantization;
    ExportOptions->bExportMorphTargets = Options.bExportMorphTargets;

    // --- Animation Options ---
    ExportOptions->bExportLevelSequences = Options.bExportLevelSequences;
    ExportOptions->bExportAnimationSequences = Options.bExportAnimationSequences;

    // --- Texture Options ---
    ExportOptions->TextureImageFormat = Options.TextureImageFormat;
    ExportOptions->TextureImageQuality = Options.TextureImageQuality;
    ExportOptions->bExportTextureTransforms = Options.bExportTextureTransforms;
    ExportOptions->bExportLightmaps = Options.bExportLightmaps;
    ExportOptions->bAdjustNormalmaps = Options.bAdjustNormalmaps;

    // --- Scene & Component Options ---
    ExportOptions->bExportHiddenInGame = Options.bExportHiddenInGame;
    ExportOptions->bExportLights = Options.bExportLights;
    ExportOptions->bExportCameras = Options.bExportCameras;
    ExportOptions->ExportMaterialVariants = Options.ExportMaterialVariants;

    const bool bIsExportSuccessful = UGLTFExporter::ExportToGLTF(CurrentWorld, ExportPath, ExportOptions, TargetActors, OutMessages);

    for (int32 ActorIndex = 0; ActorIndex < TargetActors.Num(); ActorIndex++)
    {
		USceneComponent* RootComp = TargetActors.Array()[ActorIndex]->GetRootComponent();

        if (Options.bResetLocation)
        {
            RootComp->SetWorldLocation(Array_Locations[ActorIndex], false, nullptr, ETeleportType::None);
        }

        if (Options.bResetScale)
        {
            RootComp->SetWorldScale3D(Array_Scales[ActorIndex]);
        }
    }

	return bIsExportSuccessful;
}

bool UMeshOperationsBPLibrary::GetVerticesTransforms(TArray<FTransform>& Out_Transform, UStaticMeshComponent* In_SMC, int32 LOD_Index, bool bUseRelativeLocation)
{
    if (!IsValid(In_SMC))
    {
        return false;
    }

    FStaticMeshRenderData* RenderData = In_SMC->GetStaticMesh()->GetRenderData();

    if (!RenderData)
    {
        return false;
    }

    const FVector Center_World = In_SMC->Bounds.Origin;
    const FVector Center_Rel = Center_World - In_SMC->GetComponentLocation();

    FPositionVertexBuffer* PositionVertexBuffer = &RenderData->LODResources[LOD_Index].VertexBuffers.PositionVertexBuffer;

	const size_t NumVertices = PositionVertexBuffer->GetNumVertices();
    const FTransform ComponentTransform = In_SMC->GetComponentTransform();

	TArray<FTransform> Temp_Transform;

    for (size_t VertexIndex = 0; VertexIndex < NumVertices; VertexIndex++)
    {
        FVector VertexLocation;
        FRotator VertexDirection;
        FTransform Vertex_Transform;

		if (bUseRelativeLocation)
		{
            VertexLocation = (FVector)PositionVertexBuffer->VertexPosition(VertexIndex);
			VertexDirection = UMeshOperationsBPLibrary::GetDirectionOfVector(Center_Rel, VertexLocation);
			
			Vertex_Transform.SetLocation(VertexLocation);
            Vertex_Transform.SetRotation(VertexDirection.Quaternion());
		}

		else
		{
            VertexLocation = ComponentTransform.TransformPosition((FVector)PositionVertexBuffer->VertexPosition(VertexIndex));
			VertexDirection = UMeshOperationsBPLibrary::GetDirectionOfVector(Center_World, VertexLocation);

            Vertex_Transform.SetLocation(VertexLocation);
            Vertex_Transform.SetRotation(VertexDirection.Quaternion());
		}

        Temp_Transform.Add(Vertex_Transform);
    }

    Out_Transform = Temp_Transform;

    return true;
}

bool UMeshOperationsBPLibrary::SetPivotLocation(UPARAM(ref) UStaticMeshComponent*& In_SMC, FVector PivotLocation, UObject* Outer)
{
    if (!IsValid(In_SMC))
    {
        return false;
    }

    UStaticMesh* StaticMesh = In_SMC->GetStaticMesh();

    if (!StaticMesh)
    {
        return false;
    }

    StaticMesh->bAllowCPUAccess = true;

    FStaticMeshRenderData* RenderData = StaticMesh->GetRenderData();

    if (!RenderData)
    {
        return false;
    }

    FStaticMeshLODResources& LOD = RenderData->LODResources[0];
    const int32 NumVerts = LOD.VertexBuffers.PositionVertexBuffer.GetNumVertices();
    const int32 NumUVs = LOD.VertexBuffers.StaticMeshVertexBuffer.GetNumTexCoords();
	const FVector PivotDelta = (PivotLocation - In_SMC->GetComponentLocation()) * (-1);
    const FVector3f PivotDelta_3f = (FVector3f)(PivotDelta);
    
    FMeshDescription MeshDesc;
    FStaticMeshAttributes Attributes(MeshDesc);
    Attributes.Register();

    auto Positions = Attributes.GetVertexPositions();
    auto Normals = Attributes.GetVertexInstanceNormals();
    auto Tangents = Attributes.GetVertexInstanceTangents();
    auto BinormalSigns = Attributes.GetVertexInstanceBinormalSigns();
    auto VColors = Attributes.GetVertexInstanceColors();
    auto UVs = Attributes.GetVertexInstanceUVs();
    UVs.SetNumChannels(NumUVs);

    TArray<FVertexID> VertexIDs;
    VertexIDs.SetNum(NumVerts);
    MeshDesc.ReserveNewVertices(NumVerts);
    
    for (int32 v = 0; v < NumVerts; ++v)
    {
        VertexIDs[v] = MeshDesc.CreateVertex();
        Positions[VertexIDs[v]] = LOD.VertexBuffers.PositionVertexBuffer.VertexPosition(v) + PivotDelta_3f;
    }

    // One polygon group per section preserves material slot mapping.
    FIndexArrayView Indices = LOD.IndexBuffer.GetArrayView();
    for (int32 s = 0; s < LOD.Sections.Num(); ++s)
    {
        const FStaticMeshSection& Sec = LOD.Sections[s];
        const FPolygonGroupID PG = MeshDesc.CreatePolygonGroup();
        Attributes.GetPolygonGroupMaterialSlotNames()[PG] = StaticMesh->GetStaticMaterials()[Sec.MaterialIndex].MaterialSlotName;

        for (uint32 tri = 0; tri < Sec.NumTriangles; ++tri)
        {
            const uint32 i0 = Indices[Sec.FirstIndex + tri * 3 + 0];
            const uint32 i1 = Indices[Sec.FirstIndex + tri * 3 + 1];
            const uint32 i2 = Indices[Sec.FirstIndex + tri * 3 + 2];

            FVertexInstanceID VI[3];
            const uint32 Corner[3] = { i0, i1, i2 };
            
            for (int32 c = 0; c < 3; ++c)
            {
                VI[c] = MeshDesc.CreateVertexInstance(VertexIDs[Corner[c]]);
                const FVector4f Tx = LOD.VertexBuffers.StaticMeshVertexBuffer.VertexTangentX(Corner[c]);
                const FVector4f Tz = LOD.VertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(Corner[c]);
                Normals[VI[c]] = FVector3f(Tz);
                Tangents[VI[c]] = FVector3f(Tx);
                
                // handedness carried in W
                BinormalSigns[VI[c]] = Tz.W;
               
                for (int32 uv = 0; uv < NumUVs; ++uv)
                {
					UVs.Set(VI[c], uv, LOD.VertexBuffers.StaticMeshVertexBuffer.GetVertexUV(Corner[c], uv));
                }
               
                if (LOD.VertexBuffers.ColorVertexBuffer.GetNumVertices() > 0)
                {
                    const FColor SrcColor = LOD.VertexBuffers.ColorVertexBuffer.VertexColor(Corner[c]);
                    VColors[VI[c]] = FVector4f(SrcColor.ReinterpretAsLinear());
                }
            }

            MeshDesc.CreatePolygon(PG, { VI[0], VI[1], VI[2] });
        }
    }

    UStaticMesh::FBuildMeshDescriptionsParams Params;
    Params.bFastBuild = true;
    Params.bAllowCpuAccess = true;
    
    // we bring our own
    Params.bBuildSimpleCollision = false;

    TArray<const FMeshDescription*> MeshDescriptions;
    MeshDescriptions.Add(&MeshDesc);

    UStaticMesh* NewStaticMesh = NewObject<UStaticMesh>(Outer ? Outer : GetTransientPackage(), NAME_None, RF_Public);
    NewStaticMesh->SetStaticMaterials(StaticMesh->GetStaticMaterials());
    NewStaticMesh->bAllowCPUAccess = true;
    NewStaticMesh->NeverStream = true;
    NewStaticMesh->bSupportRayTracing = StaticMesh->bSupportRayTracing;
    NewStaticMesh->BuildFromMeshDescriptions(MeshDescriptions, Params);

    NewStaticMesh->CreateBodySetup();
    UBodySetup* Dst = NewStaticMesh->GetBodySetup();
    UBodySetup* Src = StaticMesh->GetBodySetup();

    Dst->CollisionTraceFlag = Src->CollisionTraceFlag;
    
    // copy, then offset into new pivot space
    Dst->AggGeom = Src->AggGeom;

    for (FKBoxElem& Box : NewStaticMesh->GetBodySetup()->AggGeom.BoxElems)
    {
        Box.Center += PivotDelta;
    }

    for (FKSphereElem& Sphere : NewStaticMesh->GetBodySetup()->AggGeom.SphereElems)
    {
        Sphere.Center += PivotDelta;
    }

    for (FKSphylElem& Sphyle : NewStaticMesh->GetBodySetup()->AggGeom.SphylElems)
    {
        Sphyle.Center += PivotDelta;
    }

    for (FKConvexElem& Convex : NewStaticMesh->GetBodySetup()->AggGeom.ConvexElems)
    {
        for (FVector& Vertex : Convex.VertexData)
        {
            Vertex += PivotDelta;
        }

        Convex.UpdateElemBox();
    }

    Dst->InvalidatePhysicsData();
    Dst->CreatePhysicsMeshes();

	In_SMC->SetStaticMesh(NewStaticMesh);
    In_SMC->AddWorldOffset(PivotLocation - In_SMC->Bounds.Origin);
	return true;
}

bool UMeshOperationsBPLibrary::MovePivotsToCenter(USceneComponent* RootComponent, TArray<FString>& ErroredMeshes)
{
    if (!IsValid(RootComponent))
    {
        return false;
    }

    TArray<USceneComponent*> Array_Children;
    RootComponent->GetChildrenComponents(true, Array_Children);

    for (USceneComponent* Each_Child : Array_Children)
    {
        UStaticMeshComponent* Each_Mesh = Cast<UStaticMeshComponent>(Each_Child);

        if (IsValid(Each_Mesh) == true)
        {
            if (!UMeshOperationsBPLibrary::SetPivotLocation(Each_Mesh, Each_Mesh->Bounds.Origin, Each_Mesh))
            {
                const FString Error_Mesh = UMeshOperationsBPLibrary::GetObjectNameForPackage(Each_Mesh);
                ErroredMeshes.Add(Error_Mesh);
            }
        }
    }

    return true;
}

FRotator UMeshOperationsBPLibrary::GetDirectionOfVector(const FVector& Start, const FVector& End)
{
    const FVector NormDirection = FVector(End).GetSafeNormal();
    const FVector NormUp = FVector(Start).GetSafeNormal();
    return FRotationMatrix::MakeFromXZ(NormDirection, NormUp).Rotator();
}

void UMeshOperationsBPLibrary::GetSceneComponentBounds(FVector& Out_Origin, FVector& Out_Extent, USceneComponent* SceneComponent)
{
    if (!IsValid(SceneComponent))
    {
        Out_Extent = FVector::ZeroVector;
        Out_Origin = FVector::ZeroVector;
        return;
    }

	TArray<USceneComponent*> Children;
	SceneComponent->GetChildrenComponents(true, Children);

    FBox TotalBox(ForceInit);

    for (USceneComponent* Each_Child : Children)
    {
        if (UPrimitiveComponent* Geometry = Cast<UPrimitiveComponent>(Each_Child))
        {
			TotalBox += Geometry->Bounds.GetBox();
        }
    }

    Out_Origin = TotalBox.GetCenter();
    Out_Extent = TotalBox.GetExtent();
}

bool UMeshOperationsBPLibrary::IsInBounds(USceneComponent* Target_Comp, FVector In_Origin, FVector In_Extent)
{
    if (!IsValid(Target_Comp))
    {
        return false;
    }

    FBox MyBox = FBox::BuildAABB(In_Origin, In_Extent);
	const FVector ComponentLocation = Target_Comp->GetComponentLocation();

	return MyBox.IsInside(ComponentLocation);
}

void UMeshOperationsBPLibrary::Check_Assembly(USceneComponent* Target_Root, bool bSpawnBillboardForOnlyProblems)
{
    if (!IsValid(Target_Root))
    {
        return;
    }

	FVector Origin;
	FVector Extent;
    UMeshOperationsBPLibrary::GetSceneComponentBounds(Origin, Extent, Target_Root);

	UBoxComponent* BoxComp = NewObject<UBoxComponent>(Target_Root->GetOuter());
	BoxComp->SetMobility(EComponentMobility::Movable);
	BoxComp->SetHiddenInGame(false);
	BoxComp->SetLineThickness(10.0f);
	BoxComp->SetWorldLocation(Origin);
	BoxComp->SetBoxExtent(Extent);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BoxComp->AttachToComponent(Target_Root, FAttachmentTransformRules::KeepWorldTransform);
	BoxComp->RegisterComponent();

	TArray<USceneComponent*> All_Components;
	Target_Root->GetChildrenComponents(true, All_Components);
	All_Components.Add(Target_Root);

	auto BillboardCallback = [Target_Root](USceneComponent* Component, double Size = 0.25)
		{
            if (!IsValid(Component))
            {
                return;
            }

			UBillboardComponent* Billboard = NewObject<UBillboardComponent>(Component->GetOuter());
			Billboard->SetMobility(EComponentMobility::Movable);
			Billboard->SetHiddenInGame(false);
			Billboard->SetWorldLocation(Component->GetComponentLocation());
			Billboard->SetWorldScale3D(FVector(Size));
			Billboard->AttachToComponent(Target_Root, FAttachmentTransformRules::KeepWorldTransform);
			Billboard->RegisterComponent();
		};

    for (USceneComponent* Each_Component : All_Components)
    {
		if (!UMeshOperationsBPLibrary::IsInBounds(Each_Component, Origin, Extent))
        {
			UE_LOG(LogTemp, Warning, TEXT("%s is out of bounds of the assembly."), *UMeshOperationsBPLibrary::GetObjectNameForPackage(Each_Component));

            if (bSpawnBillboardForOnlyProblems)
            {
                BillboardCallback(Each_Component, 2.0);
            }
        }

        if (!bSpawnBillboardForOnlyProblems)
        {
			BillboardCallback(Each_Component);
        }
    }
}

bool UMeshOperationsBPLibrary::ChangeMaterialInstanceParent(UMaterialInstanceDynamic* MaterialInstance, UMaterialInterface* NewParent)
{
    if (!IsValid(MaterialInstance))
    {
        return false;
    }

    if (!IsValid(NewParent))
    {
        return false;
    }

	MaterialInstance->Parent = NewParent;

#if WITH_EDITORONLY_DATA
	MaterialInstance->PostEditChange();
#endif

    MaterialInstance->RecacheUniformExpressions(true);

    FMaterialUpdateContext MaterialUpdateContext;
    MaterialUpdateContext.AddMaterialInstance(MaterialInstance);
	return true;
}