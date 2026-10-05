#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MeshOps_Includes.h"
#include "MeshOps_Structs.h"

#include "Spline_Placement.generated.h"

UCLASS()
class MESHOPERATIONS_API ASpline_Placement : public AActor
{
	GENERATED_BODY()
	
private:

	virtual bool SetMaterialParameters();
	virtual void GenerateSplineMesh();

	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

protected:

	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Called when the game ends or when destroyed.
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	// Sets default values for this actor's properties.
	ASpline_Placement();

	virtual void OnConstruction(const FTransform& Transform) override;

	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest| Mesh Operations |Spline Placement")
	USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest| Mesh Operations |Spline Placement")
	USplineComponent* Spline = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest| Mesh Operations |Spline Placement")
	bool bEnableShadow = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest| Mesh Operations |Spline Placement")
	double SectionLength = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest| Mesh Operations |Spline Placement")
	double Thickness = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest| Mesh Operations |Spline Placement")
	TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis = ESplineMeshAxis::X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest| Mesh Operations |Spline Placement")
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest| Mesh Operations |Spline Placement")
	TArray<UMaterialInterface*> Materials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest| Mesh Operations |Spline Placement")
	TArray<FMaterialParametersArray> MaterialParameters;

};
