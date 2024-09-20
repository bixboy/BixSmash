// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/Movements/SmashCharacterStateRun.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterStateMachine.h"


ESmashCharacterStateID USmashCharacterStateRun::GetStateID()
{
	return ESmashCharacterStateID::Run;
}

void USmashCharacterStateRun::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	
	Character->PlayAnimMontage(AnimMontage);
}

void USmashCharacterStateRun::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
}

void USmashCharacterStateRun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	if (FMath::Abs(Character->GetInputMoveX()) < 0.1f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
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

