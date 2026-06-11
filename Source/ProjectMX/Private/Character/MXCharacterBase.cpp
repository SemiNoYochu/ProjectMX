// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MXCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AMXCharacterBase::AMXCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	float InHalfHeight = 90.f;
	float InRadius = 40.f;

	GetCapsuleComponent()->InitCapsuleSize(InRadius, InHalfHeight);

	FVector NewLocation(0.f, 0.f, -InHalfHeight);
	FRotator NewRotation(0.f, -90.f, 0.f); //Pitch, Yaw, Roll( z, y, x )
	GetMesh()->SetRelativeLocationAndRotation(NewLocation, NewRotation);

	//UCharacterMovementComponent*
	GetCharacterMovement()->MaxWalkSpeed                = 350.f;
	GetCharacterMovement()->MinAnalogWalkSpeed          = 20.f;
	GetCharacterMovement()->JumpZVelocity               = 700.f;
	GetCharacterMovement()->AirControl                  = 0.3f; //공중에서 떠 있을때(0 ~ 1)
	GetCharacterMovement()->BrakingDecelerationWalking  = 2000.f; //브레이크(크면 바로 멈춤)
}

// Called when the game starts or when spawned
void AMXCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMXCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMXCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

