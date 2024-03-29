// Copyright Epic Games, Inc. All Rights Reserved.

#include "DismambermentProjectile.h"
#include "DismambermentCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Public/Zombie.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

ADismambermentProjectile::ADismambermentProjectile() 
{
	UE_LOG(LogTemp, Warning, TEXT("BOOM"));
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ADismambermentProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ADismambermentProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) )
	{
		if(OtherComp->IsSimulatingPhysics() && Cast<AZombie>(Hit.GetActor()) == nullptr){
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		}
		else{
			UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *OtherComp->GetName());
		    AZombie* Zombie = Cast<AZombie>(Hit.GetActor());
		    if(Zombie){
			Zombie->AnyDamage(5, Hit.BoneName, this);
		    }
		}
		

		Destroy();
		
	}
	
}