// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/Movements/SmashCharacterStateFall.h"

#include "Characters/SmashCharacter.h"
#include "Characters/States/SmashCharacterState.h"
#include "Characters/States/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


ESmashCharacterStateID USmashCharacterStateFall::GetStateID()
{
	return ESmashCharacterStateID::Fall;
}

void USmashCharacterStateFall::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->PlayAnimMontage(AnimMontage);

	Character->GetCharacterMovement()->GravityScale = FallGravityScale;

	Character->GetCharacterMovement()->AirControl = FallAirControl;
	Character->Fall();
}

void USmashCharacterStateFall::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
}

void USmashCharacterStateFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character->GetCharacterMovement()->IsFalling() != true)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	else if (FMath::Abs(Character->GetInputMoveZ()) > USmashCharacterState::GetInputThreshold())
	{
		Character->GetCharacterMovement()->GravityScale = FallFastGravityScale;
	}
	else
	{
		Character->Move(FallHorizontalMoveSpeed, Character->GetOrientX());
		Character->SetOrientX(Character->GetInputMoveX());
	}
}

