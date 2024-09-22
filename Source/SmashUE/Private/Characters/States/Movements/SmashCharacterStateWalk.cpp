// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/Movements/SmashCharacterStateWalk.h"

#include "Characters/SmashCharacter.h"
#include "Characters/States/SmashCharacterState.h"
#include "Characters/States/SmashCharacterStateMachine.h"


ESmashCharacterStateID USmashCharacterStateWalk::GetStateID()
{
	return ESmashCharacterStateID::Walk;
}

void USmashCharacterStateWalk::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->PlayAnimMontage(AnimMontage);

	Character->InputMoveXFastEvent.AddDynamic(this, &USmashCharacterStateWalk::OnInputMoveXFast);
}

void USmashCharacterStateWalk::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	Character->InputMoveXFastEvent.RemoveDynamic(this, &USmashCharacterStateWalk::OnInputMoveXFast);
}

void USmashCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (FMath::Abs(Character->GetInputMoveX()) < USmashCharacterState::GetInputThreshold())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
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
	else
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->Move(MoveSpeed, Character->GetOrientX());
	}
}

void USmashCharacterStateWalk::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}


