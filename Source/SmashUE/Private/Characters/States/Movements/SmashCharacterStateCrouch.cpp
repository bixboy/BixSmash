// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/Movements/SmashCharacterStateCrouch.h"

#include "Characters/SmashCharacter.h"
#include "Characters/States/SmashCharacterState.h"
#include "Characters/States/SmashCharacterStateMachine.h"


ESmashCharacterStateID USmashCharacterStateCrouch::GetStateID()
{
	return ESmashCharacterStateID::Crouch;
}

void USmashCharacterStateCrouch::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->PlayAnimMontage(AnimMontage);

}

void USmashCharacterStateCrouch::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

}

void USmashCharacterStateCrouch::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (FMath::Abs(Character->GetInputMoveZ()) < USmashCharacterState::GetInputThreshold())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	else if (FMath::Abs(Character->GetInputMoveX()) > USmashCharacterState::GetInputThreshold())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Walk);
	}
	else if (Character->GetVelocity().Z < 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}

