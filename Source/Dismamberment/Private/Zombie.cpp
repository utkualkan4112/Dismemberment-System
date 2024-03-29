// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Dismamberment/DismambermentCharacter.h"
#include "Dismamberment/DismambermentProjectile.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = GetMesh();

	// Init Breakable Bones
	BreakableBoneDurability.Add(FName("spine_01"), 20);
	BreakableBoneDurability.Add(FName("upperarm_r"), 15);
	BreakableBoneDurability.Add(FName("upperarm_l"), 15);
	BreakableBoneDurability.Add(FName("lowerarm_r"), 10);
	BreakableBoneDurability.Add(FName("lowerarm_l"), 10);
	BreakableBoneDurability.Add(FName("hand_r"), 5);
	BreakableBoneDurability.Add(FName("hand_l"), 5);
	BreakableBoneDurability.Add(FName("thigh_r"), 15);
	BreakableBoneDurability.Add(FName("thigh_l"), 15);
	BreakableBoneDurability.Add(FName("calf_r"), 10);
	BreakableBoneDurability.Add(FName("calf_l"), 10);
	BreakableBoneDurability.Add(FName("foot_r"), 5);
	BreakableBoneDurability.Add(FName("foot_l"), 5);

}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(ZombieState == EZombieState::EZS_Crawling){
		
		if(!bIsCrawling){
		FTimerHandle UnusedHandle;
        GetWorldTimerManager().SetTimer(UnusedHandle, this, &AZombie::TimerEnd, 3.f, false);
	    }
		else {
			SetCapsuleLocation();
		    AddForceToBones();
		}
	}
	
}

void AZombie::AnyDamage(int32 const Damage, FName HitBoneName, AActor* NewDamageCauser)
{
	DamageCauser = NewDamageCauser;
	RenameBoneName(HitBoneName);
	if(DealDamageToBone(HitBoneName, Damage)){
		Dismemberment(HitBoneName);
		ChangeMovementType();
		ApplyPhysics(HitBoneName);
	}

}

void AZombie::RenameBoneName(FName &HitBoneName)
{
	if(HitBoneName == "pelvis" ||
	 HitBoneName == "spine_02" ||
	 HitBoneName == "spine_03" ||
	 HitBoneName == "head" ||
	 HitBoneName == "neck_01" ) {
		HitBoneName = "spine_01";
	 }
	
}

// Return true if the bone has reach the durability limit
bool AZombie::DealDamageToBone(FName const HitBoneName, int32 const Damage)
{
	UE_LOG(LogTemp, Warning, TEXT("I am %s"), *HitBoneName.ToString());
	if(BreakableBoneDurability.Find(HitBoneName)){
		int32 NewHealth = *BreakableBoneDurability.Find(HitBoneName) - Damage;
		if(NewHealth <= 0){
			return true;
		}
		BreakableBoneDurability.Add(HitBoneName, NewHealth);
	}
	return false;
}

void AZombie::Dismemberment(FName const HitBoneName)
{
	SkeletalMesh->BreakConstraint(CalculateImpulse(), GetSocketLocation(HitBoneName), HitBoneName);
	if(HitBoneName == "spine-01"){
		bHasSpineBroken = true;
	}
	
	BrokenBones.Add(HitBoneName);
}

void AZombie::ChangeMovementType()
{
	if(ZombieState == EZombieState::EZS_Normal){
		ZombieState = EZombieState::EZS_Crawling;
	}
}

void AZombie::ApplyPhysics(FName const HitBoneName)
{
	if(HitBoneName == "spine_01" ||
	HitBoneName == "upperarm_r" ||
	HitBoneName == "upperarm_l" ||
	HitBoneName == "lowerarm_r" ||
	HitBoneName == "lowerarm_l" ||
	HitBoneName == "hand_r" ||
	HitBoneName == "hand_l" ||
	HitBoneName == "thigh_r" ||
	HitBoneName == "thigh_l" ||
	HitBoneName == "calf_r" ||
	HitBoneName == "calf_l" ||
	HitBoneName == "foot_r" ||
	HitBoneName == "foot_l") {
		SkeletalMesh->SetSimulatePhysics(true);
		SkeletalMesh->AddImpulseAtLocation(CalculateImpulse(), GetSocketLocation(HitBoneName), HitBoneName);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AZombie::SetCapsuleLocation()
{
	GetCapsuleComponent()->SetWorldLocation(SkeletalMesh->GetSocketLocation(FName("head")));
}

void AZombie::AddForceToBones()
{

	SkeletalMesh->AddForce(CalaculateCrawlingForce(500.f, 2000.f), FName("head"), true);
	UE_LOG(LogTemp, Warning, TEXT("Head Moving"));
	if(bCrawl){
		bCrawl = false;
	TArray<FName> RightBones = {"upperarm_r", "lowerarm_r", "hand_r"};
	TArray<FName> LeftBones = {"upperarm_l", "lowerarm_l", "hand_l"};
	FTimerHandle UnusedHandle;
	if(!FindBrokenBones(RightBones) && bRightArm){
		UE_LOG(LogTemp, Warning, TEXT("Right Arm Not Broken"));
		SkeletalMesh->AddForce(CalaculateCrawlingForce(3000.f, 2000.f), FName("hand_r"), true);
		bRightArm = false;
		bLeftArm = true;
	}
	else if(!FindBrokenBones(LeftBones) && bLeftArm){
		UE_LOG(LogTemp, Warning, TEXT("Left Arm Not Broken"));
		SkeletalMesh->AddForce(CalaculateCrawlingForce(3000.f, 2000.f), FName("hand_l"), true);
		bLeftArm = false;
		bRightArm = true;
	}
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AZombie::TimerEnd, 3, false);
	}
	
}

void AZombie::TimerEnd()
{
	if(!bIsCrawling){
		bIsCrawling = true;
	}
	bCrawl = true;
	
}

bool AZombie::FindBrokenBones(TArray<FName> Bones)
{
	for(FName Bone : Bones){
		if(BrokenBones.Contains(Bone)){
			return true;
		}
	}
	return false;
}

FVector AZombie::CalculateImpulse()
{
	if(GetWorld()){
		return (UKismetMathLibrary::GetForwardVector(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraRotation()) *
	 FMath::RandRange(80000, 120000)) 
	 + (GetActorUpVector() 
	 * FMath::RandRange(5000,8000));
	}
    return FVector();
}

FVector AZombie::CalaculateCrawlingForce(float const ForwardForce, float const UpwardForce)
{
	FVector Result;
	if(DamageCauser){
		FVector ForceLocation;
		ADismambermentProjectile* Projectile = Cast<ADismambermentProjectile>(DamageCauser);
		if(Projectile->GetShooter()){
			ForceLocation = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(GetCapsuleComponent()->GetComponentLocation(), Projectile->GetShooter()->GetActorLocation()));
			UE_LOG(LogTemp, Warning, TEXT("Following Actor is %s"), *Projectile->GetShooter()->GetName());
		}
        float Weight = 1.f;
		
        if(!bHasSpineBroken){
            Weight = 1.2f;
        } 
        float UpwardForceWeight = Weight * UpwardForce;
        float ForwardForceWeight = Weight * ForwardForce;
        Result = ForwardForceWeight * ForceLocation;
        Result = Result + FVector(0.f, 0.f, UpwardForceWeight);
	}
	return Result;
}

FVector AZombie::GetSocketLocation(FName const HitBoneName)
{
	return SkeletalMesh->GetSocketLocation(HitBoneName);
}
