// Fill out your copyright notice in the Description page of Project Settings.


#include "Taco.h"

// Sets default values
ATaco::ATaco()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
}

// Called when the game starts or when spawned
void ATaco::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATaco::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATaco::AddCoin()
{
	UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();

	if(AnimInstance && SkillMontage)
	{
		AnimInstance->Montage_Play(SkillMontage);
		AnimInstance->Montage_JumpToSection(FName("Coin"), SkillMontage);
	}
}

void ATaco::SkillShot()
{
	UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();

	if (AnimInstance && SkillMontage)
	{
		AnimInstance->Montage_Play(SkillMontage);
		AnimInstance->Montage_JumpToSection(FName("Skill"), SkillMontage);
	}
}
