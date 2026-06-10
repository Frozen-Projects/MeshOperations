// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MeshOps_Includes.h"
#include "MeshOperationsBPLibrary.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_OneParam(FDeleteParents, bool, IsSuccessed);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_TwoParams(FDelegateGLTFExport, bool, bIsSuccessfull, FGLTFExportMessages, OutMessages);

UCLASS()
class UMeshOperationsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

    UFUNCTION(BlueprintPure, meta = (DisplayName = "GetClassName", Keywords = "name,class"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API FString GetClassName(const UObject* Object);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Object Name for Package", Keywords = "name,object,package"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API bool GetObjectNameForPackage(FString& Out_Name, USceneComponent* Object, bool bUseReadableName = false);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Component By Name", Keywords = "get, component, by, name"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API bool GetComponentByName(FName InName, UObject* Owner, USceneComponent*& OutComponent);
        
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "AddStaticMeshCompWithName", Keywords = "static,mesh,component,name"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API UStaticMeshComponent* AddStaticMeshCompWithName(AActor* Outer, UStaticMesh* In_Mesh, FTransform RelativeTransform, FName In_Name = NAME_None, bool Manual_Attachment = false, EAttachmentRule Attachment_Rule = EAttachmentRule::KeepWorld, EComponentMobility::Type Mobility = EComponentMobility::Movable);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "AddSceneCompWithName", Keywords = "scene,component,mesh,name"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API USceneComponent* AddSceneCompWithName(AActor* Outer, FTransform RelativeTransform, FName In_Name = NAME_None, bool Manual_Attachment = false, EAttachmentRule Attachment_Rule = EAttachmentRule::KeepWorld, EComponentMobility::Type Mobility = EComponentMobility::Movable);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "AddProcMeshCompWithName", Keywords = "procedural,mesh,component,name"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API UProceduralMeshComponent* AddProcMeshCompWithName(AActor* Outer, FName In_Name, EAttachmentRule Attachment_Rule, bool Manual_Attachment, bool bUseAsyncCooking, bool bUseComplexCollisionAsSimple, FTransform Relative_Transform, EComponentMobility::Type Mobility = EComponentMobility::Movable);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Generate Box Mesh At Bottom", Keywords = "generate, mesh, box, at, bottom"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API void GenerateBoxMeshAtBottom(FVector BoxRadius, TArray<FVector>&Vertices, TArray<int32>&Triangles, TArray<FVector>&Normals, TArray<FVector2D>&UVs, TArray<FProcMeshTangent>&ProcMeshTangents, TArray<FVector>& Tangents);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Generate Pie", Keywords = "generate, mesh, cylinder"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API void GenerateCylinderMesh(double Radius, double ArcSize, TArray<FVector2D>& Vertices, int32& EdgeTriangles);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Generate Wave", Keywords = "generate, mesh, wave"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API bool GenerateWave(bool bIsSin, double Amplitude, double RestHeight, double WaveLenght, TArray<FVector2D>& Out_Vertices, int32& EdgeTriangles);

    static MESHOPERATIONS_API UStaticMesh* GSM_Description(FName Mesh_Name, const TArray<FVector>& Vertices, const TArray<int32>& Indices, const TArray<FVector>& Normals, const TArray<FVector>& Tangents, const TArray<FVector2D>& UVs, bool bSupportRayTracing = false);
    static MESHOPERATIONS_API UStaticMesh* GSM_RenderData(FName Mesh_Name, const TArray<FVector>& Vertices, const TArray<int32>& Indices, const TArray<FVector>& Normals, const TArray<FVector>& Tangents, const TArray<FVector2D>& UVs, bool bSupportRayTracing = false);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Generate Static Mesh", Keywords = "generate, static, mesh"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API UStaticMesh* GenerateStaticMesh(FName Mesh_Name, const TArray<FVector>& Vertices, const TArray<int32>& Indices, const TArray<FVector>& Normals, const TArray<FVector>& Tangents, const TArray<FVector2D>& UVs, bool bUseDescription = false, bool bSupportRayTracing = false);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Delete Empty Roots", Keywords = "optimize,hierarchy,empty,root,roots"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API void DeleteEmptyRoots(USceneComponent* AssetRoot);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Delete Empty Parents", Keywords = "optimize,hierarchy,empty,parent,parents"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API void DeleteEmptyParents(USceneComponent* AssetRoot, UObject* Outer, FDeleteParents DelegateDeleteParents);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "OptimizeCenter", Keywords = "optimize,move,components,center"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API void OptimizeCenter(USceneComponent* AssetRoot);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "OptimizeHeight", Keywords = "optimize,height"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API void OptimizeHeight(USceneComponent* AssetRoot, float Z_Offset);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "RecordTransforms", ToolTip = "It should be attached to a MAP. Because we used local variable.", Keywords = "record,transforms"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API void RecordTransforms(USceneComponent* AssetRoot, TMap<USceneComponent*, FTransform>& MapTransform, TArray<USceneComponent*>& AllComponents, TArray<USceneComponent*>& ChildComponents);
    
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Rename Component", Keywords = "rename, component"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API bool RenameComponent(UPARAM(ref)UObject* Target, UObject* Owner, FName NewName);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Export Level As GLTF", ToolTip = "Description.", Keywords = "level, export, gltf, glb"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API void ExportLevelGLTF(bool bEnableQuantization, bool bResetLocation, bool bResetRotation, bool bResetScale, const FString ExportPath, TSet<AActor*> TargetActors, FDelegateGLTFExport DelegateGLTFExport);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Vertices Transform", Keywords = "get, vertex, vertices, locations, positions"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API bool GetVerticesTransforms(TArray<FTransform>& Out_Transform, UStaticMeshComponent* In_SMC, int32 LOD_Index, bool bUseRelativeLocation);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Pivot Location", Keywords = "set, move, pivot, location, static, mesh"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API bool SetPivotLocation(UPARAM(ref) UStaticMeshComponent*& In_SMC, FVector PivotLocation, UObject* Outer);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Move Pivots To Center", Keywords = "set, move, pivot, location, static, mesh, recursive, center"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API bool MovePivotsToCenter(USceneComponent* RootComponent, TArray<FString>& ErroredMeshes);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Direction Of Vector", Keywords = "get, direction, vector"), Category = "Frozen Forest|Mesh Operations")
	static MESHOPERATIONS_API FRotator GetDirectionOfVector(const FVector& Start, const FVector& End);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Scene Component Bounds", Keywords = "get, scene, component, bounds"), Category = "Frozen Forest|Mesh Operations")
    static MESHOPERATIONS_API void GetSceneComponentBounds(FVector& Out_Origin, FVector& Out_Extent, USceneComponent* SceneComponent);

};