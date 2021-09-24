// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "Uncommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Epic UMETA(DisplayName = "Epic"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),

	EIR_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_PickUp UMETA(DisplayName = "PickUp"),
	EIS_EquipInterping UMETA(DisplayName = "EquipInterping"),
	EIS_PickedUp UMETA(DisplayName = "PickedUp"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Falling UMETA(DisplayName = "Falling"),

	EIS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Ammo UMETA(DisplayName = "Ammo"),
	EIT_Weapon UMETA(DisplayName = "Weapon"),

	EIT_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FItemRarityTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor GlowColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfStars;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* IconBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CustomDepthStencil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier;
};

UCLASS()
class SHOOTER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called when overlapping with area sphere */
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called when overlapping with area sphere has ended*/
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Sets the ActiveStars array of booleans based on rarity */
	void SetActiveStars();

	/** Sets properties of the item components based on state */
	virtual void SetItemProperties(EItemState State);

	/** Called when item interp timer has finished */
	void FinishInterping();

	/** Handles item interpolation when in the equipped interping state */
	void ItemInterp(float DeltaTime);

	/** Get interp location based on item type */
	FVector GetInterpLocation();

	void PlayPickUpSound(bool bForcePlaySound = false);

	virtual void InitializeCustomDepth();

	virtual void OnConstruction(const FTransform& Transform) override;

	void EnableGlowMaterial();

	void UpdatePulse();

	void StartPulseTimer();
	void ResetPulseTimer();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called in AShooterCharacter::GetPickUpItem()
	void PlayEquipSound(bool bForcePlaySound = false);

private:
	/** Skeletal mesh for the item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ItemMesh;

	/** Line trace collides with box to show HUD widgets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox;

	/** Pop up widget for when the player looks at the item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* PickupWidget;

	/** Enables item tracing when overlapped */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AreaSphere;

	/** The name which appears on the pick up widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	FString ItemName;

	/** Item rarity determines the number of stars on widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	EItemRarity ItemRarity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	TArray<bool> ActiveStars;

	/** Item count (ammo etc) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	int32 ItemCount;

	/** State of the item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	EItemState ItemState;

	/** The curve asset to use for the item's Z location when interping */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	class UCurveFloat* ItemZCurve;

	/** Starting location when interp begins */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	FVector ItemInterpStartLocation;

	/** Target interp location in front of the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	FVector CameraTargetLocation;

	/** True when interping */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	bool bIsInterping;

	/** Plays when we start interping */
	FTimerHandle ItemInterpTimer;

	/** Pointer to the character */
	class AShooterCharacter* Character;

	/** Duration of the curve and timer */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	float ZCurveTime;

	/** X and Y for the item while interping in EI state */
	float ItemInterpX;
	float ItemInterpY;

	/** Initial Yaw offset between camera and the interping item */
	float InterpInitialYawOffset;

	/** The curve asset to use for item scaling while interping */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ItemScaleCurve;

	/** Pick up item sound cue */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	class USoundCue* PickUpSound;

	/** Equip item sound cue */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	class USoundCue* EquipSound;

	/** ENUM for the type of the item */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	EItemType ItemType;

	/** Index of the interp location this item is interping to */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	int32 InterpLocIndex;

	/** Index for the material to change at runtime */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	int32 MaterialIndex;

	/** Dynamic instance that can be changed at runtime */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	/** Material instance used with the dynamic material instance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* MaterialInstance;

	bool bCanChangeCustomDepth;

	/** Curve to drive the dynamic material parameters */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	class UCurveVector* PulseCurve;

	/** Curve to drive the dynamic material parameters */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	UCurveVector* InterpPulseCurve;

	FTimerHandle PulseTimer;

	/** Timer for the pulse timer */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	float PulseCurveTime;

	UPROPERTY(VisibleAnywhere, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	float GlowAmount;

	UPROPERTY(VisibleAnywhere, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	float FresnelExponent;

	UPROPERTY(VisibleAnywhere, Category = "Item properties", meta = (AllowPrivateAccess = "true"))
	float FresnelReflectFraction;

	/** Icon for this item in the inventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconItem;

	/** Icon type for this item in the inventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconType;
	
	/** Slot in inventory array */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	int32 SlotIndex;

	/** True when the character's inventory is full */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	bool bIsCharacterInventoryFull;

	/** Item rarity data table */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	class UDataTable* ItemRarityDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	FLinearColor GlowColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	int32 NumberOfStars;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	float DamageMultiplier;

public:
	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return PickupWidget; }

	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }

	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }

	FORCEINLINE EItemState GetItemState() const { return ItemState; }

	void SetItemState(EItemState State);

	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return ItemMesh; }

	/** Called from the AShooterCharacter class */
	void StartItemCurve(AShooterCharacter* Char, bool bForcePlaySound = false);

	FORCEINLINE USoundCue* GetPickUpSound() const { return PickUpSound; }
	FORCEINLINE USoundCue* GetEquipSound() const { return EquipSound; }
	FORCEINLINE void SetPickUpSound(USoundCue* Sound) { PickUpSound = Sound; }
	FORCEINLINE void SetEquipSound(USoundCue* Sound) { EquipSound = Sound; }
	
	FORCEINLINE int32 GetItemCount() const { return ItemCount; }

	virtual void EnableCustomDepth();
	virtual void DisableCustomDepth();

	void DisableGlowMaterial();

	FORCEINLINE int32 GetSlotIndex() const { return SlotIndex; }
	FORCEINLINE void SetSlotIndex(int32 Index) { SlotIndex = Index; }

	FORCEINLINE void SetCharacter(AShooterCharacter* Char) { Character = Char; }

	FORCEINLINE void SetCharacterInventoryFull(bool bIsFull) { bIsCharacterInventoryFull = bIsFull; }

	FORCEINLINE void SetItemName(FString Name) { ItemName = Name; }

	/** Set icons in inventory bar */
	FORCEINLINE void SetItemIcon(UTexture2D* Icon) { IconItem = Icon; }
	/** and pickup widget */
	FORCEINLINE void SetItemType(UTexture2D* Icon) { IconType = Icon; }

	FORCEINLINE void SetMaterialInstance(UMaterialInstance* Instance) { MaterialInstance = Instance; }
	FORCEINLINE UMaterialInstance* GetMaterialInstance() const { return MaterialInstance; }

	FORCEINLINE void SetDynamicMaterialInstance(UMaterialInstanceDynamic* Instance) { DynamicMaterialInstance = Instance; }
	FORCEINLINE UMaterialInstanceDynamic* GetDynamicMaterialInstance() const { return DynamicMaterialInstance; }

	FORCEINLINE FLinearColor GetGlowColor() const { return GlowColor; }

	FORCEINLINE int32 GetMaterialIndex() const { return MaterialIndex; }
	FORCEINLINE void SetMaterialIndex(int32 Index) { MaterialIndex = Index; }

	FORCEINLINE float GetDamageMultiplier() const { return DamageMultiplier; }
};
