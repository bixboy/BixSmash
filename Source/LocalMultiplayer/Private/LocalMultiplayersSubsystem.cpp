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
	const ULocalMultiplayerSettings* Settings = GetMutableDefault<ULocalMultiplayerSettings>();
    
	const int32 NbKeyboardProfiles = Settings->GetNbKeyboardProfiles();
	if (KeyboardProfileIndex < 0 || KeyboardProfileIndex >= NbKeyboardProfiles)
	{
		return -1;
	}

	for (int32 PlayerIndex = 0; PlayerIndex < Settings->GetNbKeyboardProfiles(); ++PlayerIndex)
	{
		const FLocalMultiplayerProfileData& PlayerProfileData = Settings->KeyboardProfilesData[PlayerIndex];

		if (PlayerProfileData.ContainsKey(Settings->KeyboardProfilesData[KeyboardProfileIndex].ProfileName, ELocalMultiplayerInputMappingType::InGame))
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
	const ULocalMultiplayerSettings* Settings = GetMutableDefault<ULocalMultiplayerSettings>();
	if (PlayerIndex < 0 || PlayerIndex >= Settings->GetNbKeyboardProfiles())
	{
		return;
	}
	
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

	if (PlayerIndex < 0 || PlayerIndex >= Settings->GetNbKeyboardProfiles())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid PlayerIndex: %d"), PlayerIndex);
		return;
	}

	ULocalPlayer* LocalPlayer = GameInstance->GetLocalPlayerByIndex(PlayerIndex);
	if (!LocalPlayer || !LocalPlayer->PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("LocalPlayer or PlayerController is null for PlayerIndex: %d"), PlayerIndex);
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

