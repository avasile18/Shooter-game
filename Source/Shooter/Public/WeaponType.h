#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_PISTOL UMETA(DisplayName = "PISTOL"),
	EWT_AR UMETA(DisplayName = "AR"),
	EWT_SR UMETA(DisplayName = "SR"),

	EWT_MAX UMETA(DisplayName = "DefaultMAX")
};