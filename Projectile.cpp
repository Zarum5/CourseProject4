// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "BasePawn.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Camera/CameraShakeBase.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Component"));
	ParticleComponent->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	MovementComponent->MaxSpeed = 5000.f;
	MovementComponent->InitialSpeed = 4000.f;
}


// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	BaseMesh->OnComponentHit.AddDynamic(this,&AProjectile::OnHit);

	UGameplayStatics::PlaySoundAtLocation(this,LaunchSound,GetActorLocation());
}


// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto MyOwner = GetOwner();
	if (MyOwner==nullptr) 
	{
		DelayedKillProjectile();
		return;
	}
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	if (MyOwnerInstigator==nullptr)
	{
		DelayedKillProjectile();
		return;
	}
	auto DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor,Damage,MyOwnerInstigator,this,DamageTypeClass);

		if (Cast<ABasePawn>(OtherActor) && HitParticles && HitTargetSound)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this,HitParticles,GetActorLocation(),GetActorRotation());
			UGameplayStatics::PlaySoundAtLocation(this,HitTargetSound,GetActorLocation());
		} else if (MissParticles && HitSound) {
			UGameplayStatics::SpawnEmitterAtLocation(this,MissParticles,GetActorLocation(),GetActorRotation());
			UGameplayStatics::PlaySoundAtLocation(this,HitSound,GetActorLocation());
		}

		if (HitCameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}

		
	}
	DelayedKillProjectile();
}

void AProjectile::DelayedKillProjectile()
{
	FTimerHandle KillProjectileTimerHandle;
	GetWorldTimerManager().SetTimer(KillProjectileTimerHandle,this,&AProjectile::KillProjectile,DestructionTimer,false);
	BaseMesh->OnComponentHit.RemoveDynamic(this,&AProjectile::OnHit);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BaseMesh->SetHiddenInGame(true);
	ParticleComponent->Deactivate();
}

void AProjectile::KillProjectile()
{
	Destroy();
}

