
#pragma once

#include "CoreMinimal.h"
#include "NICharacterPlayerState.generated.h"

UENUM()
enum class EBehaviorState : uint8
{
	EIdle,
	ECrouch,
	EWalk,
	ERun,
	EJump,
	EAttacking,
	EDefensing,
	EFocusing
};