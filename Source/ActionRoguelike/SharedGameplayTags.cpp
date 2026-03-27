#include "SharedGameplayTags.h"

namespace SharedGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Health, "Attribute.Health")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_HealthMax, "Attribute.HealthMax")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_RageAmount, "Attribute.RageAmount")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_RageAmountMax, "Attribute.RageAmountMax")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_MoveSpeedMultiplier, "Attribute.MoveSpeedMultiplier")

	UE_DEFINE_GAMEPLAY_TAG(Action_PrimaryAttack, "Action.PrimaryAttack")
	UE_DEFINE_GAMEPLAY_TAG(Action_SecondaryAttack, "Action.SecondaryAttack")
	UE_DEFINE_GAMEPLAY_TAG(Action_SpecialAttack, "Action.SpecialAttack")
	UE_DEFINE_GAMEPLAY_TAG(Action_Sprint, "Action.Sprint")
	
	UE_DEFINE_GAMEPLAY_TAG(StatusEffect_RageBlock, "StatusEffect.RageBlock")
}