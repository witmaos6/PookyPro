// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerPlayerController.h"

#include "InputDataAsset.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Runner.h"
#include "GameFramework/CharacterMovementComponent.h"

ARunnerPlayerController::ARunnerPlayerController(const FObjectInitializer& ObjectInitializer)
{
	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
	MoveInterval = 200.f;
	bRailsChangeable = true;
	Direction = 0.0f;

	LeftRail = 0;
	CurrentRail = 1;
	RightRail = 2;

	TimelineBegin = 0.0f;
	TimelineEnd = 0.0f;
}

void ARunnerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Runner = Cast<ARunner>(GetCharacter());

	if (TimelineCurve)
	{
		TimelineUpdateDelegate.BindUFunction(this, FName("TimeLineUpdateFunc"));
		TimelineFinishedDelegate.BindUFunction(this, FName("TimeLineFinishedFunc"));

		TimelineComponent->AddInterpFloat(TimelineCurve, TimelineUpdateDelegate);
		TimelineComponent->SetTimelineFinishedFunc(TimelineFinishedDelegate);
		TimelineComponent->SetLooping(false);

		TimelineCurve->GetTimeRange(TimelineBegin, TimelineEnd);
		TimelineComponent->SetTimelineLength(TimelineEnd);
	}
}

void ARunnerPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ARunnerPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
l
	EnhancedInput->BindAction(InputData->MoveRight, ETriggerEvent::Triggered, this, &ARunnerPlayerController::MoveRight);
	EnhancedInput->BindAction(InputData->MoveLeft, ETriggerEvent::Triggered, this, &ARunnerPlayerController::MoveLeft);
	EnhancedInput->BindAction(InputData->Jump, ETriggerEvent::Triggered, this, &ARunnerPlayerController::Jump);
	EnhancedInput->BindAction(InputData->Slide , ETriggerEvent::Triggered, this, &ARunnerPlayerController::Slide);
}

void ARunnerPlayerController::MoveRight(const FInputActionValue& Value)
{
	if(!Runner->GetCharacterMovement()->IsFalling() && bRailsChangeable && CurrentRail < RightRail)
	{
		bRailsChangeable = false;

		CurrentRail++;

		Direction = 1.0f;
		ChangeRail();
	}
}

void ARunnerPlayerController::MoveLeft(const FInputActionValue& Value)
{
	if (!Runner->GetCharacterMovement()->IsFalling() && bRailsChangeable && CurrentRail > LeftRail)
	{
		bRailsChangeable = false;

		CurrentRail--;

		Direction = -1.0f;
		ChangeRail();
	}
}

void ARunnerPlayerController::ChangeRail()
{
	TimelineComponent->PlayFromStart();
}

void ARunnerPlayerController::TimeLineUpdateFunc(float Output)
{
	FVector ForwardVector = Runner->GetActorForwardVector() * Runner->GetDiagonal();
	FVector RightVector = Runner->GetActorRightVector() * Direction;
	FVector Location = Runner->GetActorLocation();
	FVector MoveLocation = Location + ForwardVector + RightVector;

	FMath::Lerp(Location, MoveLocation, Output);
}

void ARunnerPlayerController::TimeLineFinishedFunc()
{
	bRailsChangeable = true;
}

void ARunnerPlayerController::Slide(const FInputActionValue& Value)
{
	if(Runner)
	{
		FVector ImpulseForce = FVector(0.f, 0.f, -1000.f);

		UCharacterMovementComponent* Movement = Runner->GetCharacterMovement();
		if(Movement)
		{
			Movement->AddImpulse(ImpulseForce, true);

			if(Movement->Velocity.Length() > 60.f && !Movement->IsFalling())
			{
				//UAnimInstance* AnimInstance = Runner->GetMesh()->GetAnimInstance();
				//To do: MontagePlay and Ignore Collision process
			}
		}
	}
}

void ARunnerPlayerController::Jump(const FInputActionValue& Value)
{
	if(Runner)
	{
		Runner->Jump();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Black, "Failed");
	}
}
