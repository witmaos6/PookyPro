// Fill out your copyright notice in the Description page of Project Settings.


#include "Runner.h"

#include "RunnerPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputDataAsset.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"


// Sets default values
ARunner::ARunner()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	JumpMaxCount = 2;
	JumpKeyHoldTime = 1.0f;

	DiagonalTravelDistance = 100.f;

	MP = 0.f;
	MaxMP = 100.f;
	FirstRequireSkill = 30.f;
	SecondRequireSkill = 60.f;
	ThirdRequireSkill = 100.f;
	ChargeGage = 0.0f;
	DeltaCharge = 1.0f;
	bTransparent = false;
	BombDistance = 50.f;
	BombPitch = 20.f;
	BombDelay = 0.5f;
	BombShotTime = 5.0f;
	CurrentShotTime = 0.f;

	Life = 3;

	bUseControllerRotationYaw = false;
	bCollisionState = false;
	CollisionDelay = 0.5f;
}

// Called when the game starts or when spawned
void ARunner::BeginPlay()
{
	Super::BeginPlay();

	RunnerPlayerController = Cast<ARunnerPlayerController>(GetController());
}

// Called every frame
void ARunner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bCollisionState)
	{
		AddMovementInput(GetActorForwardVector());
	}
}

// Called to bind functionality to input
void ARunner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Cast<ULocalPlayer>(this));

	/*Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);*/

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);

	EnhancedInput->BindAction(InputData->Charging, ETriggerEvent::Ongoing, this, &ARunner::Charging);
	EnhancedInput->BindAction(InputData->SkillShot, ETriggerEvent::Completed, this, &ARunner::SkillShot);
}

void ARunner::Charging()
{
	if (ChargeGage < MP)
	{
		ChargeGage = FMath::Clamp(ChargeGage + DeltaCharge * GetWorld()->GetDeltaSeconds(), 0.f, MP);
	}
}

void ARunner::SkillShot()
{
	if (ChargeGage >= ThirdRequireSkill)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Blue, FString("Third"), false);
		MP -= ThirdRequireSkill;
	}
	else if (ChargeGage >= SecondRequireSkill)
	{
		GetWorldTimerManager().SetTimer(BombShotTimer, this, &ARunner::ShotBomb, BombDelay, true, 0.f);
		MP -= SecondRequireSkill;
	}
	else if(ChargeGage >= FirstRequireSkill)
	{
		MP -= FirstRequireSkill;
		FirstSkill();
	}
	ChargeGage = 0.0f;
}

void ARunner::FirstSkill()
{
	bTransparent = true;

	GetWorldTimerManager().SetTimer(TransparentTimer, this, &ARunner::FirstSkillTime, 5.0f, false);
}

void ARunner::FirstSkillTime()
{
	bTransparent = false;
}

void ARunner::ShotBomb()
{
	CurrentShotTime += GetWorldTimerManager().GetTimerElapsed(BombShotTimer);
	if(CurrentShotTime >= BombShotTime)
	{
		GetWorldTimerManager().ClearTimer(BombShotTimer);
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector Location = GetActorLocation() + GetActorForwardVector() * BombDistance;
	FRotator Rotation = GetActorRotation();
	Rotation.Pitch += BombPitch;
	GetWorld()->SpawnActor<AActor>(Bomb, Location, Rotation, SpawnParams);
}

void ARunner::IncreaseMP(float Value)
{
	MP = FMath::Clamp(MP + Value, 0.f, MaxMP);
}

void ARunner::DecreaseHP()
{
	if(Life > 0) // Temp
	{
		Life--;

		if (Life == 0)
		{
			RunnerPlayerController->OpenGameOver();
		}
	}
}

void ARunner::SetCollisionState()
{
	bCollisionState = true;

	FTimerHandle HitTimer;

	if (Life > 0)
	{
		GetWorldTimerManager().SetTimer(HitTimer, this, &ARunner::InitCollisionState, CollisionDelay);
	}

	RunnerPlayerController->BGMStop();

	if(HitMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(HitMontage, 1.0f);
	}
}

void ARunner::InitCollisionState()
{
	bCollisionState = false;

	RunnerPlayerController->BGMPlay();
}
