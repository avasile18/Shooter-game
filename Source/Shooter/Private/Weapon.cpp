// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

AWeapon::AWeapon() :
	ThrowWeaponTime(1.f),
	bIsFalling(false),
	ImpulseAmplifier(4'000.f),
	WeaponType(EWeaponType::EWT_AR),
	AmmoType(EAmmoType::EAT_AR),
	MagazineCapacity(32),
	Ammo(0),
	ReloadMontageSection(FName(TEXT("Reload AR"))),
	ClipBoneName(TEXT("Clip_Bone")),
	SlideDisplacement(0.f),
	SlideDisplacementTime(0.25f),
	bIsMovingSlide(false),
	MaxSlideDisplacement(1.f),
	MaxRecoilRotation(20.f),
	bCanAuto(true)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Keep the weapon upright
	if (GetItemState() == EItemState::EIS_Falling && bIsFalling)
	{
		const FRotator MeshRotation{ 0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f };
		GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}

	// Update slide on pistol
	UpdateSlideDisplacement();
}

void AWeapon::ThrowWeapon()
{
	FRotator MeshRotation{ 0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f };
	GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

	const FVector MeshForward{ GetItemMesh()->GetForwardVector() };
	const FVector MeshRight{ GetItemMesh()->GetRightVector() };

	// Direction in which the weapon is thrown
	FVector ImpulseDirection = MeshRight.RotateAngleAxis(-20.f, MeshForward);

	float RandomRotation{ FMath::FRandRange(0.f, 60.f) };
	ImpulseDirection = ImpulseDirection.RotateAngleAxis(RandomRotation, FVector(0.f, 0.f, 1.f));
	ImpulseDirection *= ImpulseAmplifier;
	GetItemMesh()->AddImpulse(ImpulseDirection);

	bIsFalling = true;
	GetWorldTimerManager().SetTimer(ThrowWeaponTimer, this, &AWeapon::StopFalling, ThrowWeaponTime);

	EnableGlowMaterial();
}

void AWeapon::DecrementAmmo()
{
	if (Ammo - 1 <= 0)
	{
		Ammo = 0;
	}
	else
	{
		--Ammo;
	}
}

void AWeapon::ReloadAmmo(int32 Amount)
{
	checkf(Ammo + Amount <= MagazineCapacity, TEXT("Attempted to reload with more than magazine capacity!"));
	Ammo += Amount;
}

bool AWeapon::IsMagazineFull()
{
	return Ammo >= MagazineCapacity;
}

void AWeapon::StartSlideTimer()
{
	bIsMovingSlide = true;
	GetWorldTimerManager().SetTimer(SlideTimer, this, &AWeapon::FinishMovingSlide, SlideDisplacementTime);
}

void AWeapon::StopFalling()
{
	bIsFalling = false;
	SetItemState(EItemState::EIS_PickUp);

	StartPulseTimer();
}

void AWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	const FString WeaponTablePath{ TEXT("DataTable'/Game/_Game/DataTables/WeaponDataTable.WeaponDataTable'") };
	UDataTable* WeaponTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *WeaponTablePath));

	if (WeaponTableObject)
	{
		FWeaponDataTable* WeaponDataRow = nullptr;

		switch (WeaponType)
		{
		case EWeaponType::EWT_PISTOL:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("PISTOL"), TEXT(""));
			break;
		case EWeaponType::EWT_AR:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("AR"), TEXT(""));
			break;
		case EWeaponType::EWT_SR:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("SR"), TEXT(""));
			break;
		case EWeaponType::EWT_MAX:
			break;
		default:
			break;
		}

		if (WeaponDataRow)
		{
			AmmoType = WeaponDataRow->AmmoType;
			Ammo = WeaponDataRow->WeaponAmmo;
			MagazineCapacity = WeaponDataRow->MagazineCapacity;
			SetPickUpSound(WeaponDataRow->PickUpSound);
			SetEquipSound(WeaponDataRow->EquipSound);
			GetItemMesh()->SetSkeletalMesh(WeaponDataRow->ItemMesh);
			SetItemName(WeaponDataRow->ItemName);
			SetItemIcon(WeaponDataRow->InventoryIcon);
			SetItemType(WeaponDataRow->AmmoIcon);
			SetMaterialInstance(WeaponDataRow->MaterialInstance);

			PreviousMaterialIndex = GetMaterialIndex();

			GetItemMesh()->SetMaterial(PreviousMaterialIndex, nullptr);
			SetMaterialIndex(WeaponDataRow->MaterialIndex);

			SetClipBoneName(WeaponDataRow->ClipBoneName);
			SetReloadMontageSection(WeaponDataRow->ReloadMontageSection);
			GetItemMesh()->SetAnimInstanceClass(WeaponDataRow->AnimBP);

			CrosshairMiddle = WeaponDataRow->CrosshairMiddle;
			CrosshairLeft = WeaponDataRow->CrosshairLeft;
			CrosshairTop = WeaponDataRow->CrosshairTop;
			CrosshairBot = WeaponDataRow->CrosshairBot;
			CrosshairRight = WeaponDataRow->CrosshairRight;

			AutoFireRate = WeaponDataRow->AutoFireRate;
			MuzzleFlash = WeaponDataRow->MuzzleFlash;
			FireSound = WeaponDataRow->FireSound;
			ZoomInSound = WeaponDataRow->ZoomInSound;
			ZoomOutSound = WeaponDataRow->ZoomOutSound;
			ImpactVFX = WeaponDataRow->ImpactVFX;

			bCanAuto = WeaponDataRow->bIsAuto;

			Damage = WeaponDataRow->Damage * GetDamageMultiplier();
			HeadShotDamage = WeaponDataRow->Damage * 2.5f * GetDamageMultiplier();
		}

		if (GetMaterialInstance())
		{
			SetDynamicMaterialInstance(UMaterialInstanceDynamic::Create(GetMaterialInstance(), this));
			GetDynamicMaterialInstance()->SetVectorParameterValue(TEXT("EmissiveColor"), GetGlowColor());
			GetDynamicMaterialInstance()->SetVectorParameterValue(TEXT("Fresnel Color"), GetGlowColor());
			GetItemMesh()->SetMaterial(GetMaterialIndex(), GetDynamicMaterialInstance());

			EnableGlowMaterial();
		}
	}
}

void AWeapon::FinishMovingSlide()
{
	bIsMovingSlide = false;
}

void AWeapon::UpdateSlideDisplacement()
{
	if (SlideDisplacementCurve && bIsMovingSlide)
	{
		const float ELapsedTime{ GetWorldTimerManager().GetTimerElapsed(SlideTimer) };
		const float CurveValue{ SlideDisplacementCurve->GetFloatValue(ELapsedTime) };

		SlideDisplacement = CurveValue * MaxSlideDisplacement;
		RecoilRotation = CurveValue * MaxRecoilRotation;
	}
}
