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

	static bool IsPointInsideSpline(USplineComponent* BoundarySpline, const FVector& Point);

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

	UFUNCTION(BlueprintCallable, Category = "Frozen Forest|Contents|Planar Grid")
	bool Grid_Generate(TArray<FTransform>& Out_Vertices, USplineComponent* BoundarySpline, FVector Size = FVector(0.1), double GridSize = 100, int32 Layer = 1, double Height = 10);

	UFUNCTION(BlueprintCallable, Category = "Frozen Forest|Contents|Planar Grid")
	void Grid_Debug(FVector Point, bool bInside, bool bIsPersistant, double Time = 10);

};
