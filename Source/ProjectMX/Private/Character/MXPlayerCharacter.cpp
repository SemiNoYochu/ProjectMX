

#include "Character/MXPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/MXInputConfig.h"
#include "Internal/MyLogMacros.h"
#include "Internal/ComponentUtils.h"
#include "Internal/InputUtils.h"

#pragma region Override ACharacter

AMXPlayerCharacter::AMXPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 450.0f, 0.0f);
	
	SpringArmComponent = ComponentUtils::CreateComponent<USpringArmComponent>(this);
	SpringArmComponent->TargetArmLength = 600.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 10.0f;
	
	CameraComponent = ComponentUtils::CreateComponent<UCameraComponent>(this, true, SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
}

void AMXPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InputUtils::AddInputMapping(this, InputMappingContext);
}

void AMXPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateDynamicCamera(DeltaTime);
}

void AMXPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(PlayerControllerInputConfig->Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(PlayerControllerInputConfig->Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
		
		EnhancedInputComponent->BindAction(PlayerControllerInputConfig->Jump, ETriggerEvent::Started, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(PlayerControllerInputConfig->Jump, ETriggerEvent::Completed, this, &ThisClass::Jump);
		
		EnhancedInputComponent->BindAction(PlayerControllerInputConfig->Dash, ETriggerEvent::Started, this, &ThisClass::StartDash);
		EnhancedInputComponent->BindAction(PlayerControllerInputConfig->Dash, ETriggerEvent::Completed, this, &ThisClass::StopDash);
	}
}

#pragma endregion

#pragma region DynamicCamera

void AMXPlayerCharacter::UpdateDynamicCamera(float DeltaTime)
{
	if (IsValid(GetCharacterMovement()) == true)
	{
		
	}
}

#pragma endregion

#pragma region InputAction

void AMXPlayerCharacter::Move(const FInputActionValue& Value)
{
	if (IsValid(GetController()) == true)
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();
		
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotator(0.f, ControlRotation.Yaw, 0.f);
		
		const FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardVector, MovementVector.X);
		
		const FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightVector, MovementVector.Y);
		
		// MYSCREENLOG("MoveAction X: %f, Y: %f", MovementVector.X, MovementVector.Y);
	}
}

void AMXPlayerCharacter::Look(const FInputActionValue& Value)
{
	if (IsValid(GetController()) == true)
	{
		FVector2D LookVector = Value.Get<FVector2D>();

		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
		
		// MYSCREENLOG("LookAction X: %f, Y: %f", LookVector.X, LookVector.Y);
	}
}

void AMXPlayerCharacter::Jump()
{
	Super::Jump();
	
	// MYSCREENLOG("JumpAction is true");
}

void AMXPlayerCharacter::StartDash()
{
	bIsDashing = true;
	
	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
	
	MYSCREENLOG("StartDashAction is true");
}

void AMXPlayerCharacter::StopDash()
{
	bIsDashing = false;
	
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;
	
	MYSCREENLOG("StopDashAction is true");
}

#pragma endregion

