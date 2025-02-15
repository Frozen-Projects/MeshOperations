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

bool UMeshOperationsBPLibrary::GetObjectNameForPackage(USceneComponent* Object, bool bUseReadableName, FString Delimeter, FString& OutName)
{
    if (IsValid(Object) == false)
    {
        return false;
    }
    
    FString ObjectName;
    if (bUseReadableName == true)
    {
        ObjectName = Object->GetReadableName();
    }

    else
    {
        ObjectName = Object->GetName();
    }

    FString GeneratedName;
    TArray<FString> NameSections;
    ObjectName.ParseIntoArray(NameSections, *Delimeter, true);

    if (NameSections.Num() > 1)
    {
        if (WITH_EDITOR == true)
        {
            NameSections.RemoveAt(NameSections.Num() - 1);
            GeneratedName = FString::Join(NameSections, *Delimeter);
        }

        else
        {
            for (int32 SectionID = 0; SectionID < 1; SectionID++)
            {
                NameSections.RemoveAt(NameSections.Num() - 1 - SectionID);
            }

            GeneratedName = FString::Join(NameSections, *Delimeter);
        }

        OutName = GeneratedName;

        return true;
    }

    else
    {
        return false;
    }
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

bool UMeshOperationsBPLibrary::AddStaticMeshCompWithName(UStaticMeshComponent*& Out_Comp, FName& Out_Name, AActor* Outer, FTransform RelativeTransform, FName InName, bool Manual_Attachment, EAttachmentRule Attachment_Rule, EComponentMobility::Type Mobility)
{
    if (!IsValid(Outer))
    {
        Out_Comp = nullptr;
        Out_Name = NAME_None;
        return false;
    }

	UStaticMeshComponent* StaticMeshComp = NewObject<UStaticMeshComponent>(Outer, InName.ToString().IsEmpty() ? NAME_None : InName);

    if (StaticMeshComp == nullptr)
    {
        return false;
    }

    //Set Mobility of Static Mesh Component.
    StaticMeshComp->SetMobility(Mobility);

    //Render Static Mesh Component.
    StaticMeshComp->RegisterComponent();

    //Get Root Component.
    USceneComponent* ActorRootForSMC = Outer->GetRootComponent();

    //Create Attachment Rules.
    StaticMeshComp->AttachToComponent(ActorRootForSMC, FAttachmentTransformRules(Attachment_Rule, true));

    if (Manual_Attachment == true)
    {
        StaticMeshComp->SetRelativeTransform(RelativeTransform);
    }

    //Output Pins.
    Out_Comp = StaticMeshComp;
    Out_Name = InName;

    return true;
}

bool UMeshOperationsBPLibrary::AddSceneCompWithName(USceneComponent*& Out_Comp, FName& Out_Name, AActor* Outer, FTransform RelativeTransform, FName InName, bool Manual_Attachment, EAttachmentRule Attachment_Rule, EComponentMobility::Type Mobility)
{
    if (!IsValid(Outer))
    {
        Out_Comp = nullptr;
        Out_Name = NAME_None;
        return false;
    }

    USceneComponent* SceneComp = NewObject<USceneComponent>(Outer, InName.ToString().IsEmpty() ? NAME_None : InName);

    if (SceneComp == nullptr)
    {
        return false;
    }

    //Set Mobility of Static Mesh Component.
    SceneComp->SetMobility(Mobility);

    //Get Root Component.
    USceneComponent* ActorRootForSC = Outer->GetRootComponent();

    //Create Attachment Rules.
    SceneComp->AttachToComponent(ActorRootForSC, FAttachmentTransformRules(Attachment_Rule, true));

    if (Manual_Attachment == true)
    {
        //Set Realtive Transform.
        SceneComp->SetRelativeTransform(RelativeTransform);
    }

    Out_Comp = SceneComp;
    Out_Name = InName;
    return true;
}

bool UMeshOperationsBPLibrary::AddProcMeshCompWithName(UProceduralMeshComponent*& Out_Comp, FName& Out_Name, AActor* Outer, FName InName, EAttachmentRule Attachment_Rule, bool Manual_Attachment, bool bUseAsyncCooking, bool bUseComplexCollisionAsSimple, FTransform Relative_Transform, EComponentMobility::Type Mobility)
{
    if (!IsValid(Outer))
    {
        Out_Comp = nullptr;
        Out_Name = NAME_None;
        return false;
    }

    UProceduralMeshComponent* ProcMeshComp = NewObject<UProceduralMeshComponent>(Outer, InName.ToString().IsEmpty() ? NAME_None : InName);

    if (ProcMeshComp == nullptr)
    {
        return false;
    }

    //Set Mobility of Static Mesh Component.
    ProcMeshComp->SetMobility(Mobility);

    ProcMeshComp->bUseAsyncCooking = bUseAsyncCooking;
    ProcMeshComp->bUseComplexAsSimpleCollision = bUseComplexCollisionAsSimple;
    ProcMeshComp->RegisterComponent();
    ProcMeshComp->AttachToComponent(Outer->GetRootComponent(), FAttachmentTransformRules(Attachment_Rule, true));

    if (Manual_Attachment == true)
    {
        //Set Realtive Transform.
        ProcMeshComp->SetRelativeTransform(Relative_Transform);
    }

    //Output Pins
    Out_Comp = ProcMeshComp;
    Out_Name = InName;

    return true;
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

bool UMeshOperationsBPLibrary::GenerateMeshFromVertices(UStaticMesh*& Out_Mesh, TArray<FVector> In_Vertices, TArray<FVector> In_Normals, TArray<FVector2D> In_UVs, TArray<int32> In_Tris)
{
    if (In_Vertices.IsEmpty())
    {
        return false;
    }

    // Create a new static mesh description.
    UStaticMeshDescription* StaticMeshDesc = UStaticMesh::CreateStaticMeshDescription();
    if (!StaticMeshDesc)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create StaticMeshDescription."));
        return false;
    }

    FMeshDescriptionBuilder MeshDescBuilder;
    MeshDescBuilder.SetMeshDescription(&StaticMeshDesc->GetMeshDescription());
    MeshDescBuilder.EnablePolyGroups();
    MeshDescBuilder.SetNumUVLayers(1);

    // Create vertices and vertex instances.
    const int32 NumVertices = In_Vertices.Num();
    TArray<FVertexInstanceID> VertexInstances;
    VertexInstances.SetNum(NumVertices);

    for (int32 Index = 0; Index < NumVertices; Index++)
    {
        // Append a vertex and create its instance.
        const FVertexID VertexID = MeshDescBuilder.AppendVertex(In_Vertices[Index]);
        const FVertexInstanceID Instance = MeshDescBuilder.AppendInstance(VertexID);
        VertexInstances[Index] = Instance;

        // Set per-instance normals if provided.
        if (!In_Normals.IsEmpty() && In_Normals.IsValidIndex(Index))
        {
            MeshDescBuilder.SetInstanceNormal(Instance, In_Normals[Index]);
        }

        // Set per-instance UVs if provided.
        if (!In_UVs.IsEmpty() && In_UVs.IsValidIndex(Index))
        {
            MeshDescBuilder.SetInstanceUV(Instance, In_UVs[Index], 0);
        }
    }

    // Create one polygon group (needed for at least one material).
    const FPolygonGroupID PolygonGroup = MeshDescBuilder.AppendPolygonGroup();

    // Validate and create triangles (polygons) from the provided indices.
    if (In_Tris.Num() % 3 != 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Triangle index count must be a multiple of 3."));
        return false;
    }

    const int32 NumTriangles = In_Tris.Num() / 3;
    for (int32 Tri = 0; Tri < NumTriangles; Tri++)
    {
        TArray<FVertexID> TriangleVertexInstances;
        TriangleVertexInstances.Add(VertexInstances[In_Tris[Tri * 3 + 0]]);
        TriangleVertexInstances.Add(VertexInstances[In_Tris[Tri * 3 + 1]]);
        TriangleVertexInstances.Add(VertexInstances[In_Tris[Tri * 3 + 2]]);

        MeshDescBuilder.AppendPolygon(TriangleVertexInstances, PolygonGroup);
    }

    // Create a new UStaticMesh and add at least one material.
    UStaticMesh* StaticMesh = NewObject<UStaticMesh>();
    StaticMesh->GetStaticMaterials().Add(FStaticMaterial());

    // Build parameters (e.g., enabling simple collision).
    UStaticMesh::FBuildMeshDescriptionsParams MeshDescriptionsParams;
    MeshDescriptionsParams.bBuildSimpleCollision = true;

    // Build the mesh from the description.
    TArray<const FMeshDescription*> MeshDescriptions;
    MeshDescriptions.Emplace(&StaticMeshDesc->GetMeshDescription());
    StaticMesh->BuildFromMeshDescriptions(MeshDescriptions, MeshDescriptionsParams);

    Out_Mesh = StaticMesh;
    return true;
}

UStaticMesh* UMeshOperationsBPLibrary::GenerateStaticMesh(FString Mesh_Name, const TArray<FVector>& Vertices, const TArray<int32>& Indices, const TArray<FVector>& Normals, const TArray<FVector>& Tangents, const TArray<FVector2D>& UVs)
{
    UStaticMesh* StaticMesh = NewObject<UStaticMesh>(GetTransientPackage(), Mesh_Name.IsEmpty() ? NAME_None : (FName)Mesh_Name, RF_Public | RF_Standalone);

    if (!StaticMesh)
    {
        return nullptr;
    }

    StaticMesh->bAllowCPUAccess = true;
    StaticMesh->NeverStream = true;
    StaticMesh->bSupportRayTracing = false;

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

	// We assume one LOD.
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

    // --- STATIC MESH VERTEX BUFFER ---

    uint32 NumVertices = Vertices.Num();
    LOD_Resource.VertexBuffers.StaticMeshVertexBuffer.SetUseFullPrecisionUVs(true);

    // We assume one UV channel.
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

    // Finalize render data.
    StaticMesh->InitResources();
    StaticMesh->CalculateExtendedBounds();

    // --- AGGREGATED COLLISION GEOMETRY (AggGeom) ---
    UBodySetup* BodySetup = NewObject<UBodySetup>(StaticMesh, NAME_None, RF_Public | RF_Standalone);
    StaticMesh->SetBodySetup(BodySetup);
    BodySetup->CollisionTraceFlag = CTF_UseDefault;

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

void UMeshOperationsBPLibrary::DeleteEmptyParents(USceneComponent* AssetRoot, UObject* Outer, FDeleteParents DelegateDeleteParents)
{
    AsyncTask(ENamedThreads::GameThread, [DelegateDeleteParents, AssetRoot, Outer]()
        {
            // Get all children components at start.
            TArray<USceneComponent*> ChildrenComps;
            AssetRoot->GetChildrenComponents(true, ChildrenComps);

            if (ChildrenComps.Num() > 1)
            {
                // Conditions.
                bool IsProcessFinished = false;
                TMap<USceneComponent*, bool> AnyEmptyParentLeft;

                while (IsProcessFinished == false)
                {
                    for (int32 ChildIndex = 0; ChildIndex < ChildrenComps.Num(); ChildIndex++)
                    {
                        if (ChildrenComps[ChildIndex]->GetClass()->GetName() == TEXT("StaticMeshComponent") && ChildrenComps[ChildIndex]->GetAttachParent()->GetNumChildrenComponents() == 1)
                        {
                            USceneComponent* EachChild = ChildrenComps[ChildIndex];
                            USceneComponent* MiddleParent = EachChild->GetAttachParent();
                            USceneComponent* GrandParent = MiddleParent->GetAttachParent();

                            // We will use this name as static mesh's new name.
                            const FString New_SMC_Name = MiddleParent->GetName();

                            // Change middle parent's name and destroy it.
                            FGuid MiddleParentSuffix = FGuid::NewGuid();
                            const FString MiddleParentName = TEXT("DeletedSceneComp") + MiddleParentSuffix.ToString();
                            MiddleParent->Rename(*MiddleParentName, Outer);
                            MiddleParent->DestroyComponent(false);

                            // Change target static mesh name.
                            EachChild->AttachToComponent(GrandParent, FAttachmentTransformRules::KeepWorldTransform, NAME_None);
                            EachChild->Rename(*New_SMC_Name, Outer);

                            // Check if there is any empty parent left.
                            if (EachChild->GetAttachParent()->GetNumChildrenComponents() == 1)
                            {
                                AnyEmptyParentLeft.Add(EachChild, true);
                            }
                        }
                    }

                    // If there is an empty parent, get related component and start loop again. 
                    TArray<bool> Array_Bool_Values;
                    AnyEmptyParentLeft.GenerateValueArray(Array_Bool_Values);

                    if (Array_Bool_Values.Contains(true) == true)
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

            AsyncTask(ENamedThreads::GameThread, [DelegateDeleteParents]()
                {
                    DelegateDeleteParents.ExecuteIfBound(true);
                }
            );
        }
    );
}

void UMeshOperationsBPLibrary::OptimizeCenter(USceneComponent* AssetRoot)
{
    // Array variable for children components.
    TArray<USceneComponent*> Children;

    // Array variable for children locations.
    TArray<FVector> ChildrenLocations;

    // Get children components of asset root.
    AssetRoot->GetChildrenComponents(false, Children);

    // Add relative locations of children components to children locations array.
    for (int32 ChildID = 0; ChildID < Children.Num(); ChildID++)
    {
        ChildrenLocations.Add(Children[ChildID]->GetRelativeLocation());
    }

    // Calculate assembly center (Relative Location).
    FVector Sum(0.f);
    for (int32 ChildID = 0; ChildID < (ChildrenLocations.Num()); ChildID++)
    {
        Sum += ChildrenLocations[ChildID];
    }
    FVector AssemblyCenter = Sum / ((float)ChildrenLocations.Num());

    // Subtract assembly center from each child's relative location.
    for (int32 ChildID = 0; ChildID < ChildrenLocations.Num(); ChildID++)
    {
        Children[ChildID]->SetRelativeLocation((ChildrenLocations[ChildID] - AssemblyCenter), false, nullptr, ETeleportType::None);
    }
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

void UMeshOperationsBPLibrary::ExportLevelGLTF(bool bEnableQuantization, bool bResetLocation, bool bResetRotation, bool bResetScale, const FString ExportPath, TSet<AActor*> TargetActors, FDelegateGLTFExport DelegateGLTFExport)
{
    AsyncTask(ENamedThreads::AnyNormalThreadNormalTask, [DelegateGLTFExport, bEnableQuantization, bResetLocation, bResetRotation, bResetScale, ExportPath, TargetActors]()
        {
            TArray<FVector> Array_Locations;
            TArray<FRotator> Array_Rotations;
            TArray<FVector> Array_Scales;

            for (int32 ActorIndex = 0; ActorIndex < TargetActors.Num(); ActorIndex++)
            {
                if (bResetLocation == true)
                {
                    Array_Locations.Add(TargetActors.Array()[ActorIndex]->GetRootComponent()->GetComponentLocation());
                    TargetActors.Array()[ActorIndex]->GetRootComponent()->SetWorldLocation(FVector(0.0f), false, nullptr, ETeleportType::None);
                }

                if (bResetRotation == true)
                {
                    Array_Rotations.Add(TargetActors.Array()[ActorIndex]->GetRootComponent()->GetComponentRotation());
                    TargetActors.Array()[ActorIndex]->GetRootComponent()->SetWorldRotation(FQuat(0.0f), false, nullptr, ETeleportType::None);
                }

                if (bResetScale == true)
                {
                    Array_Scales.Add(TargetActors.Array()[ActorIndex]->GetRootComponent()->GetComponentScale());
                    TargetActors.Array()[ActorIndex]->GetRootComponent()->SetWorldScale3D(FVector(1.0f));
                }
            }

            AsyncTask(ENamedThreads::GameThread, [DelegateGLTFExport, bEnableQuantization, bResetLocation, bResetRotation, bResetScale, Array_Locations, Array_Rotations, Array_Scales, ExportPath, TargetActors]()
                {
                    UGLTFExportOptions* ExportOptions = NewObject<UGLTFExportOptions>();
                    ExportOptions->ResetToDefault();
                    ExportOptions->bExportProxyMaterials = true;
                    ExportOptions->bExportVertexColors = true;
                    ExportOptions->bUseMeshQuantization = bEnableQuantization;

                    FGLTFExportMessages ExportMessages;
                    bool bIsExportSuccessful = UGLTFExporter::ExportToGLTF(GEngine->GetCurrentPlayWorld(), ExportPath, ExportOptions, TargetActors, ExportMessages);

                    for (int32 ActorIndex = 0; ActorIndex < TargetActors.Num(); ActorIndex++)
                    {
                        if (bResetLocation == true)
                        {
                            TargetActors.Array()[ActorIndex]->GetRootComponent()->SetWorldLocation(Array_Locations[ActorIndex], false, nullptr, ETeleportType::None);
                        }

                        if (bResetRotation == true)
                        {
                            TargetActors.Array()[ActorIndex]->GetRootComponent()->SetWorldRotation(Array_Rotations[ActorIndex], false, nullptr, ETeleportType::None);
                        }

                        if (bResetScale == true)
                        {
                            TargetActors.Array()[ActorIndex]->GetRootComponent()->SetWorldScale3D(Array_Scales[ActorIndex]);
                        }
                    }

                    DelegateGLTFExport.ExecuteIfBound(bIsExportSuccessful, ExportMessages);
                }
            );
        }
    );
}

bool UMeshOperationsBPLibrary::GetVerticesLocations(UStaticMeshComponent* In_SMC, int32 LOD_Index, TArray<FVector>& OutVertices)
{
    if (IsValid(In_SMC) == true)
    {
        FPositionVertexBuffer* PositionVertexBuffer = &In_SMC->GetStaticMesh()->GetRenderData()->LODResources[LOD_Index].VertexBuffers.PositionVertexBuffer;

        FVector3d EachVertices;
        for (uint32 VertexIndex = 0; VertexIndex < PositionVertexBuffer->GetNumVertices(); VertexIndex++)
        {
            EachVertices = In_SMC->GetComponentTransform().TransformPosition((FVector3d)PositionVertexBuffer->VertexPosition(VertexIndex));
            OutVertices.Add(EachVertices);
        }

        return true;
    }

    else
    {
        return false;
    }
}

bool UMeshOperationsBPLibrary::SetPivotLocation(UPARAM(ref) UStaticMeshComponent*& In_SMC, FVector PivotLocation, UObject* Outer)
{
    if (IsValid(In_SMC) == false)
    {
        return false;
    }

    UStaticMesh* StaticMesh = In_SMC->GetStaticMesh();

    if (!StaticMesh)
    {
        return false;
    }

    if (!StaticMesh->bAllowCPUAccess)
    {
        return false;
    }

    FStaticMeshRenderData* RenderData = StaticMesh->GetRenderData();

    if (!RenderData)
    {
        return false;
    }

    FVector PivotDelta = (PivotLocation - In_SMC->GetComponentLocation()) * (-1);

    UStaticMesh* NewStaticMesh = NewObject<UStaticMesh>(Outer ? Outer : GetTransientPackage(), NAME_None, RF_Public);
    NewStaticMesh->bAllowCPUAccess = true;
    NewStaticMesh->NeverStream = true;
    NewStaticMesh->bSupportRayTracing = StaticMesh->bSupportRayTracing;

    NewStaticMesh->SetRenderData(MakeUnique<FStaticMeshRenderData>());
    FStaticMeshRenderData* NewRenderData = NewStaticMesh->GetRenderData();

    NewRenderData->AllocateLODResources(RenderData->LODResources.Num());

    for (int32 LODIndex = 0; LODIndex < NewRenderData->LODResources.Num(); LODIndex++)
    {
        FStaticMeshLODResources& OriginalLOD = RenderData->LODResources[LODIndex];
        FStaticMeshLODResources& NewLOD = NewRenderData->LODResources[LODIndex];

        TArray<uint32> Indices;
        OriginalLOD.IndexBuffer.GetCopy(Indices);
        NewLOD.IndexBuffer.SetIndices(Indices, OriginalLOD.IndexBuffer.Is32Bit() ? EIndexBufferStride::Force32Bit : EIndexBufferStride::Force16Bit);

        int32 NumPositions = OriginalLOD.VertexBuffers.PositionVertexBuffer.GetNumVertices();
        NewLOD.VertexBuffers.PositionVertexBuffer.Init(NumPositions);
        for (int32 PositionIndex = 0; PositionIndex < NumPositions; PositionIndex++)
        {
            NewLOD.VertexBuffers.PositionVertexBuffer.VertexPosition(PositionIndex) = OriginalLOD.VertexBuffers.PositionVertexBuffer.VertexPosition(PositionIndex) + FVector3f(PivotDelta);
        }

        uint32 NumVertices = OriginalLOD.VertexBuffers.StaticMeshVertexBuffer.GetNumVertices();

        NewLOD.VertexBuffers.StaticMeshVertexBuffer.Init(NumVertices, OriginalLOD.VertexBuffers.StaticMeshVertexBuffer.GetNumTexCoords());

        for (uint32 VertexIndex = 0; VertexIndex < NumVertices; VertexIndex++)
        {
            FVector4f TangentX = OriginalLOD.VertexBuffers.StaticMeshVertexBuffer.VertexTangentX(VertexIndex);
            FVector4f TangentY = OriginalLOD.VertexBuffers.StaticMeshVertexBuffer.VertexTangentY(VertexIndex);
            FVector4f TangentZ = OriginalLOD.VertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(VertexIndex);
            NewLOD.VertexBuffers.StaticMeshVertexBuffer.SetVertexTangents(VertexIndex, TangentX, TangentY, TangentZ);
            
            for (uint32 UVIndex = 0; UVIndex < OriginalLOD.VertexBuffers.StaticMeshVertexBuffer.GetNumTexCoords(); UVIndex++)
            {
                FVector2f UV = OriginalLOD.VertexBuffers.StaticMeshVertexBuffer.GetVertexUV(VertexIndex, UVIndex);
                NewLOD.VertexBuffers.StaticMeshVertexBuffer.SetVertexUV(VertexIndex, UVIndex, UV);
            }
        }

        TArray<FColor> Colors;
        OriginalLOD.VertexBuffers.ColorVertexBuffer.GetVertexColors(Colors);
        if (Colors.Num() > 0)
        {
            NewLOD.VertexBuffers.ColorVertexBuffer.InitFromColorArray(Colors);
        }

        for (int32 SectionIndex = 0; SectionIndex < OriginalLOD.Sections.Num(); SectionIndex++)
        {
            FStaticMeshSection& OriginalSection = OriginalLOD.Sections[SectionIndex];
            FStaticMeshSection& NewSection = NewLOD.Sections.AddDefaulted_GetRef();
            // copy section configuration
            NewSection = OriginalSection;
        }
    }

    NewStaticMesh->SetStaticMaterials(StaticMesh->GetStaticMaterials());

    NewStaticMesh->InitResources();

    NewRenderData->Bounds = RenderData->Bounds;
    NewRenderData->Bounds.Origin += PivotDelta;

    NewStaticMesh->CalculateExtendedBounds();
    NewStaticMesh->CreateBodySetup();
    NewStaticMesh->GetBodySetup()->CollisionTraceFlag = StaticMesh->GetBodySetup()->CollisionTraceFlag;
    NewStaticMesh->GetBodySetup()->InvalidatePhysicsData();
    NewStaticMesh->GetBodySetup()->AggGeom = StaticMesh->GetBodySetup()->AggGeom;

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

    NewStaticMesh->GetBodySetup()->CreatePhysicsMeshes();

    In_SMC->SetStaticMesh(NewStaticMesh);
    In_SMC->AddWorldOffset(PivotLocation - In_SMC->Bounds.Origin);
    return true;
}

bool UMeshOperationsBPLibrary::MovePivotsToCenter(USceneComponent* RootComponent, TArray<FString>& ErroredMeshes)
{
    if (IsValid(RootComponent) == false)
    {
        return false;
    }

    TArray<USceneComponent*> Array_Children;
    RootComponent->GetChildrenComponents(true, Array_Children);

    int32 StaticMeshCount = 0;

    for (int32 ChildIndex = 0; ChildIndex < Array_Children.Num(); ChildIndex++)
    {
        UStaticMeshComponent* EachMesh = Cast<UStaticMeshComponent>(Array_Children[ChildIndex]);

        if (IsValid(EachMesh) == true)
        {
            if (UMeshOperationsBPLibrary::SetPivotLocation(EachMesh, EachMesh->Bounds.Origin, EachMesh) == false)
            {
                ErroredMeshes.Add(EachMesh->GetReadableName());
            }

            StaticMeshCount += 1;
        }
    }

    if (ErroredMeshes.Num() == StaticMeshCount)
    {
        return false;
    }

    return true;
}