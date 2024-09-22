// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h" 
#include "GameFramework/Character.h"
#include "Interfaces/SmashCharacterHit.h"
#include "SmashCharacter.generated.h"

struct FInputActionValue;
class USmashCharacterInputData;
class UInputMappingContext;
class USmashCharacterStateMachine;

UCLASS()
class SMASHUE_API ASmashCharacter : public ACharacter, public ISmashCharacterHit
{
	GENERATED_BODY()
	
#pragma region Unreal Default
	
public:
	ASmashCharacter();
	float FallHorizontalMoveSpeed;
	float FallGravityScale;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma endregion

#pragma region Orient

public:
	float GetOrientX() const;

	void SetOrientX(float NewOrientX);

protected:
	UPROPERTY(BlueprintReadOnly)
	float OrientX = 1.f;

	void RotateMeshUsingOrientX() const;

#pragma endregion

#pragma region State Machine

public:
	void CreateStateMachine();

	void InitStateMachine();

	void TickStateMachine(float DeltaTime) const;

	void Move(float Speed, float Orient);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USmashCharacterStateMachine> StateMachine;
	
#pragma endregion

#pragma region Input Data / Mapping Context

public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<USmashCharacterInputData> InputData;

protected:
	void SetupMappingContextInToController() const;

#pragma endregion	

#pragma region Input Move X

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveXEvent, float, InputMoveX);
	
public:
	float GetInputMoveX() const;

	float GetInputMoveZ() const;

	UPROPERTY()
	FInputMoveXEvent InputMoveXFastEvent;

	void Jumping();

	void Fall();

	void Landed(const FHitResult& Hit) override;

protected:
	UPROPERTY()
	float InputMoveX = 0.f;

	UPROPERTY()
	float InputMoveZ = 0.f;

private:
	void BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);

	void OnInputMoveX(const FInputActionValue& InputActionValue);

	void OnInputMoveXFast(const FInputActionValue& InputActionValue);

	void OnInputMoveZ(const FInputActionValue& InputActionValue);

	void OnInputJump(const FInputActionValue& InputActionValue);
	
	UPROPERTY(EditAnywhere)
	int MaxJump = 1;

	int JumpIndex = 0;

#pragma endregion

#pragma region Attack

public:
	void EndAttack();

	bool GetIsAttacking() const;

	virtual void TakeDamage_Implementation(FHitResult HitResult) override;

	virtual void StartAttackTrace_Implementation() override;

	virtual void StopAttackTrace_Implementation() override;

protected:
	FTimerHandle SpawnTraceTimerHandle;
	
private:
	
	void OnInputAttack(const FInputActionValue& InputActionValue);

	void AttackTraceLoop();
	
	bool InAttack = false;

	bool CanHit = true;

	UPROPERTY(EditAnywhere)
	FName StartTrace;

	UPROPERTY(EditAnywhere)
	float AttackRange = 10.f;

	UPROPERTY(EditAnywhere)
	float TraceRadius = 20.f;
	
	UPROPERTY(EditAnywhere)
	float KnockBackStrength = 100000.f;

	UPROPERTY()
	TArray<AActor*> HitActors;
	
#pragma endregion
};

