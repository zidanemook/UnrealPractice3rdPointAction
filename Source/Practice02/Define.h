// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

UENUM()
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


UENUM()
enum class Equipment_Type : uint8
{
	None,
	Weapon_Sword,
	Weapon_Bow,
	Weapon_Rifle
};
