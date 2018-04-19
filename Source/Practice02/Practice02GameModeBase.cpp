// Fill out your copyright notice in the Description page of Project Settings.

#include "Practice02GameModeBase.h"
#include "Practice02.h"
#include "WeaponEssentialsHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

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

void APractice02GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (wHUD) // Check if the Asset is assigned in the blueprint.
	{
		// Create the widget and store it.
		m_HUD = CreateWidget<UUserWidget>(GetWorld(), wHUD);

		// now you can use the widget directly since you have a referance for it.
		// Extra check to  make sure the pointer holds the widget.
		if (m_HUD)
		{
			//let add it to the view port
			m_HUD->AddToViewport();
		}
	}
}
