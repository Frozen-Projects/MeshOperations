#include "Actors/Planar_Meshing.h"

// Sets default values.
APlanar_Meshing::APlanar_Meshing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!this->RootComponent)
	{
		this->DefaultSceneRoot = this->CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
		this->RootComponent = this->DefaultSceneRoot;
	}

	this->Spline = this->CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	this->Spline->SetupAttachment(this->DefaultSceneRoot);
}

// Called when the game starts or when spawned.
void APlanar_Meshing::BeginPlay()
{
	Super::BeginPlay();
}

// Called when the game ends or when destroyed.
void APlanar_Meshing::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame.
void APlanar_Meshing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}