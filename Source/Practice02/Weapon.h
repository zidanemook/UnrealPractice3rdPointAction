#pragma once

#include "GameFramework/Actor.h"
#include "./Define.h"
#include "Rocket.h"
#include "Enemy.h"
#include "Weapon.generated.h"

class AUserCharacter;

#define TRACE_WEAPON ECC_GameTraceChannel1

/**
 * 
 */

UENUM(BlueprintType)
namespace EWeaponProjectile
{
	enum ProjectileType
	{
		EBullet			UMETA(DisplayName = "Bullet"),
		ESpread			UMETA(DisplayName = "Spread"),
		EProjectile		UMETA(DisplayName = "Projectile"),
	};
}


USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 MaxClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float TimeBetweenShots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int32 ShotCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float WeaponRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float WeaponSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	UTexture2D* SplashArt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 Priority;
};


UCLASS()
class PRACTICE02_API AWeapon : public AActor
{
	GENERATED_UCLASS_BODY()

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void Instant_Fire();

	UFUNCTION()
	virtual void ProjectileFire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)//BlueprintClass inheriting AWeapon sets this variable.
	FWeaponData WeaponConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	TEnumAsByte<EWeaponProjectile::ProjectileType> ProjectileType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	UBoxComponent *CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ARocket> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	USoundCue *FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 CurrentClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	Equipment_Type m_eEquipType;

	void SetOwningPawn(AUserCharacter *NewOwner);

	void AttachToPlayer();
	void DetachFromPlayer();

	void OnEquip();
	void OnUnEquip();

	void ReloadAmmo();

	Equipment_Type GetEquipmentType();
	void SetEquipmentType(Equipment_Type eType);

	void PlayWeaponSound(USoundCue *Sound);

protected:

	FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread);

	AUserCharacter *MyPawn;
	
};
