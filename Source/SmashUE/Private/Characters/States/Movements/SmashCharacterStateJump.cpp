// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/Movements/SmashCharacterStateJump.h"

#include "Characters/SmashCharacter.h"
#include "Characters/States/SmashCharacterState.h"
#include "Characters/States/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}

void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	CurrentRotation = Character->GetOrientX();
	Character->SetOrientX(CurrentRotation);

	Character->PlayAnimMontage(AnimMontage);
	Character->GetCharacterMovement()->MaxWalkSpeed = JumpWalkSpeed;
	Character->GetCharacterMovement()->JumpZVelocity = (2 * JumpMaxHeight) / JumpDuration;
	Character->GetCharacterMovement()->GravityScale = (-2 * JumpMaxHeight) / FMath::Square(JumpDuration) / GetWorld()->GetGravityZ();;
	Character->GetCharacterMovement()->AirControl = JumpAirControl;
	Character->Jumping();
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character->GetVelocity().Z < 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
	else
	{
		Character->Move(JumpWalkSpeed, Character->GetOrientX());
		Character->SetOrientX(Character->GetInputMoveX());
	}
}

