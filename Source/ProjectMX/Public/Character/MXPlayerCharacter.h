// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MXCharacterBase.h"
#include "MXPlayerCharacter.generated.h"

struct FInputActionValue;
class UMXInputConfig;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;

UCLASS()
class PROJECTMX_API AMXPlayerCharacter : public AMXCharacterBase
{
	GENERATED_BODY()

#pragma region Override ACharacter	
	
public:
	// Sets default values for this character's properties
	AMXPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UCameraComponent> CameraComponent;
	
#pragma endregion
	
#pragma region DynamicCamera
	
protected:
	
	
private:
	void UpdateDynamicCamera(float DeltaTime);
	
#pragma endregion
	
protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UMXInputConfig> PlayerControllerInputConfig;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float DashSpeed = 2000.0f;
 
	bool bIsDashing = false;
	float DefaultMaxSpeed;
	
private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	
	void StartDash();
	void StopDash();
};
