// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnObstacle.h"

#include "ObstacleBase.h"

// Sets default values
ASpawnObstacle::ASpawnObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bSetObstacles.Init(false, 3);

	RailInterval = 200.f;
	SpawnInterval = 600.f;
	NrOfSpawn = 1;

	LeftRail = 0;
	RightRail = 2;
}

// Called when the game starts or when spawned
void ASpawnObstacle::BeginPlay()
{
	Super::BeginPlay();
	
	for(int32 i = 0; i < NrOfSpawn; i++)
	{
		float NextSpawnInterval = SpawnInterval * i;

		//GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Black, FString::Printf(TEXT("%f"), NextSpawnInterval));
		RailSpawnObstacle(NextSpawnInterval);
	}
}

// Called every frame
void ASpawnObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnObstacle::RailSpawnObstacle(float ForwardInterval)
{
	for (int32 i = 0; i < bSetObstacles.Num(); i++)
	{
		bSetObstacles[i] = FMath::RandBool();
	}

	if (AllObstacles())
	{
		int32 Index = FMath::RandRange(0, bSetObstacles.Num() - 1);
		bSetObstacles[Index] = false;
	}

	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	FVector RightVector = GetActorRightVector();
	FVector ForwardVector = GetActorForwardVector() * ForwardInterval;

	FActorSpawnParameters SpawnParams;
	for (int32 i = LeftRail; i < RightRail; i++)
	{
		if (bSetObstacles[i])
		{
			float DeltaRailInterval = RailInterval * i;
			FVector SpawnLocation = Location + (RightVector * DeltaRailInterval) + ForwardVector;
			GetWorld()->SpawnActor<AObstacleBase>(SpawnObstacle, SpawnLocation, Rotation, SpawnParams);
		}
	}
}

bool ASpawnObstacle::AllObstacles()
{
	for (int32 i = 0; i < bSetObstacles.Num(); i++)
	{
		if (!bSetObstacles[i])
		{
			return false;
		}
	}
	return true;
}