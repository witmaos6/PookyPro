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
class ATaco;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Normal UMETA(DisplayName = "Normal"),
	ECS_Jump UMETA(DisplayName = "Jump"),
	ECS_Slide UMETA(DisplayName = "Slide"),
	ECS_Hit UMETA(DisplayName = "Hit"),
	ECS_Haetae UMETA(DisplayName = "Haetae"),
	ECS_End UMETA(DisplayName = "End")
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ECharacterState CharacterState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float DiagonalTravelDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float BasicSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	UAnimMontage* SlideMontage;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	uint8 bTacoEquip : 1;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	USoundBase* FirstSkillSound;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	USoundBase* SecondSkillSound;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	USoundBase* ThirdSkillSound;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	USoundBase* HaeteSpawnSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	float SkillSound;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	UAnimMontage* HitMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	UAnimMontage* FailedMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	ARunnerPlayerController* RunnerPlayerController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Complete")
	UAnimMontage* CompleteMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pet")
	TSubclassOf<ATaco> Taco;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pet")
	ATaco* TacoPet;

	UPROPERTY(VisibleDefaultsOnly, Category = "Pet")
	FName TacoSocketName;

protected:
	void Charging();

	void SkillShot();

	void FirstSkill();

	void FirstSkillTime();

	void ShotBomb();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Turning(float Angle);

	FORCEINLINE float GetDiagonal() { return DiagonalTravelDistance; };

	FORCEINLINE float GetBasicSpeed() { return BasicSpeed; }

	UFUNCTION(BlueprintCallable)
	void IncreaseMP(float Value);

	UFUNCTION(BlueprintCallable)
	void DecreaseHP();

	void SetCollisionState();

	void ResetCollisionState();

	bool IsTransparent() { return bTransparent; }

	bool IsCollisionState() { return bCollisionState; }

	UAnimMontage* GetSlideMontage() { return SlideMontage; }

	FORCEINLINE void SetCharacterState(ECharacterState State) { CharacterState = State; }

	FORCEINLINE ECharacterState GetCharacterState() { return CharacterState; }

	UFUNCTION(BlueprintCallable)
	void LevelComplete();

	UFUNCTION(BlueprintCallable)
	void FailedAnimationPlay();
};
