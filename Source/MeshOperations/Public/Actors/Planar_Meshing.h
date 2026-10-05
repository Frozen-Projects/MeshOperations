#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MeshOps_Includes.h"

#include "Planar_Meshing.generated.h"

UCLASS()
class MESHOPERATIONS_API APlanar_Meshing : public AActor
{
	GENERATED_BODY()
	
private:

protected:
	
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Called when the game ends or when destroyed.
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	
	// Sets default values for this actor's properties.
	APlanar_Meshing();

	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest| Mesh Operations |Spline Placement")
	USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frozen Forest| Mesh Operations |Spline Placement")
	USplineComponent* Spline = nullptr;

};
