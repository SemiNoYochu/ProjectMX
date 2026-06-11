// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MXPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/MXInputConfig.h"
#include "Internal/ComponentUtils.h"
#include "Internal/InputUtils.h"


// Sets default values
AMXPlayerCharacter::AMXPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = ComponentUtils::CreateComponent<USpringArmComponent>(this);
	SpringArmComponent->TargetArmLength = 300.f;
	// SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	CameraComponent = ComponentUtils::CreateComponent<UCameraComponent>(this, true, SpringArmComponent);
	// CameraComponent->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AMXPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InputUtils::AddInputMapping(this, InputMappingContext);
}

// Called every frame
void AMXPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMXPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(PlayerControllerInputConfig->Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(PlayerControllerInputConfig->Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
		
		EnhancedInputComponent->BindAction(PlayerControllerInputConfig->Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(PlayerControllerInputConfig->Jump, ETriggerEvent::Completed, this, &ACharacter::Jump);
	}
	
	
}

void AMXPlayerCharacter::Move(const FInputActionValue& Value)
{
	
}

void AMXPlayerCharacter::Look(const FInputActionValue& Value)
{
	
}

void AMXPlayerCharacter::Jump(const FInputActionValue& Value)
{
	
}

