// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Zombie.generated.h"

class USkeletalMeshComponent;

enum class EZombieState : uint8
{
	EZS_Normal UMETA(DisplayName = "Normal"),
	EZS_Crawling UMETA(DisplayName = "Crawling")
};

UCLASS()
class DISMAMBERMENT_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RenameBoneName(FName &HitBoneName);

    bool DealDamageToBone(FName const HitBoneName, int32 const Damage);

	void Dismemberment(FName const HitBoneName);

	void ChangeMovementType();

	void ApplyPhysics(FName const HitBoneName);

	void SetCapsuleLocation();

	void AddForceToBones();

	void TimerEnd();

	bool FindBrokenBones(TArray<FName> Bones);

	FVector CalculateImpulse();

	FVector CalaculateCrawlingForce(float const ForwardForce, float const UpwardForce);

	FVector GetSocketLocation(FName const HitBoneName);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AnyDamage(int32 const Damage, FName HitBoneName, AActor* NewDamageCauser);

private:
    TObjectPtr<USkeletalMeshComponent> SkeletalMesh;
	TObjectPtr<AActor> DamageCauser;
	TMap<FName, int32> BreakableBoneDurability;

	bool bHasSpineBroken = false;

	bool bIsCrawling = false;

	bool bRightArm = true;

	bool bLeftArm = false;

	bool bCrawl = false;

	TArray<FName> BrokenBones;

	EZombieState ZombieState = EZombieState::EZS_Normal;
	
};
