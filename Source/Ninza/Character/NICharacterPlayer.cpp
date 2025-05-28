// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NICharacterPlayer.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Component/Character/NIMovementComponent.h"
#include "Component/Character/NIAttackComponent.h"
#include "Component/Character/NIStatBaseComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/NIAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "UI/NIHUDWidget.h"
#include "Data/NIInputData.h"
#include "Data/NICameraData.h"
#include "Ninza.h"

ANICharacterPlayer::ANICharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/CharacterAsset/Assassin/Mesh/SKM_Assassin_Skin4.SKM_Assassin_Skin4'"));
	if (BodyMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(BodyMeshRef.Object);
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
	}

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComp->SetupAttachment(GetMesh());
	SpringArmComp->TargetArmLength = 600.f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = 450.f;
	JumpMaxCount = 2;

	WeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh Component"));
	WeaponMeshComp->SetupAttachment(GetMesh(), TEXT("Weapon_Socket_R"));
	WeaponMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UNIInputData> InputDataRef(TEXT("/Script/Ninza.NIInputData'/Game/Data/Input/DA_Input.DA_Input'"));
	if (InputDataRef.Object)
	{
		InputData = InputDataRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UNICameraData> IdleCameraSetDataRef(TEXT("/Script/Ninza.NICameraData'/Game/Data/Camera/DA_IdleCameraSet.DA_IdleCameraSet'"));
	if (IdleCameraSetDataRef.Object)
	{
		IdleCameraSetData = IdleCameraSetDataRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNICameraData> FocusOnCameraSetDataRef(TEXT("/Script/Ninza.NICameraData'/Game/Data/Camera/DA_FocusOnCameraSet.DA_FocusOnCameraSet'"));
	if (FocusOnCameraSetDataRef.Object)
	{
		FocusOnCameraSetData = FocusOnCameraSetDataRef.Object;
	}

	InputChangeMap.Add(EBehaviorState::EIdle, FOnInputChangeWrapper(FOnInputChange::CreateUObject(this, &ANICharacterPlayer::SetIdleState)));
	InputChangeMap.Add(EBehaviorState::ECrouch, FOnInputChangeWrapper(FOnInputChange::CreateUObject(this, &ANICharacterPlayer::SetCrouchState)));
	InputChangeMap.Add(EBehaviorState::EWalk, FOnInputChangeWrapper(FOnInputChange::CreateUObject(this, &ANICharacterPlayer::SetWalkState)));
	InputChangeMap.Add(EBehaviorState::ERun, FOnInputChangeWrapper(FOnInputChange::CreateUObject(this, &ANICharacterPlayer::SetRunState)));
	InputChangeMap.Add(EBehaviorState::EJump, FOnInputChangeWrapper(FOnInputChange::CreateUObject(this, &ANICharacterPlayer::SetJumpState)));
	InputChangeMap.Add(EBehaviorState::EAttacking, FOnInputChangeWrapper(FOnInputChange::CreateUObject(this, &ANICharacterPlayer::SetAttackingState)));
	InputChangeMap.Add(EBehaviorState::EDefensing, FOnInputChangeWrapper(FOnInputChange::CreateUObject(this, &ANICharacterPlayer::SetDefensingState)));
	InputChangeMap.Add(EBehaviorState::EFocusing, FOnInputChangeWrapper(FOnInputChange::CreateUObject(this, &ANICharacterPlayer::SetFocusingState)));

	NinzaMovementComp = CreateDefaultSubobject<UNIMovementComponent>(TEXT("Ninza Movement Component"));

	AttackComp = CreateDefaultSubobject<UNIAttackComponent>(TEXT("Ninza Attack Component"));
	AttackComp->OnEndAttack.AddUObject(this, &ANICharacterPlayer::SetBehaviorState);
	AttackComp->OnEndAttack.AddUObject(this, &ANICharacterPlayer::UnFocusing);

	StatComp = CreateDefaultSubobject<UNIStatBaseComponent>(TEXT("Ninza Stat Component"));

	ClassName = TEXT("Player");
	TeamId = FGenericTeamId(0);

	SwordDamage.Add(EBehaviorState::EIdle, 300.f);
	SwordDamage.Add(EBehaviorState::EWalk, 500.f);
	SwordDamage.Add(EBehaviorState::ERun, 700.f);
	SwordDamage.Add(EBehaviorState::EFocusing, 450.f);
}

void ANICharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAttacking || bIsJumping || bIsDefensing || bIsFocusing) return;

	float Speed = GetCharacterMovement()->Velocity.Size();
	EBehaviorState NewState;
	if (Speed >= 600.f) NewState = EBehaviorState::ERun;
	else if (Speed >= 300.f) NewState = EBehaviorState::EWalk;
	else NewState = EBehaviorState::EIdle;

	if (CurrentState != NewState) 
	{
		SetBehaviorState(NewState);
	}
}

void ANICharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(InputData->IA_Move, ETriggerEvent::Triggered, this, &ANICharacterPlayer::Move);
	EnhancedInputComponent->BindAction(InputData->IA_Move, ETriggerEvent::Completed, this, &ANICharacterPlayer::UnMove);
	EnhancedInputComponent->BindAction(InputData->IA_Look, ETriggerEvent::Triggered, this, &ANICharacterPlayer::Look);
	EnhancedInputComponent->BindAction(InputData->IA_Run, ETriggerEvent::Started, this, &ANICharacterPlayer::Run);
	EnhancedInputComponent->BindAction(InputData->IA_Run, ETriggerEvent::Completed, this, &ANICharacterPlayer::Walk);
	EnhancedInputComponent->BindAction(InputData->IA_Jump, ETriggerEvent::Started, this, &ANICharacterPlayer::Jump);

	EnhancedInputComponent->BindAction(InputData->IA_AttackOnIdle, ETriggerEvent::Started, this, &ANICharacterPlayer::AttackOnIdle);
	EnhancedInputComponent->BindAction(InputData->IA_AttackOnWalk, ETriggerEvent::Started, this, &ANICharacterPlayer::AttackOnWalk);
	EnhancedInputComponent->BindAction(InputData->IA_AttackOnRun, ETriggerEvent::Started, this, &ANICharacterPlayer::AttackOnRun);
	EnhancedInputComponent->BindAction(InputData->IA_AttackOnFocusing, ETriggerEvent::Started, this, &ANICharacterPlayer::AttackOnFocusing);
	EnhancedInputComponent->BindAction(InputData->IA_Attacking, ETriggerEvent::Started, this, &ANICharacterPlayer::Attacking);
	EnhancedInputComponent->BindAction(InputData->IA_RushAttack, ETriggerEvent::Started, this, &ANICharacterPlayer::RushAttack);

	EnhancedInputComponent->BindAction(InputData->IA_Defense, ETriggerEvent::Started, this, &ANICharacterPlayer::Defense);
	EnhancedInputComponent->BindAction(InputData->IA_Defense, ETriggerEvent::Completed, this, &ANICharacterPlayer::UnDefense);

	EnhancedInputComponent->BindAction(InputData->IA_FocusOn, ETriggerEvent::Started, this, &ANICharacterPlayer::FocusingOn);

}

FName ANICharacterPlayer::GetClassName() const
{
	return ClassName;
}

USkeletalMeshComponent* ANICharacterPlayer::GetSwordComp() const
{
	return WeaponMeshComp;
}

TMap<EBehaviorState, float> ANICharacterPlayer::GetSwordDamage() const
{
	return SwordDamage;
}

EBehaviorState ANICharacterPlayer::GetPlayerState() const
{
	return CurrentState;
}

void ANICharacterPlayer::SetHUD(UNIHUDWidget* InHUDWidget)
{
	HUDWidget = InHUDWidget;
	if (HUDWidget)
	{
		HUDWidget->SetMaxHp(1000.f);
		HUDWidget->UpdateHpBar(1000.f);
		StatComp->OnHpChanged.AddUObject(HUDWidget, &UNIHUDWidget::UpdateHpBar);
	}
}

UCameraComponent* ANICharacterPlayer::GetPlayerCamera() const
{
	return CameraComp;
}

USpringArmComponent* ANICharacterPlayer::GetPlayerSpringArm() const
{
	return SpringArmComp;
}

void ANICharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetIdleState();
}

void ANICharacterPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	SetBehaviorState(EBehaviorState::EIdle);

	CurrentJumpCount = 0;
	GetCharacterMovement()->JumpZVelocity = 450.f;
	UNIAnimInstance* Anim = Cast<UNIAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim) Anim->bIsDoubleJumping = false;
}

APlayerController* ANICharacterPlayer::GetPlayerController() const
{
	return CastChecked<APlayerController>(GetController());
}

void ANICharacterPlayer::SetBehaviorState(const EBehaviorState& InState)
{
	if (InState != EBehaviorState::EAttacking) CurrentState = InState;

	InputChangeMap[InState].OnInputChange.ExecuteIfBound();
}

void ANICharacterPlayer::SetIdleState()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		NI_LOG(LogNinza, Display, TEXT("Idle State"));
		bIsAttacking = bIsJumping = false;
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputData->IMC_Idle, 0);
	}
}

void ANICharacterPlayer::SetCrouchState()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputData->IMC_Crouch, 0);
	}
}

void ANICharacterPlayer::SetWalkState()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		NI_LOG(LogNinza, Display, TEXT("Walk State"));
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputData->IMC_Walk, 0);
	}
}

void ANICharacterPlayer::SetRunState()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		NI_LOG(LogNinza, Display, TEXT("Run State"));
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputData->IMC_Run, 0);
	}
}

