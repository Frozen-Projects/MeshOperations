#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MeshOps_Includes.h"

#include "Planar_Placement.generated.h"

UCLASS()
class MESHOPERATIONS_API APlanar_Placement : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY()
	TArray<FTransform> Grid_Vertices;

protected:
	
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Called when the game ends or when destroyed.
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	
	// Sets default values for this actor's properties.
	APlanar_Placement();

	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest | Mesh Operations | Planar Placement")
	USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest | Mesh Operations | Planar Placement")
	USplineComponent* Spline = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest | Mesh Operations | Planar Placement")
	int32 Layer = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest | Mesh Operations | Planar Placement")
	double GridSize = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest | Mesh Operations | Planar Placement")
	double Layer_Height = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest | Mesh Operations | Planar Placement")
	FVector ObjectSize = FVector(0.1);

	UFUNCTION(BlueprintPure, Category = "Frozen Forest | Mesh Operations | Planar Placement")
	virtual bool IsPointInsideSpline(const FVector& Point) const;

	UFUNCTION(BlueprintPure, Category = "Frozen Forest | Mesh Operations | Planar Placement")
	virtual TArray<FTransform> GetGridVertices() const;

	UFUNCTION(BlueprintCallable, Category = "Frozen Forest | Mesh Operations | Planar Placement")
	bool Generate_Grid();

};
