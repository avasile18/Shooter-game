// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponType.h"
#include "ShooterAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EOffsetState : uint8
{
	EOS_Aiming UMETA(DisplayName = "Aiming"),
	EOS_Hip UMETA(DisplayName = "Hip"),
	EOS_Reloading UMETA(DisplayName = "Reloading"),
	EOS_InAir UMETA(DisplayName = "InAir"),

	EOS_MAX UMETA(DisplayName = "DefaultMAX")
};

/**
 * 
 */
UCLASS()
class SHOOTER_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UShooterAnimInstance();

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

protected:
	/** Handle turning in place variables */
	void TurnInPlace();

	/** Handle calculations for leaning while running */
	void Lean(float DeltaTime);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class AShooterCharacter* ShooterCharacter;

	/** The speed of the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	/** Whether or not the character is in air */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	/** Whether or not the character is moving */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	/** Offset Yaw used for strafing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw;

	/** Offset Yaw before stopped moving */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;

	/** Yaw of the character this frame; Updated only when standing still */
	float TIPCharacterYaw;

	/** Yaw of the character the previous frame */
	float TIPCharacterYawLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in place", meta = (AllowPrivateAccess = "true"))
	float RootYawOffset;

	/** Rotation curve value this frame */
	float RotationCurve;

	/** Rotation curve value last frame */
	float RotationCurveLastFrame;

	/** The pitch for aim rotation used for aim offset */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in place", meta = (AllowPrivateAccess = "true"))
	float Pitch;

	/** True when reloading, used to prevent aim offset while reloading */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in place", meta = (AllowPrivateAccess = "true"))
	bool bIsReloading;

	/** True when equipping */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in place", meta = (AllowPrivateAccess = "true"))
	bool bIsEquipping;

	/** Offset state used to determine which aim offset to use */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in place", meta = (AllowPrivateAccess = "true"))
	EOffsetState OffsetState;

	/** Yaw of the character this frame */
	FRotator CharacterRotation;

	/** Yaw of the character the previous frame */
	FRotator CharacterRotationLastFrame;

	/** Used for leaning in the running blend space */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lean", meta = (AllowPrivateAccess = "true"))
	float YawDelta;

	/** True when crouching */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crouch", meta = (AllowPrivateAccess = "true"))
	bool bIsCrouching;

	/** Change the recoil weight based on turning in place and aiming */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float RecoilWeight;

	/** True when turning in place */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bIsTurningInPlace;

	/** Type of currently equipped weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	EWeaponType EquippedWeaponType;

	/** True when not reloading or equipping */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bShouldUseFABRIK;
};
