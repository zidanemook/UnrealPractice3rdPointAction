// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
enum class Motion_State : uint8
{
	Stand_Idle,
	Stand_Walk,
	Stand_Run,
	Stand_Jump,
	Crouch_Idle,
	Crouch_Walk,
	Crouch_Run,
	Crouch_Jump,
};


UENUM(BlueprintType)
enum class Equipment_Type : uint8
{
	Equip_None		UMETA(DisplayName = "Equip_None"),
	Equip_Sword		UMETA(DisplayName = "Equip_Sword"),
	Equip_Bow		UMETA(DisplayName = "Equip_Bow"),
	Equip_Pistol	UMETA(DisplayName = "Equip_Pistol"),
	Equip_Rifle		UMETA(DisplayName = "Equip_Rifle"),
	Equip_Axe		UMETA(DisplayName = "Equip_Axe"),
	Equip_Shovel	UMETA(DisplayName = "Equip_Shovel"),
	Equip_Pickaxe	UMETA(DisplayName = "Equip_Pickaxe"),
	Equip_Max
};
