// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Taco.generated.h"

UCLASS()
class FIRST_API ATaco : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATaco();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* SkillMontage;

public:
	UFUNCTION(BlueprintCallable)
	void AddCoin();

	void SkillShot();
};
