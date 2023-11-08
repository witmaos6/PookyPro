l// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/PlayerController.h"
#include "RunnerPlayerController.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnTimelineFloat, float, Output);

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

	float Direction;

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

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupInputComponent() override;

protected:
	void MoveRight(const FInputActionValue& Value);

	void MoveLeft(const FInputActionValue& Value);

	UFUNCTION(BlueprintImplementableEvent, Category = "Movement")
	void ChangeRail(FVector CharacterLocation);

	void ChangeRail();

	UFUNCTION()
	void TimeLineUpdateFunc(float Output);

	UFUNCTION()
	void TimeLineFinishedFunc();

	void Jump(const FInputActionValue& Value);

	void Slide(const FInputActionValue& Value);
};
