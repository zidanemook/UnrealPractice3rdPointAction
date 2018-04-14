
#include "Weapon.h"
#include "Practice02.h"
#include "UserCharacter.h"
#include "Engine.h"


AWeapon::AWeapon(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	WeaponMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh"));
	WeaponMesh->SetRelativeRotation(FRotator(0, 180, 0), false);
	WeaponMesh->AttachTo(RootComponent);

	m_eEquipType = Equipment_Type::Equip_None;
}

void AWeapon::Fire()
{
	if (ProjectileType == EWeaponProjectile::EBullet)
	{
		if (CurrentClip > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Bullet"));
			Instant_Fire();
			PlayWeaponSound(FireSound);
			CurrentClip -= WeaponConfig.ShotCost;
		}
		else
		{
			ReloadAmmo();
		}
	}
	if (ProjectileType == EWeaponProjectile::ESpread)
	{
		if (CurrentClip > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Spread"));
			for (int32 i = 0; i <= WeaponConfig.WeaponSpread; i++)
			{
				Instant_Fire();
			}
			PlayWeaponSound(FireSound);
			CurrentClip -= WeaponConfig.ShotCost;
		}
		else
		{
			ReloadAmmo();
		}
	}
	if (ProjectileType == EWeaponProjectile::EProjectile)
	{
		if (CurrentClip > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Projectile"));
			ProjectileFire();
			PlayWeaponSound(FireSound);
			CurrentClip -= WeaponConfig.ShotCost;
		}
		else
		{
			ReloadAmmo();
		}
	}
}

void AWeapon::SetOwningPawn(AUserCharacter * NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}

void AWeapon::OnEquip()
{
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToPlayer();
}

void AWeapon::OnUnEquip()
{
	DetachFromPlayer();
}

void AWeapon::AttachToPlayer()
{
	if (MyPawn)
	{
		DetachFromPlayer();

		USkeletalMeshComponent *Character = MyPawn->GetMesh();
		WeaponMesh->SetHiddenInGame(false);
		WeaponMesh->AttachTo(Character, "Hand_R_Socket");
	}
}

void AWeapon::DetachFromPlayer()
{
	WeaponMesh->DetachFromParent();
	WeaponMesh->SetHiddenInGame(true);
}

void AWeapon::ReloadAmmo()
{
	if (CurrentAmmo > 0)
	{
		if (CurrentAmmo < WeaponConfig.MaxClip)
		{
			CurrentClip = CurrentAmmo;
		}
		else
		{
			CurrentAmmo -= WeaponConfig.MaxClip;
			CurrentClip += WeaponConfig.MaxClip;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Blue, "NO AMMO");
	}
}

Equipment_Type AWeapon::GetEquipmentType()
{
	return m_eEquipType;
}

void AWeapon::SetEquipmentType(Equipment_Type eType)
{
	m_eEquipType = eType;
}

void AWeapon::Instant_Fire()
{
		const int32 RandomSeed = FMath::Rand();
		FRandomStream WeaponRandomStream(RandomSeed);
		const float CurrentSpread = WeaponConfig.WeaponSpread;
		const float SpreadCone = FMath::DegreesToRadians(WeaponConfig.WeaponSpread * 0.5);
		//const FVector AimDir = WeaponMesh->GetSocketRotation("Handle").Vector();
		//const FVector AimDir = WeaponMesh->GetSocketTransform("Handle", RTS_World).GetRotation().GetUpVector();

		const FVector StartTrace = WeaponMesh->GetSocketLocation("Handle");

		//끝점위치벡터 = 총위치 + (카메라 방향 벡터 * 무기사거리)
		//총의 발사 방향 벡터 = 정규화( 끝점위치 - 총위치벡터 )
		FVector AimDir;
		if (MyPawn)
		{
			const FVector CamLookAt = MyPawn->GetController()->CastToPlayerController()->PlayerCameraManager->GetCameraRotation().Vector();
			const FVector EndPointOfWeaponRange = StartTrace + (CamLookAt * WeaponConfig.WeaponRange);
			AimDir = (EndPointOfWeaponRange - StartTrace);
			AimDir.Normalize();

			DrawDebugLine(this->GetWorld(), StartTrace, EndPointOfWeaponRange, FColor::Black, true, 10000, 10.f);
		}
	
		const FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);
		const FVector EndTrace = StartTrace + ShootDir * WeaponConfig.WeaponRange;
		const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

		ProcessInstantHit(Impact, StartTrace, ShootDir, RandomSeed, CurrentSpread);
}

FHitResult AWeapon::WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const
{
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, TRACE_WEAPON, TraceParams);

	return Hit;
}

void AWeapon::ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread)
{
	const FVector EndTrace = Origin + ShootDir * WeaponConfig.WeaponRange;
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;
	DrawDebugLine(this->GetWorld(), Origin, Impact.TraceEnd, FColor::Black, true, 10000, 10.f);

	AEnemy *Enemy = Cast<AEnemy>(Impact.GetActor());
	if (Enemy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT AN ENEMY!!");
		Enemy->Destroy();
	}
}

void AWeapon::ProjectileFire()
{

}

UAudioComponent* AWeapon::PlayWeaponSound(USoundCue *Sound)
{
	UAudioComponent* AC = NULL;
	if (Sound && MyPawn)
	{
		AC = UGameplayStatics::PlaySoundAttached(Sound, MyPawn->GetRootComponent());
	}

	return AC;
}

