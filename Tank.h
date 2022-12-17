// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

	APlayerController* GetTankPlayerController() const {return TankPlayerController;}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess="True"), Category = "Components")
	class USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess="True"), Category = "Components")
	class UCameraComponent* Camera;

	void Move(float Value);
	void Rotate(float Value);

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 200.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationSpeed = 100.f;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	bool IsMovingBackwards = false;
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool InvertBackwardsRotation = true;
	
	APlayerController* TankPlayerController;
};
