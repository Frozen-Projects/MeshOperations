#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MeshOps_Includes.h"
#include "Planar_Meshing.generated.h"

UCLASS(Blueprintable)
class MESHOPERATIONS_API APlanar_Meshing : public AActor
{
	GENERATED_BODY()

public:
	APlanar_Meshing();
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Frozen Forest | Mesh Operations | Planar Meshing")
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Frozen Forest | Mesh Operations | Planar Meshing")
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Frozen Forest | Mesh Operations | Planar Meshing")
	TObjectPtr<UProceduralMeshComponent> GeneratedMesh;

	// Optional external boundary, including a Planar_Placement actor's spline.
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Frozen Forest | Mesh Operations | Planar Meshing")
	TObjectPtr<USplineComponent> SourceSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest | Mesh Operations | Planar Meshing")
	bool bGenerateOnConstruction = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest | Mesh Operations | Planar Meshing")
	bool bGenerateOnBeginPlay = true;

	// Approximate maximum distance between curved boundary samples, in world units.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest | Mesh Operations | Planar Meshing", meta = (ClampMin = "0.1", Units = "cm"))
	double SampleSpacing = 25.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Frozen Forest | Mesh Operations | Planar Meshing", meta = (ClampMin = "3", ClampMax = "8192"))
	int32 MaxBoundaryVertices = 2048;

	// The plane uses the first spline point's local Z plus this offset.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest | Mesh Operations | Planar Meshing", meta = (Units = "cm"))
	double HeightOffset = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest | Mesh Operations | Planar Meshing", meta = (ClampMin = "0.1", Units = "cm"))
	double UVTileSize = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest | Mesh Operations | Planar Meshing")
	FVector2D UVOffset = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest | Mesh Operations | Planar Meshing")
	TObjectPtr<UMaterialInterface> Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest | Mesh Operations | Planar Meshing")
	bool bCreateCollision = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Frozen Forest | Mesh Operations | Planar Meshing")
	bool bUseAsyncCooking = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest | Mesh Operations | Planar Meshing")
	bool bFlipNormals = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Transient, Category = "Frozen Forest | Mesh Operations | Planar Meshing")
	int32 GeneratedVertexCount = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Transient, Category = "Frozen Forest | Mesh Operations | Planar Meshing")
	int32 GeneratedTriangleCount = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Transient, Category = "Frozen Forest | Mesh Operations | Planar Meshing")
	FString LastGenerationError;

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Frozen Forest | Mesh Operations | Planar Meshing")
	bool Generate_Mesh();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Frozen Forest | Mesh Operations | Planar Meshing")
	void Clear_Mesh();

protected:
	virtual void BeginPlay() override;

private:
	bool FailGeneration(const TCHAR* Reason);
};
