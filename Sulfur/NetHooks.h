#pragma once
#include "SDK/FN_Engine_structs.hpp"
#include "SDK/FN_Engine_classes.hpp"
#include "SDK/FN_FortniteGame_structs.hpp"

using namespace SDK;

namespace NetHooks
{
	Replicator* NetReplicator;
	Beacon* BeaconHost;

	void(*UWorld_NotifyControlMessage)(UWorld* World, UNetConnection* NetConnection, uint8_t a3, void* a4);
	__int64(*WelcomePlayer)(UWorld* This, UNetConnection* NetConnection);
	APlayerController* (*SpawnPlayActor)(UWorld* a1, UPlayer* a2, ENetRole a3, FURL a4, void* a5, FString& Src, uint8_t a7);

	void __fastcall AOnlineBeaconHost_NotifyControlMessageHook(AOnlineBeaconHost* BeaconHost, UNetConnection* NetConnection, uint8_t a3, void* a4)
	{
		if (TO_STRING(a3) == "4") {
			NetConnection->CurrentNetSpeed = 30000;
			return;
		}

		if (TO_STRING(a3) == "15") {
			return;
		}

		SULFUR_LOG("AOnlineBeaconHost::NotifyControlMessage Called! " << TO_STRING(a3).c_str());
		SULFUR_LOG("Channel Count " << TO_STRING(NetConnection->OpenChannels.Num()).c_str());
		return UWorld_NotifyControlMessage(Globals::World, NetConnection, a3, a4);
	}

	__int64 __fastcall WelcomePlayerHook(UWorld*, UNetConnection* NetConnection)
	{
		SULFUR_LOG("Welcoming Player!");
		return WelcomePlayer(Globals::World, NetConnection);
	}

	APlayerController* SpawnPlayActorHook(UWorld*, UNetConnection* Connection, ENetRole NetRole, FURL a4, void* a5, FString& Src, uint8_t a7)
	{
		NetReplicator->InitalizeConnection(Connection);

		auto PlayerController = SpawnPlayActor(Globals::World, Connection, NetRole, a4, a5, Src, a7);
		Connection->PlayerController = PlayerController;
		auto CurrentGameModeClass = Globals::World->AuthorityGameMode->Class;
		auto PlayerState = (AFortPlayerState*)Connection->PlayerController->PlayerState;
		PlayerState->SetOwner(PlayerController);
		PlayerState->SetReplicates(true);
		auto GameState = (AGameStateBase*)Globals::World->GameState;
		GameState->SetOwner(PlayerController);
		GameState->SetReplicates(true);

		NetReplicator->ReplicateToClient(PlayerController, Connection);
		NetReplicator->ReplicateToClient(PlayerState, Connection);
		NetReplicator->ReplicateToClient(Globals::World->GameState, Connection);
		NetReplicator->ReplicateToClient(GameState, Connection);

		if (PlayerController->Pawn)
		{
			PlayerController->ClientSetViewTarget(PlayerController->Pawn, FViewTargetTransitionParams());
			NetReplicator->ReplicateToClient(PlayerController->Pawn, Connection);
		}

		if (CurrentGameModeClass == AFortGameModeAthena::StaticClass())
		{
			PlayerController->ClientReturnToMainMenu(L"ATHENA GAMEMODE NOT IMPLEMENTED!\n");
		}

		//auto Pawn = (APlayerPawn_Athena_C*)Util::SpawnActor(APlayerPawn_Athena_C::StaticClass(). {0, 0, 1000};
		//Pawn->PlayerState = PlayerState;
		//NetReplicator->ReplicateToClient(Pawn, Connection);
		//PlayerController->ClientSetViewTarget(Pawn, FViewTargetTransitionParams());

		PlayerState->bHasStartedPlaying = true;
		PlayerState->bHasFinishedLoading = true;
		PlayerState->bIsReadyToContinue = true;
		PlayerState->OnRep_bHasStartedPlaying();

		//PlayerController->bClientPawnIsLoaded = true;
		//PlayerController->bReadyToStartMatch = true;
		//PlayerController->bAssignedStartSpawn = true;
		//PlayerController->bHasInitiallySpawned = true;
		//PlayerController->bHasClientFinishedLoading = true;
		//PlayerController->bHasServerFinishedLoading = true;
		//PlayerController->OnRep_bHasServerFinishedLoading();

		PlayerState->OnRep_HeroType();

		//PlayerController->Pawn = Pawn;
		//Pawn->Owner = PlayerController;
		//Pawn->OnRep_Owner();
		PlayerController->OnRep_Pawn();
		//PlayerController->Possess(Pawn);

		//Pawn->ServerChoosePart(EFortCustomPartType::Head, UObject::FindObject<UCustomCharacterPart>("CustomCharacterPart F_Med_Head1.F_Med_Head1");
		//Pawn->ServerChoosePart(EFortCustomPartType::Body, UObject::FindObject<UCustomCharacterPart>("CustomCharacterPart F_Med_Soldier_01.F_Med_Soldier_01");
		//PlayerState->OnRep_CharacterParts();

		PlayerController->NetConnection = Connection;
		PlayerController->Player = Connection;
		Connection->PlayerController = PlayerController;
		Connection->OwningActor = PlayerController;
		return PlayerController;

	}

	static void Init()
	{
		auto BaseAddr = Util::BaseAddress();

		UWorld_NotifyControlMessage = decltype(UWorld_NotifyControlMessage)(BaseAddr + 0x1DA15C0);

		auto AOnlineBeaconHost_NotifyControlMessageAddr = BaseAddr + 0x27B7620;
		auto WelcomePlayerAddr = BaseAddr + 0x1DACC50;
		auto SpawnPlayActorAddr = BaseAddr + 0x1A9E7B0;

		SpawnPlayActor = decltype(SpawnPlayActor)(SpawnPlayActorAddr);

		MH_CreateHook((void*)(AOnlineBeaconHost_NotifyControlMessageAddr), AOnlineBeaconHost_NotifyControlMessageHook, nullptr);
		MH_EnableHook((void*)(AOnlineBeaconHost_NotifyControlMessageAddr));
		MH_CreateHook((void*)(WelcomePlayerAddr), WelcomePlayerHook, (void**)(&WelcomePlayer));
		MH_EnableHook((void*)(WelcomePlayerAddr));
		MH_CreateHook((void*)(SpawnPlayActorAddr), SpawnPlayActorHook, (void**)(&SpawnPlayActor));
		MH_EnableHook((void*)(SpawnPlayActorAddr));

		BeaconHost = new Beacon(7777);
		NetReplicator = new Replicator(BeaconHost->GetNetDriver());
	}
}