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

    UFUNCTION(BlueprintPure, meta = (DispayName = "GetClassName", Keywords = "name,class"), Category = "MeshOperations")
    static FString GetClassName(const UObject* Object);

    UFUNCTION(BlueprintCallable, meta = (DispayName = "Get Object Name for Package", Keywords = "name,object,package"), Category = "MeshOperations")
    static bool GetObjectNameForPackage(USceneComponent* Object, bool bUseReadableName, FString Delimeter, FString& OutName);

    UFUNCTION(BlueprintCallable, meta = (DispayName = "Get Component By Name", Keywords = "get, component, by, name"), Category = "MeshOperations")
    static bool GetComponentByName(FName InName, UObject* Owner, USceneComponent*& OutComponent);
        
    UFUNCTION(BlueprintCallable, meta = (DispayName = "AddStaticMeshCompWithName", Keywords = "static,mesh,component,name"), Category = "MeshOperations")
    static bool AddStaticMeshCompWithName(UStaticMeshComponent*& Out_SMC, FName& Out_SMC_Name, FName InName, AActor* SMC_Outer, EAttachmentRule SMC_Attachment_Rule, bool SMC_Manual_Attachment, FTransform SMC_Relative_Transform, EComponentMobility::Type SMC_Mobility = EComponentMobility::Movable);

    UFUNCTION(BlueprintCallable, meta = (DispayName = "AddSceneCompWithName", Keywords = "scene,component,mesh,name"), Category = "MeshOperations")
    static void AddSceneCompWithName(FName InName, AActor* SC_Outer, EComponentMobility::Type SC_Mobility, EAttachmentRule SC_Attachment_Rule, bool SC_Manual_Attachment, FTransform SC_Relative_Transform, bool& Is_SC_Created, FName& Out_SC_Name, USceneComponent*& Out_SC);

    UFUNCTION(BlueprintCallable, meta = (DispayName = "AddProcMeshCompWithName", Keywords = "procedural,mesh,component,name"), Category = "MeshOperations")
    static bool AddProcMeshCompWithName(FName& Out_PMC_Name, UProceduralMeshComponent*& Out_PMC, AActor* PMC_Outer, FName InName, EAttachmentRule PMC_Attachment_Rule, bool PMC_Manual_Attachment, bool bUseAsyncCooking, bool bUseComplexCollisionAsSimple, FTransform PMC_Relative_Transform, EComponentMobility::Type PMC_Mobility = EComponentMobility::Movable);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Generate Box Mesh At Bottom", Keywords = "generate, mesh, box, at, bottom"), Category = "MeshOperations")
    static void GenerateBoxMeshAtBottom(FVector BoxRadius, TArray<FVector>&Vertices, TArray<int32>&Triangles, TArray<FVector>&Normals, TArray<FVector2D>&UVs, TArray<FProcMeshTangent>&Tangents);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Generate Pie", Keywords = "generate, mesh, cylinder"), Category = "MeshOperations")
    static void GenerateCylinderMesh(double Radius, double ArcSize, TArray<FVector2D>& Vertices, int32& EdgeTriangles);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Generate Wave", Keywords = "generate, mesh, wave"), Category = "MeshOperations")
    static bool GenerateWave(bool bIsSin, double Amplitude, double RestHeight, double WaveLenght, TArray<FVector2D>& Out_Vertices, int32& EdgeTriangles);

    UFUNCTION(BlueprintCallable)
    static bool GenerateMeshFromVertices(UStaticMesh*& Out_Mesh, TArray<FVector> In_Vertices, TArray<FVector> In_Normals, TArray<FVector2D> In_UVs, TArray<int32> In_Tris);

    UFUNCTION(BlueprintCallable)
    static UStaticMesh* GenerateStaticMesh(const TArray<FVector>& Vertices, const TArray<int32>& Indices, const TArray<FVector>& Normals, const TArray<FVector>& Tangents, const TArray<FVector2D>& UVs);

    UFUNCTION(BlueprintCallable, meta = (DispayName = "Delete Empty Roots", Keywords = "optimize,hierarchy,empty,root,roots"), Category = "MeshOperations")
    static void DeleteEmptyRoots(USceneComponent* AssetRoot);

    UFUNCTION(BlueprintCallable, meta = (DispayName = "Delete Empty Parents", Keywords = "optimize,hierarchy,empty,parent,parents"), Category = "MeshOperations")
    static void DeleteEmptyParents(USceneComponent* AssetRoot, UObject* Outer, FDeleteParents DelegateDeleteParents);

    UFUNCTION(BlueprintCallable, meta = (DispayName = "OptimizeCenter", Keywords = "optimize,move,components,center"), Category = "MeshOperations")
    static void OptimizeCenter(USceneComponent* AssetRoot);

    UFUNCTION(BlueprintCallable, meta = (DispayName = "OptimizeHeight", Keywords = "optimize,height"), Category = "MeshOperations")
    static void OptimizeHeight(USceneComponent* AssetRoot, float Z_Offset);

    UFUNCTION(BlueprintCallable, meta = (DispayName = "RecordTransforms", ToolTip = "It should be attached to a MAP. Because we used local variable.", Keywords = "record,transforms"), Category = "MeshOperations")
    static void RecordTransforms(USceneComponent* AssetRoot, TMap<USceneComponent*, FTransform>& MapTransform, TArray<USceneComponent*>& AllComponents, TArray<USceneComponent*>& ChildComponents);
    
    UFUNCTION(BlueprintCallable, meta = (DispayName = "Rename Component", Keywords = "rename, component"), Category = "MeshOperations")
    static bool RenameComponent(UPARAM(ref)UObject* Target, UObject* Owner, FName NewName);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Export Level As GLTF", ToolTip = "Description.", Keywords = "level, export, gltf, glb"), Category = "File Converters|GLTF")
    static void ExportLevelGLTF(bool bEnableQuantization, bool bResetLocation, bool bResetRotation, bool bResetScale, const FString ExportPath, TSet<AActor*> TargetActors, FDelegateGLTFExport DelegateGLTFExport);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Vertices Locations", Keywords = "get, vertex, vertices, locations, positions"), Category = "MeshOperations")
    static bool GetVerticesLocations(UStaticMeshComponent* In_SMC, int32 LOD_Index, TArray<FVector>& OutVertices);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Pivot Location", Keywords = "set, move, pivot, location, static, mesh"), Category = "MeshOperations")
    static bool SetPivotLocation(UPARAM(ref) UStaticMeshComponent*& In_SMC, FVector PivotLocation, UObject* Outer);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Move Pivots To Center", Keywords = "set, move, pivot, location, static, mesh, recursive, center"), Category = "MeshOperations")
    static bool MovePivotsToCenter(USceneComponent* RootComponent, TArray<FString>& ErroredMeshes);

};