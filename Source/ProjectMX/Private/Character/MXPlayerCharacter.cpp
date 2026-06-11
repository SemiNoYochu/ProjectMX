

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
	
	CameraComponent = ComponentUtils::CreateComponent<UCameraComponent>(this, true, SpringArmComponent, NAME_None, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
}

void AMXPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InputUtils::AddInputMapping(this, InputMappingContext);
	
	if (IsValid(GetCharacterMovement()) == true)
	{
		DefaultMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;
	}
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
		// [수식 설명]
		// 기존 FMath::Clamp는 단순히 값을 제한할 뿐입니다.
		// FMath::GetMappedRangeValueClamped는 "0부터 2000(DashSpeed)까지의 현재 속도를
		// 0.0부터 1.0 사이의 비율로 변환해줘!" 라는 뜻입니다. 
		// 그래야 우리가 만든 커브(0~1초 사이)에서 정확한 값을 가져올 수 있거든요.
		float CurrentSpeed = GetVelocity().Size();
		float SpeedRatio = FMath::GetMappedRangeValueClamped(
			FVector2D(0.f, DashSpeed), // 입력 범위 (정지 ~ 대쉬속도)
			FVector2D(0.f, 1.f),       // 출력 범위 (0 ~ 1)
			CurrentSpeed               // 현재 값
		);
		
		// 1. FOV 업데이트
		if (FOVCurve)
		{
			// 커브에서 현재 속도 비율(0~1)에 해당하는 값을 가져옵니다.
			float ExtraFOV = FOVCurve->GetFloatValue(SpeedRatio);
			// 기본 FOV 90에 커브 값을 더합니다. (예: 속도가 빠르면 90 + 20 = 110)
			float TargetFOV = 90.0f + ExtraFOV;
			CameraComponent->FieldOfView = FMath::FInterpTo(CameraComponent->FieldOfView, TargetFOV, DeltaTime, 5.0f);
		}
 
		// 2. 팔 길이(TargetArmLength) 업데이트
		if (ArmLengthCurve)
		{
			float ExtraDistance = ArmLengthCurve->GetFloatValue(SpeedRatio);
			// 기본 거리 600에 커브 값을 더합니다.
			float TargetLength = 600.0f + ExtraDistance;
			SpringArmComponent->TargetArmLength = FMath::FInterpTo(SpringArmComponent->TargetArmLength, TargetLength, DeltaTime, 3.0f);
		}
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