void ANICharacterPlayer::SetJumpState()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputData->IMC_Jump, 0);
	}
}

void ANICharacterPlayer::SetAttackingState()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputData->IMC_Attacking, 0);
	}
}

void ANICharacterPlayer::SetDefensingState()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputData->IMC_Defensing, 0);
	}
}

void ANICharacterPlayer::SetFocusingState()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		NI_LOG(LogNinza, Display, TEXT("Focusing State"));
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputData->IMC_Focusing, 0);
	}
}

void ANICharacterPlayer::Move(const FInputActionValue& Value)
{
	NinzaMovementComp->Move(Value);
}

void ANICharacterPlayer::UnMove(const FInputActionValue& Value)
{
	NinzaMovementComp->UnMove();
}

void ANICharacterPlayer::Look(const FInputActionValue& Value)
{
	NinzaMovementComp->Look(Value);
}

void ANICharacterPlayer::Run()
{
	NinzaMovementComp->Run();
}

void ANICharacterPlayer::Walk()
{
	NinzaMovementComp->Walk();
}

void ANICharacterPlayer::Jump()
{
	bIsJumping = true;
	SetBehaviorState(EBehaviorState::EJump);

	if (CurrentJumpCount < MaxJumpCount)
	{
		if (CurrentJumpCount == 1)
		{
			UNIAnimInstance* Anim = Cast<UNIAnimInstance>(GetMesh()->GetAnimInstance());
			if (Anim) Anim->bIsDoubleJumping = true;

			GetCharacterMovement()->JumpZVelocity = 600.f;
		}

		ACharacter::Jump();
		CurrentJumpCount++;
	}
}

void ANICharacterPlayer::Attacking()
{
	switch (CurrentState)
	{
	case EBehaviorState::EIdle:
		AttackOnIdle();
		break;
	case EBehaviorState::EWalk:
		AttackOnWalk();
		break;
	case EBehaviorState::ERun:
		AttackOnRun();
		break;
	case EBehaviorState::EFocusing:
		AttackOnFocusing();
		break;
	default:
		break;
	}
}

void ANICharacterPlayer::AttackOnIdle()
{
	bIsAttacking = true;
	SetBehaviorState(EBehaviorState::EAttacking);
	AttackComp->AttackOnIdle();
}

void ANICharacterPlayer::AttackOnWalk()
{
	bIsAttacking = true;
	SetBehaviorState(EBehaviorState::EAttacking);
	AttackComp->AttackOnWalk();
}

void ANICharacterPlayer::AttackOnRun()
{
	bIsAttacking = true;
	SetBehaviorState(EBehaviorState::EAttacking);
	AttackComp->AttackOnRun();
}

void ANICharacterPlayer::AttackOnFocusing()
{
	NI_LOG(LogNinza, Display, TEXT("Focusing"));
	bIsAttacking = true;
	SetBehaviorState(EBehaviorState::EAttacking);
	AttackComp->AttackOnFocusing();
}

void ANICharacterPlayer::RushAttack()
{
	AttackComp->RushAttack();
	AttackComp->UnFocusingOn();
	FocusOnToIdleCamera();
}

void ANICharacterPlayer::Defense()
{
	bIsDefensing = true;
	SetBehaviorState(EBehaviorState::EDefensing);
	AttackComp->Defense();
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void ANICharacterPlayer::UnDefense()
{
	bIsDefensing = false;
	AttackComp->UnDefense();
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void ANICharacterPlayer::FocusingOn()
{
	if (AttackComp->GetFocusState())
	{
		UnFocusing(EBehaviorState::EIdle);
		AttackComp->UnFocusingOn();
		FocusOnToIdleCamera();
	}
	else
	{
		bIsFocusing = true;
		SetBehaviorState(EBehaviorState::EFocusing);
		AttackComp->FocusingOn();
		IdleToFocusOnCamera();
	}
}

void ANICharacterPlayer::IdleToFocusOnCamera()
{
	SpringArmComp->bUsePawnControlRotation = FocusOnCameraSetData->bUsePawnControlRotation;
	bUseControllerRotationYaw = FocusOnCameraSetData->bUseControllerRotationYaw;
	GetCharacterMovement()->bOrientRotationToMovement = FocusOnCameraSetData->bOrientRotationToMovement;
}

void ANICharacterPlayer::FocusOnToIdleCamera()
{
	SpringArmComp->bUsePawnControlRotation = IdleCameraSetData->bUsePawnControlRotation;
	bUseControllerRotationYaw = IdleCameraSetData->bUseControllerRotationYaw;
	GetCharacterMovement()->bOrientRotationToMovement = IdleCameraSetData->bOrientRotationToMovement;
}

void ANICharacterPlayer::UnFocusing(const EBehaviorState& InBehaviorState)
{
	bIsFocusing = false;
	SetBehaviorState(InBehaviorState);
}

