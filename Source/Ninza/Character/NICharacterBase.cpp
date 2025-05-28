// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NICharacterBase.h"
#include "MotionWarpingComponent.h"

ANICharacterBase::ANICharacterBase()
{
	MotionWarpComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion Warping Component"));
}

FGenericTeamId ANICharacterBase::GetGenericTeamId() const
{
	return TeamId;
}

void ANICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


