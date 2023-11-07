// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerPlayerController.h"

#include "InputDataAsset.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Runner.h"
#include "GameFramework/CharacterMovementComponent.h"

ARunnerPlayerController::ARunnerPlayerController(const FObjectInitializer& ObjectInitializer)
{
	MoveInterval = 200.f;
	bRailsChangeable = true;

	LeftRail = 0;
	CurrentRail = 1;
	RightRail = 2;
}

void ARunnerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Runner = Cast<ARunner>(GetCharacter());
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

		ChangeRail(Runner->GetActorLocation(), 1.0f);
	}
}

void ARunnerPlayerController::MoveLeft(const FInputActionValue& Value)
{
	if (!Runner->GetCharacterMovement()->IsFalling() && bRailsChangeable && CurrentRail > LeftRail)
	{
		bRailsChangeable = false;

		CurrentRail--;

		ChangeRail(Runner->GetActorLocation(), -1.0f);
	}
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
