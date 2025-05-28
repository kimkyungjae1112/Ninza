// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NICharacterEnemyBase.h"
#include "Perception/AISenseConfig_Damage.h"

ANICharacterEnemyBase::ANICharacterEnemyBase()
{
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> OutlineMaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Material/OutlineMaterial_Inst.OutlineMaterial_Inst'"));
	if (OutlineMaterialRef.Object)
	{
		OutlineMaterial = OutlineMaterialRef.Object;
	}
}

float ANICharacterEnemyBase::GetTurnSpeed() const
{
	return 0.0f;
}

float ANICharacterEnemyBase::GetAttakRange() const
{
	return 0.0f;
}

void ANICharacterEnemyBase::SetAIAttackOnWalkDelegate(const FAttackOnWalkFinished& InAttackOnWalkFinished)
{
	AttackOnWalkFinished = InAttackOnWalkFinished;
}

void ANICharacterEnemyBase::AttackOnWalkByAI()
{
}

FName ANICharacterEnemyBase::GetClassName() const
{
	return ClassName;
}

void ANICharacterEnemyBase::SetOverlayMaterial() const
{
	GetMesh()->SetOverlayMaterial(OutlineMaterial);
}

void ANICharacterEnemyBase::SetUnOverlayMaterial() const
{
	GetMesh()->SetOverlayMaterial(nullptr);
}

float ANICharacterEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float SuperResult = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UAISense_Damage::ReportDamageEvent(
		GetWorld(),
		this,
		DamageCauser,
		DamageAmount,
		GetActorLocation(),
		GetActorLocation()
	);

	return 0.f;
}
