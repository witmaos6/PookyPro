// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runner.generated.h"

class ARunnerPlayerController;
class UTimelineComponent;
class UInputDataAsset;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class FIRST_API ARunner : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARunner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputDataAsset* InputData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float DiagonalTravelDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill")
	float MP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill")
	float MaxMP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill")
	float ChargeGage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill")
	float DeltaCharge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	bool bTransparent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSubclassOf<AActor> Bomb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float BombDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float BombPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float BombDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float BombShotTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	float CurrentShotTime;

	FTimerHandle TransparentTimer;

	FTimerHandle BombShotTimer;

	float FirstRequireSkill;

	float SecondRequireSkill;

	float ThirdRequireSkill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HP")
	int32 Life;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
	bool bCollisionState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
	float CollisionDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
	UAnimMontage* HitMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	ARunnerPlayerController* RunnerPlayerController;

protected:
	void Charging();

	void SkillShot();

	void FirstSkill();

	void FirstSkillTime();

	void ShotBomb();

	void GameOver();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Turning(float Angle);

	FORCEINLINE float GetDiagonal() { return DiagonalTravelDistance; };

	UFUNCTION(BlueprintCallable)
	void IncreaseMP(float Value);

	UFUNCTION(BlueprintCallable)
	void DecreaseHP();

	void SetCollisionState();

	void InitCollisionState();

	bool IsTransparent() { return bTransparent; }

	bool IsCollisionState() { return bCollisionState; }
};
