// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/PlayerController.h"
#include "RunnerPlayerController.generated.h"

class UTimelineComponent;
class ARunner;
class UInputDataAsset;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class FIRST_API ARunnerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARunnerPlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputDataAsset* InputData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	ARunner* Runner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MoveInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bRailsChangeable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int32 CurrentRail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int32 LeftRail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int32 RightRail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimelineComponent* TimelineComponent;

	FOnTimelineFloat TimelineUpdateDelegate;

	FOnTimelineEvent TimelineFinishedDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	UCurveFloat* TimelineCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	float TimelineBegin;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	float TimelineEnd;

	FVector TimelineBeginLocation;

	FVector TimelineEndLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
	float MusicPlayTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music") // To do: 레벨 단위로 바껴야 하기 때문에 구조 변경 필요
	UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
	USoundBase* BackGroundMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
	bool bMusicPlayTemp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<UUserWidget> WBPCharacterStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
	UUserWidget* CharacterStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	float ThirdSkillSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	float ThirdSkillDelta;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	float ThirdSkillTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	TSubclassOf<ACharacter> Haetae;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	ACharacter* HaetaeCharacter;

	FTimerHandle ThirdSkillTimer;

	FTimerHandle JumpTimer;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupInputComponent() override;

protected:
	void MoveRight(const FInputActionValue& Value);

	void MoveLeft(const FInputActionValue& Value);

	UFUNCTION()
	void TimelineUpdateFunc(float Output);

	UFUNCTION()
	void TimelineFinishedFunc();

	void Jump(const FInputActionValue& Value);

	void ResetJumpState();

	void Slide(const FInputActionValue& Value);

public:
	UFUNCTION(BlueprintCallable)
	void ChangeRail(float Direction);

	void BGMStop();

	void BGMPlay();

	UFUNCTION(BlueprintImplementableEvent)
	void RaiseSound();

	UFUNCTION(BlueprintCallable)
	void MusicStart();

	UFUNCTION(BlueprintCallable)
	void OpenGameOver();

	void BGMPitchUp();

	void BGMPitchReset();

	void SpawnHaetaeAndPossess();

	void ResetPossess();
};
