// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterStateRun.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterStateMachine.h"


ESmashCharacterStateID USmashCharacterStateRun::GetStateID()
{
	return ESmashCharacterStateID::Walk;
}

void USmashCharacterStateRun::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Super::StateExit(PreviousStateID);

	Character->PlayAnimMontage(AnimMontage);
}

void USmashCharacterStateRun::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
}

void USmashCharacterStateRun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	Super::StateTick(DeltaTime);

	if (FMath::Abs(Character->GetInputMoveX()) < 0.1f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	else
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->Move(MoveSpeed, Character->GetOrientX());
	}
}

