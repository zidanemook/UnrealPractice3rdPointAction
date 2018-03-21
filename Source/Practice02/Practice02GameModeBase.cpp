// Fill out your copyright notice in the Description page of Project Settings.

#include "Practice02GameModeBase.h"
#include "Practice02.h"
#include "WeaponEssentialsHUD.h"
#include "WeaponEssentialsCharacter.h"

APractice02GameModeBase::APractice02GameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BPC_Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		HUDClass = AWeaponEssentialsHUD::StaticClass();
	}
}
