// Fill out your copyright notice in the Description page of Project Settings.


#include "Runner.h"

#include "RunnerPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputDataAsset.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Taco.h"
#include "Engine/LocalPlayer.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


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
	bTacoEquip = false;

	SkillSound = 1.0f;

	Life = 3;

	bUseControllerRotationYaw = false;
	bCollisionState = false;
	CollisionDelay = 0.5f;
	
	BasicSpeed = 600.f;

	CharacterState = ECharacterState::ECS_Normal;

	TacoSocketName = "PetSocket";
}

// Called when the game starts or when spawned
void ARunner::BeginPlay()
{
	Super::BeginPlay();

	RunnerPlayerController = Cast<ARunnerPlayerController>(GetController());
	
	BasicSpeed = GetCharacterMovement()->MaxWalkSpeed;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	TacoPet = GetWorld()->SpawnActor<ATaco>(Taco, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);

	const USkeletalMeshSocket* PetSocket = GetMesh()->GetSocketByName(TacoSocketName);
	if(TacoPet)
	{
		PetSocket->AttachActor(TacoPet, GetMesh());
	}
}

// Called every frame
void ARunner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CharacterState != ECharacterState::ECS_Hit)
	{
		AddMovementInput(GetActorForwardVector());
	}
}

// Called to bind functionality to input
void ARunner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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
		MP -= ThirdRequireSkill;
		if(ThirdSkillSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ThirdSkillSound, SkillSound);
		}
		if(HaeteSpawnSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), HaeteSpawnSound, SkillSound);
		}
		SetCharacterState(ECharacterState::ECS_Haetae);
		RunnerPlayerController->BGMPitchUp();
		RunnerPlayerController->SpawnHaetaeAndPossess();
		FirstSkill();
	}
	else if (ChargeGage >= SecondRequireSkill)
	{
		MP -= SecondRequireSkill;
		bTacoEquip = true;
		TacoPet->SkillShot();
		if (SecondSkillSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), SecondSkillSound, SkillSound);
		}
		GetWorldTimerManager().SetTimer(BombShotTimer, this, &ARunner::ShotBomb, BombDelay, true, 0.f);
		
	}
	else if(ChargeGage >= FirstRequireSkill)
	{
		MP -= FirstRequireSkill;
		if(FirstSkillSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), FirstSkillSound, SkillSound);
		}
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
		CurrentShotTime = 0.0f;
		bTacoEquip = false;
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
	Life--;
	CharacterState = ECharacterState::ECS_Hit;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if(Life > 0)
	{
		if (StiffenMontage)
		{
			AnimInstance->Montage_Play(StiffenMontage, 2.0f);
			AnimInstance->Montage_JumpToSection(FName("Stiffen"), StiffenMontage);
		}
	}
	else if (Life == 0)
	{
		RunnerPlayerController->OpenGameOver();
		
		if (HitMontage)
		{
			FName SectionName = "ScissorKick";
			int32 Index = FMath::RandRange(0, 1);

			switch (Index)
			{
			case 0:
				SectionName = "SoccerTrip";
				break;
			case 1:
				SectionName = "Stunned";
				break;
			default:
				break;
			}
			AnimInstance->Montage_Play(HitMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(SectionName, HitMontage);
		}
	}
}

void ARunner::SetCollisionState()
{
	CharacterState = ECharacterState::ECS_Hit;

	FTimerHandle HitTimer;

	if (Life > 0)
	{
		GetWorldTimerManager().SetTimer(HitTimer, this, &ARunner::ResetCollisionState, CollisionDelay);
	}

	RunnerPlayerController->BGMStop();
}

void ARunner::ResetCollisionState()
{
	CharacterState = ECharacterState::ECS_Normal;

	RunnerPlayerController->BGMPlay();
}

void ARunner::LevelComplete()
{
	if(CompleteMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		FName SectionName = "Cartwheel";
		int32 Index = FMath::RandRange(0, 3);

		switch (Index)
		{
		case 0:
			SectionName = "Looking";
			break;
		case 1:
			SectionName = "Moonwalk";
			break;
		case 2:
			SectionName = "StrongGesture";
			break;
		case 3:
			SectionName = "WaveHipHopDance";
			break;
		default:
				break;
		}

		AnimInstance->Montage_Play(CompleteMontage);
		AnimInstance->Montage_JumpToSection(SectionName, CompleteMontage);
	}
}

void ARunner::FailedAnimationPlay()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (FailedMontage)
	{
		int32 Index = FMath::RandRange(0, 1);
		FName SectionName = (Index == 0) ? "Dying" : "Yelling";
		AnimInstance->Montage_Play(FailedMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(SectionName, FailedMontage);
	}
}
