#pragma once

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	EAT_9mm UMETA(DisplayName = "9mm"),
	EAT_AR UMETA(DisplayName = "AssaultRifle"),
	EAT_SR UMETA(DisplayName = "SniperRifle"),

	EAT_MAX UMETA(DisplayName = "DefaulMAX")
};