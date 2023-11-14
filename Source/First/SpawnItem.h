// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnItem.generated.h"

UCLASS()
class FIRST_API ASpawnItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	float RailInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	float SpawnInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	int32 NrOfSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	TSubclassOf<AActor> Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int32 LeftRail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int32 RightRail;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	void SpawnItem();
};
