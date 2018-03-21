// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Practice02.h"



AEnemy::AEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "Mesh");
	RootComponent = Mesh;
}


