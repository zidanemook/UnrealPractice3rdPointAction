// Fill out your copyright notice in the Description page of Project Settings.

#include "Practice02GameModeBase.h"
#include "Practice02.h"
#include "WeaponEssentialsHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "UserCharacter.generated.h"

APractice02GameModeBase::APractice02GameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BluePrint/BPC_Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		HUDClass = AWeaponEssentialsHUD::StaticClass();
	}
}
