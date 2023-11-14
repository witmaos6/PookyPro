// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnItem.h"

// Sets default values
ASpawnItem::ASpawnItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RailInterval = 200.f;
	SpawnInterval = 300.f;
	NrOfSpawn = 4;

	LeftRail = 0;
	RightRail = 2;
}

// Called when the game starts or when spawned
void ASpawnItem::BeginPlay()
{
	Super::BeginPlay();

	SpawnItem();
}

// Called every frame
void ASpawnItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnItem::SpawnItem()
{
	int32 SpawnRail = FMath::RandRange(LeftRail, RightRail);
	FVector RightLocation = GetActorRightVector() * (RailInterval * SpawnRail);
	FVector ForwardLocation = GetActorForwardVector() * SpawnInterval;
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	FActorSpawnParameters SpawnParams;

	for(int32 i = 0; i < NrOfSpawn; i++)
	{
		FVector SpawnLocation = Location + RightLocation + ForwardLocation * i;
		GetWorld()->SpawnActor<AActor>(Item, SpawnLocation, Rotation, SpawnParams);
	}
}

