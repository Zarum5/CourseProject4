// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"



// Sets default values
ATank::ATank()
{
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
    CameraSpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(CameraSpringArm);
}


// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

    TankPlayerController = Cast<APlayerController>(GetController());
}


// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (TankPlayerController) {
        FHitResult HitResult;
        TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,HitResult);

        ATank::RotateTurret(HitResult.ImpactPoint);
    }
}



// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Rotate);
    PlayerInputComponent->BindAction(TEXT("Fire"),IE_Pressed,this,&ATank::Fire);
}


void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}


void ATank::Move(float Value)
{
    //find backwards boolean "sticky"
    if (InvertBackwardsRotation) {
        if(Value < 0.f){IsMovingBackwards = true;}
        if(Value > 0.f){IsMovingBackwards = false;}
    }

    FVector DeltaLocation = FVector::ZeroVector;
    DeltaLocation.X = Value*MoveSpeed*UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalOffset(DeltaLocation,true);
}


void ATank::Rotate(float Value)
{
    //Should invert turning direction?
    float RotBaseMult;
    if(IsMovingBackwards){RotBaseMult=-1.f;}
    else{RotBaseMult=1.f;}

    FRotator DeltaRotation = FRotator::ZeroRotator;
    DeltaRotation.Yaw = RotBaseMult*Value*RotationSpeed*UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(DeltaRotation,true);
}