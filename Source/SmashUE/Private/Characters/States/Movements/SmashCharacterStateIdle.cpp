// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/Movements/SmashCharacterStateIdle.h"

#include "Characters/SmashCharacter.h"
#include "Characters/States/SmashCharacterState.h"
#include "Characters/States/SmashCharacterStateMachine.h"


ESmashCharacterStateID USmashCharacterStateIdle::GetStateID()
{
	return ESmashCharacterStateID::Idle;
}

void USmashCharacterStateIdle::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->PlayAnimMontage(AnimMontage);

	Character->InputMoveXFastEvent.AddDynamic(this, &USmashCharacterStateIdle::OnInputMoveXFast);
}

void USmashCharacterStateIdle::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	Character->InputMoveXFastEvent.RemoveDynamic(this, &USmashCharacterStateIdle::OnInputMoveXFast);
}

void USmashCharacterStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	if (FMath::Abs(Character->GetInputMoveX()) > USmashCharacterState::GetInputThreshold())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Walk);
	}
	else if (FMath::Abs(Character->GetInputMoveZ()) > USmashCharacterState::GetInputThreshold() &&
		FMath::Abs(Character->GetInputMoveX()) < USmashCharacterState::GetInputThreshold())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Crouch);
	}
	else if (Character->GetVelocity().Z < 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}

void USmashCharacterStateIdle::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}

