// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnObstacle.generated.h"

class AObstacleBase;

UCLASS()
class FIRST_API ASpawnObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnObstacle();

private:
	TArray<bool> bSetObstacles;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	float RailInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	float SpawnInterval;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Obstacle")
	int32 NrOfSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	TSubclassOf<AActor> SpawnObstacle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Obstacle")
	TArray<TSubclassOf<AObstacleBase>> SpawnObstacles;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Movement")
	int32 LeftRail;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Movement")
	int32 RightRail;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	void RailSpawnObstacle(float ForwardInterval);

	bool AllObstacles();
};
