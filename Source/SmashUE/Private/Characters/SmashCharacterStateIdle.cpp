// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterStateIdle.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterState.h"
#include "Characters/SmashCharacterStateMachine.h"


ESmashCharacterStateID USmashCharacterStateIdle::GetStateID()
{
	return ESmashCharacterStateID::Idle;
}

void USmashCharacterStateIdle::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	Super::StateExit(PreviousStateID);

	Character->PlayAnimMontage(AnimMontage);
}

void USmashCharacterStateIdle::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
}

void USmashCharacterStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	if (FMath::Abs(Character->GetInputMoveX()) > USmashCharacterState::GetInputThreshold())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Walk);
	}
}

