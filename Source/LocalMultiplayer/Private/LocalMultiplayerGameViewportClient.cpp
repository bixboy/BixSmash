// Fill out your copyright notice in the Description page of Project Settings.
#include "LocalMultiplayerGameViewportClient.h"
#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayersSubsystem.h"

void ULocalMultiplayerGameViewportClient::PostInitProperties()
{
	Super::PostInitProperties();
	MaxSplitscreenPlayers = 8;
}

bool ULocalMultiplayerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	ULocalMultiplayersSubsystem* subsystem = GetMutableDefault<ULocalMultiplayersSubsystem>();
	if (!subsystem)
		return Super::InputKey(EventArgs);
	
	if (EventArgs.Key.IsGamepadKey())
	{
		int PlayerIndex = subsystem->GetAssignedPlayerIndexFromGamepadDeviceID(EventArgs.ControllerId);
		if (PlayerIndex == -1)
		{
			PlayerIndex = subsystem->AssignNewPlayerToGamepadDeviceID(EventArgs.ControllerId);
		}

		subsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);
	}
	else if (EventArgs.Key.IsValid())
	{
		ULocalMultiplayerSettings* Settings = GetMutableDefault<ULocalMultiplayerSettings>();
		if (!Settings)
			return Super::InputKey(EventArgs);

		const int KeyboardProfileIndex = Settings->FindKeyboardProfileIndexFromKey(EventArgs.Key, ELocalMultiplayerInputMappingType::InGame);
		if (KeyboardProfileIndex != -1)
		{
			int PlayerIndex = subsystem->GetAssignedPlayerIndexFromKeyProfileIndex(KeyboardProfileIndex);
			if (PlayerIndex == -1)
			{
				PlayerIndex = subsystem->AssignNewPlayerToKeyboardProfile(KeyboardProfileIndex);
			}
			
			subsystem->AssignKeyboardMapping(PlayerIndex, KeyboardProfileIndex, ELocalMultiplayerInputMappingType::InGame);
		}
	}

	return Super::InputKey(EventArgs);
}

bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	ULocalMultiplayersSubsystem* subsystem = GetMutableDefault<ULocalMultiplayersSubsystem>();
	if (!subsystem)
		return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
	
	if (bGamepad)
	{
		int PlayerIndex = subsystem->GetAssignedPlayerIndexFromGamepadDeviceID(InputDevice.GetId());
		if (PlayerIndex == -1)
		{
			PlayerIndex = subsystem->AssignNewPlayerToGamepadDeviceID(InputDevice.GetId());
		}

		subsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);
		
		ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex);
		if (LocalPlayer && LocalPlayer->PlayerController)
		{
			LocalPlayer->PlayerController->InputAxis(Key, Delta, DeltaTime, NumSamples, bGamepad);
		}
	}
	return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
}
