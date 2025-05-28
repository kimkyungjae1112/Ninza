// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NICharacterEnemyAssassin.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/Character/NIEnemyAttackComponent.h"
#include "Component/Character/NIStatBaseComponent.h"
#include "Component/Character/NIDamagedComponent.h"
#include "Components/CapsuleComponent.h"
#include "AI/Controller/NIAIControllerAssassin.h"
#include "Ninza.h"

ANICharacterEnemyAssassin::ANICharacterEnemyAssassin()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	WeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh Component"));
	WeaponMeshComp->SetupAttachment(GetMesh(), TEXT("Weapon_Socket_R"));
	WeaponMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	AttackComp = CreateDefaultSubobject<UNIEnemyAttackComponent>(TEXT("Attack Component"));
	AttackComp->AttackOnWalkFinish.BindUObject(this, &ANICharacterEnemyAssassin::EndAttackOnWalkByAI);

	StatComp = CreateDefaultSubobject<UNIStatBaseComponent>(TEXT("Stat Base Component"));

	DamagedComp = CreateDefaultSubobject<UNIDamagedComponent>(TEXT("Damaged Component"));

	ClassName = TEXT("Assassin");
}

float ANICharacterEnemyAssassin::GetTurnSpeed() const
{
	return 2.f;
}

float ANICharacterEnemyAssassin::GetAttakRange() const
{
	return 500.0f;
}

void ANICharacterEnemyAssassin::AttackOnWalkByAI()
{
	AttackComp->BeginAttackOnWalk();
}

AAIController* ANICharacterEnemyAssassin::GetAIController() const
{
	return GetMyAIController();
}

float ANICharacterEnemyAssassin::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float SuperResult = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamagedComp->Damaged();
	NI_LOG(LogNinza, Display, TEXT("Damaged : %f"), DamageAmount);

	return 0.0f;
}

void ANICharacterEnemyAssassin::BeginPlay()
{
	Super::BeginPlay();

	DamagedComp->OnFinishedUpperHit.AddUObject(GetMyAIController(), &ANIAIControllerAssassin::UpperHitSense);
}

void ANICharacterEnemyAssassin::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (DamagedComp->GetIsUppered())
	{
		DamagedComp->EndUppered();
	}
	else if (DamagedComp->GetIsUpperedHit())
	{
		DamagedComp->BeginStandUp();
	}
}

ANIAIControllerAssassin* ANICharacterEnemyAssassin::GetMyAIController() const
{
	return Cast<ANIAIControllerAssassin>(GetController());
}

void ANICharacterEnemyAssassin::EndAttackOnWalkByAI()
{
	AttackOnWalkFinished.ExecuteIfBound();
}
