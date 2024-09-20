	// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacter.h"

#include "Characters/SmashCharacterStateMachine.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/SmashCharacterInputData.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ASmashCharacter::ASmashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASmashCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateStateMachine();
	
	InitStateMachine();

	FallHorizontalMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	FallGravityScale = GetCharacterMovement()->GravityScale;
}

// Called every frame
void ASmashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickStateMachine(DeltaTime);
	RotateMeshUsingOrientX();
}

// Called to bind functionality to input
void ASmashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupMappingContextInToController();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent == nullptr) return;

	BindInputMoveXAxisAndActions(EnhancedInputComponent);
}

#pragma region Orient X

float ASmashCharacter::GetOrientX() const
{
	return OrientX;
}

void ASmashCharacter::SetOrientX(float NewOrientX)
{
	OrientX = NewOrientX;
}	

void ASmashCharacter::RotateMeshUsingOrientX() const
{
	FRotator Rotation = GetMesh()->GetRelativeRotation();
	Rotation.Yaw = -90.f * OrientX;
	GetMesh()->SetRelativeRotation(Rotation);
}

#pragma endregion

#pragma region State Machine

void ASmashCharacter::CreateStateMachine()
{
	StateMachine = NewObject<USmashCharacterStateMachine>(this);
}

void ASmashCharacter::InitStateMachine()
{
	if (StateMachine == nullptr) return;
	StateMachine->Init(this);
}

void ASmashCharacter::TickStateMachine(float DeltaTime) const
{
	if (StateMachine == nullptr) return;
	StateMachine->Tick(DeltaTime);
}

#pragma endregion

void ASmashCharacter::SetupMappingContextInToController() const
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController == nullptr) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if (Player == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (InputSystem == nullptr) return;

	InputSystem->AddMappingContext(InputMappingContext, 0);
}

	float ASmashCharacter::GetInputMoveX() const
	{
		return InputMoveX;
	}

	float ASmashCharacter::GetInputMoveZ() const
	{
		return  InputMoveZ;
	}

	void ASmashCharacter::BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent)
	{
		if(InputComponent == nullptr) return;

		if (InputData->InputActionMoveX)
		{
			EnhancedInputComponent->BindAction(
				InputData->InputActionMoveX,
				ETriggerEvent::Started,
				this,
				&ASmashCharacter::OnInputMoveX
			);

			EnhancedInputComponent->BindAction(
				InputData->InputActionMoveX,
				ETriggerEvent::Completed,
				this,
				&ASmashCharacter::OnInputMoveX
			);
			
			EnhancedInputComponent->BindAction(
				InputData->InputActionMoveX,
				ETriggerEvent::Triggered,
				this,
				&ASmashCharacter::OnInputMoveX
			);
		}

		if (InputData->InputActionMoveXFast)
		{
			EnhancedInputComponent->BindAction(
				InputData->InputActionMoveXFast,
				ETriggerEvent::Triggered,
				this,
				&ASmashCharacter::OnInputMoveXFast
				);
		}

		if (InputData->InputActionMoveZ)
		{
			EnhancedInputComponent->BindAction(
				InputData->InputActionMoveZ,
				ETriggerEvent::Started,
				this,
				&ASmashCharacter::OnInputMoveZ
			);

			EnhancedInputComponent->BindAction(
				InputData->InputActionMoveZ,
				ETriggerEvent::Completed,
				this,
				&ASmashCharacter::OnInputMoveZ
			);
			
			EnhancedInputComponent->BindAction(
				InputData->InputActionMoveZ,
				ETriggerEvent::Triggered,
				this,
				&ASmashCharacter::OnInputMoveZ
			);
		}

		if (InputData->InputActionJump)
		{
			EnhancedInputComponent->BindAction(
				InputData->InputActionJump,
				ETriggerEvent::Started,
				this,
				&ASmashCharacter::OnInputJump
			);
		}
	}

#pragma region MovementInput

	void ASmashCharacter::OnInputMoveX(const FInputActionValue& InputActionValue)
	{
		InputMoveX = InputActionValue.Get<float>();
	}

	void ASmashCharacter::OnInputMoveXFast(const FInputActionValue& InputActionValue)
	{
		InputMoveX = InputActionValue.Get<float>();
		InputMoveXFastEvent.Broadcast(InputMoveX);
	}

	void ASmashCharacter::OnInputMoveZ(const FInputActionValue& InputActionValue)
	{
		InputMoveZ = InputActionValue.Get<float>();
	}

	void ASmashCharacter::Move(float Speed, float Orient)
	{
		GetCharacterMovement()-> MaxWalkSpeed = Speed;
		AddMovementInput(GetActorForwardVector(), Orient);
	}

#pragma endregion

#pragma region Jump Actions
	void ASmashCharacter::OnInputJump(const FInputActionValue& InputActionValue)
	{
	if (StateMachine && GetCharacterMovement()->IsMovingOnGround())
    	{
    	    StateMachine->ChangeState(ESmashCharacterStateID::Jump);
    	}
	}

	void ASmashCharacter::Jumping()
	{
		Jump();
	}

	void ASmashCharacter::Fall()
	{
		StopJumping();
	}

	void ASmashCharacter::Landed(const FHitResult& Hit)
	{
		Super::Landed(Hit);
	    GetCharacterMovement()->MaxWalkSpeed = FallHorizontalMoveSpeed;
		GetCharacterMovement()->GravityScale = FallGravityScale;
	}
#pragma endregion
