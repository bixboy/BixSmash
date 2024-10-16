// Fill out your copyright notice in the Description page of Project Settings.

#include "LocalMultiplayersSubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSettings.h"

void ULocalMultiplayersSubsystem::CreatAndInitPlayers(ELocalMultiplayerInputMappingType MappingType)
{
    ULocalMultiplayerSettings* Settings = GetMutableDefault<ULocalMultiplayerSettings>();
    for (int32 i = 0; i < Settings->GetNbKeyboardProfiles(); ++i)
    {
        FString Error;
        GetGameInstance()->CreateLocalPlayer(i, Error, true);
    }
}

int ULocalMultiplayersSubsystem::GetAssignedPlayerIndexFromKeyProfileIndex(int KeyboardProfileIndex) const
{
    for (int32 PlayerIndex = 0; PlayerIndex < GetGameInstance()->GetNumLocalPlayers(); ++PlayerIndex)
    {
        ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex);
        if (LocalPlayer && LocalPlayer->PlayerController)
        {
            return PlayerIndex;
        }
    }
    return -1;
}

int ULocalMultiplayersSubsystem::AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
	PlayerIndexFromKeyboardProfileIndex.Add(KeyboardProfileIndex, LastAssignedPlayerIndex);
	LastAssignedPlayerIndex++;
	return  PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex];
}

void ULocalMultiplayersSubsystem::AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const
{
	if (PlayerIndex < 0 || PlayerIndex >= GetGameInstance()->GetNumLocalPlayers())
	{
		return;
	}

	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	if (!Settings)
	{
		return;
	}

	if (KeyboardProfileIndex < 0 || KeyboardProfileIndex >= Settings->KeyboardProfilesData.Num())
	{
		return;
	}

	const UInputMappingContext* IMC = Settings->KeyboardProfilesData[KeyboardProfileIndex].GetIMCFromType(MappingType);
	if (!IMC)
	{
		return;
	}

	ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex);
	if (!LocalPlayer || !LocalPlayer->PlayerController)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (InputSubsystem)
	{
		InputSubsystem->AddMappingContext(IMC, 0);
	}
}

int ULocalMultiplayersSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID)
{
	if (PlayerIndexFromGamepadProfileIndex.Contains(DeviceID))
	{
		return PlayerIndexFromGamepadProfileIndex[DeviceID];
	}

	return -1;
}

int ULocalMultiplayersSubsystem::AssignNewPlayerToGamepadDeviceID(int GamepadDeviceID)
{
	if (GamepadDeviceID < 0)
	{
		return -1;
	}

	PlayerIndexFromGamepadProfileIndex.Add(GamepadDeviceID, LastAssignedPlayerIndex);
	LastAssignedPlayerIndex++;
	return LastAssignedPlayerIndex - 1;
}

void ULocalMultiplayersSubsystem::AssignGamepadInputMapping(int PlayerIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	if (PlayerIndex < 0 || PlayerIndex >= GetGameInstance()->GetNumLocalPlayers())
		return;

	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	if (!Settings)
	{
		return;
	}

	if (!Settings->GamepadeProfileData.GetIMCFromType(MappingType))
		return;

	ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex);
	if (!LocalPlayer || !LocalPlayer->PlayerController)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (InputSubsystem)
	{
		InputSubsystem->AddMappingContext(Settings->GamepadeProfileData.GetIMCFromType(MappingType), 0);
	}
}

