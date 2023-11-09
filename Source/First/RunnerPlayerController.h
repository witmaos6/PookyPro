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
	USoundBase* BackGroundMusic; // To do:일시 정지 및 재개를 위해서 오디오 컴포넌트로 바꿔야함.

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

	void Slide(const FInputActionValue& Value);

public:
	UFUNCTION(BlueprintCallable)
	void ChangeRail(float Direction);
};
