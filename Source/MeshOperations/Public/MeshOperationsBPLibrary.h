// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "MeshOps_Includes.h"
#include "MeshOps_Structs.h"

#include "MeshOperationsBPLibrary.generated.h"

UCLASS()
class MESHOPERATIONS_API UMeshOperationsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

    UFUNCTION(BlueprintPure, meta = (DisplayName = "GetClassName", Keywords = "name,class"), Category = "Frozen Forest|Mesh Operations")
    static FString GetClassName(const UObject* Object);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Object Name for Package", Keywords = "name,object,package"), Category = "Frozen Forest|Mesh Operations")
    static FString GetObjectNameForPackage(USceneComponent* Object, bool bUseReadableName = false);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Component By Name", Keywords = "get, component, by, name"), Category = "Frozen Forest|Mesh Operations")
    static bool GetComponentByName(FName InName, UObject* Owner, USceneComponent*& OutComponent);
        
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "AddStaticMeshCompWithName", Keywords = "static,mesh,component,name"), Category = "Frozen Forest|Mesh Operations")
    static UStaticMeshComponent* AddStaticMeshCompWithName(AActor* Outer, UStaticMesh* In_Mesh, FTransform RelativeTransform, FName In_Name = NAME_None, bool Manual_Attachment = false, EAttachmentRule Attachment_Rule = EAttachmentRule::KeepWorld, EComponentMobility::Type Mobility = EComponentMobility::Movable);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "AddSceneCompWithName", Keywords = "scene,component,mesh,name"), Category = "Frozen Forest|Mesh Operations")
    static USceneComponent* AddSceneCompWithName(AActor* Outer, FTransform RelativeTransform, FName In_Name = NAME_None, bool Manual_Attachment = false, EAttachmentRule Attachment_Rule = EAttachmentRule::KeepWorld, EComponentMobility::Type Mobility = EComponentMobility::Movable);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "AddProcMeshCompWithName", Keywords = "procedural,mesh,component,name"), Category = "Frozen Forest|Mesh Operations")
    static UProceduralMeshComponent* AddProcMeshCompWithName(AActor* Outer, FName In_Name, EAttachmentRule Attachment_Rule, bool Manual_Attachment, bool bUseAsyncCooking, bool bUseComplexCollisionAsSimple, FTransform Relative_Transform, EComponentMobility::Type Mobility = EComponentMobility::Movable);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Generate Box Mesh At Bottom", Keywords = "generate, mesh, box, at, bottom"), Category = "Frozen Forest|Mesh Operations")
    static void GenerateBoxMeshAtBottom(FVector BoxRadius, TArray<FVector>&Vertices, TArray<int32>&Triangles, TArray<FVector>&Normals, TArray<FVector2D>&UVs, TArray<FProcMeshTangent>&ProcMeshTangents, TArray<FVector>& Tangents);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Generate Pie", Keywords = "generate, mesh, cylinder"), Category = "Frozen Forest|Mesh Operations")
    static void GenerateCylinderMesh(double Radius, double ArcSize, TArray<FVector2D>& Vertices, int32& EdgeTriangles);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Generate Wave", Keywords = "generate, mesh, wave"), Category = "Frozen Forest|Mesh Operations")
    static bool GenerateWave(bool bIsSin, double Amplitude, double RestHeight, double WaveLenght, TArray<FVector2D>& Out_Vertices, int32& EdgeTriangles);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Generate Static Mesh (Description)", Keywords = "generate, static, mesh"), Category = "Frozen Forest|Mesh Operations")
    static UStaticMesh* GSM_Description(FName Mesh_Name, const TArray<FVector>& Vertices, const TArray<int32>& Indices, const TArray<int32>& TriangleMaterialSlots, int32 NumMaterialSlots, const TArray<FVector>& Normals, const TArray<FVector>& Tangents, const TArray<FVector2D>& UVs, bool bSupportRayTracing);
    
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Generate Static Mesh (Render Data)", Keywords = "generate, static, mesh"), Category = "Frozen Forest|Mesh Operations")
    static UStaticMesh* GSM_RenderData(FName Mesh_Name, const TArray<FVector>& Vertices, const TArray<int32>& Indices, const TArray<FVector>& Normals, const TArray<FVector>& Tangents, const TArray<FVector2D>& UVs, bool bSupportRayTracing = false);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Delete Empty Roots", Keywords = "optimize,hierarchy,empty,root,roots"), Category = "Frozen Forest|Mesh Operations")
    static void DeleteEmptyRoots(USceneComponent* AssetRoot);

    /*
    * Delete Empty Parents
    */
	UFUNCTION(BlueprintCallable, CallInEditor, meta = (DisplayName = "Delete Empty Parents (Level Actors at Editor)", Keywords = "optimize, hierarchy, remove, empty, parent"), Category = "Frozen Forest|Mesh Operations")
	static void DEP_LevelActors(AActor* TargetActor);

    /*
    * Delete Empty Parents
    */
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Delete Empty Parents (Components at Runtime)", Keywords = "optimize, hierarchy, remove, empty, parent"), Category = "Frozen Forest|Mesh Operations")
    static void DEP_Components_Runtime(USceneComponent* AssetRoot);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "OptimizeCenter", Keywords = "optimize,move,components,center"), Category = "Frozen Forest|Mesh Operations")
    static void OptimizeCenter(USceneComponent* AssetRoot);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "OptimizeHeight", Keywords = "optimize,height"), Category = "Frozen Forest|Mesh Operations")
    static void OptimizeHeight(USceneComponent* AssetRoot, float Z_Offset);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "RecordTransforms", ToolTip = "It should be attached to a MAP. Because we used local variable.", Keywords = "record,transforms"), Category = "Frozen Forest|Mesh Operations")
    static void RecordTransforms(USceneComponent* AssetRoot, TMap<USceneComponent*, FTransform>& MapTransform, TArray<USceneComponent*>& AllComponents, TArray<USceneComponent*>& ChildComponents);
    
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Rename Component", Keywords = "rename, component"), Category = "Frozen Forest|Mesh Operations")
    static bool RenameComponent(UPARAM(ref)UObject* Target, UObject* Owner, FName NewName);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Export Level As GLTF", ToolTip = "Description.", Keywords = "level, export, gltf, glb"), Category = "Frozen Forest|Mesh Operations")
    static bool ExportLevelGLTF(FGLTFExportMessages& OutMessages, FGLTFExportOptionsStruct Options, FString ExportPath, TSet<AActor*> TargetActors);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Vertices Transform", Keywords = "get, vertex, vertices, locations, positions"), Category = "Frozen Forest|Mesh Operations")
    static bool GetVerticesTransforms(TArray<FTransform>& Out_Transform, UStaticMeshComponent* In_SMC, int32 LOD_Index, bool bUseRelativeLocation);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Pivot Location", Keywords = "set, move, pivot, location, static, mesh"), Category = "Frozen Forest|Mesh Operations")
    static bool SetPivotLocation(UPARAM(ref) UStaticMeshComponent*& In_SMC, FVector PivotLocation, UObject* Outer);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Move Pivots To Center", Keywords = "set, move, pivot, location, static, mesh, recursive, center"), Category = "Frozen Forest|Mesh Operations")
    static bool MovePivotsToCenter(USceneComponent* RootComponent, TArray<FString>& ErroredMeshes);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Direction Of Vector", Keywords = "get, direction, vector"), Category = "Frozen Forest|Mesh Operations")
	static FRotator GetDirectionOfVector(const FVector& Start, const FVector& End);

    UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Scene Component Bounds", Keywords = "get, scene, component, bounds"), Category = "Frozen Forest|Mesh Operations")
    static void GetSceneComponentBounds(FVector& Out_Origin, FVector& Out_Extent, USceneComponent* SceneComponent);

    UFUNCTION(BlueprintPure, meta = (DisplayName = "IsInBounds", Keywords = "is, vector, in, bounds, box, component"), Category = "Frozen Forest|Mesh Operations")
    static bool IsInBounds(USceneComponent* Target_Comp, FVector In_Origin, FVector In_Extent);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Check Assembly", Keywords = "check, assembly"), Category = "Frozen Forest|Mesh Operations")
    static void Check_Assembly(USceneComponent* Target_Root, bool bSpawnBillboardForOnlyProblems = false);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Change Material Instance Parent", Keywords = "change, material, instance, parent"), Category = "Frozen Forest|Mesh Operations|Materials")
	static bool ChangeMaterialInstanceParent(UMaterialInstanceDynamic* MaterialInstance, UMaterialInterface* NewParent);

};