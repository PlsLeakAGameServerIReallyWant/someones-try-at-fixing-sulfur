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
			NetConnection->CurrentNetSpeed = 60000;
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
	    GameState->SetReplicates(true);

        NetReplicator->ReplicateToClient(PlayerController, Connection);
		NetReplicator->ReplicateToClient(PlayerState, Connection);
        NetReplicator->ReplicateToClient(Globals::World->GameState, Connection);
		NetReplicator->ReplicateToClient(GameState, Connection);
		//Globals::GameState->OnRep_MatchState();
		//Globals::GameState->OnRep_GameplayState();
		PlayerState->bHasFinishedLoading = true;
		Globals::PC->ServerSetClientHasFinishedLoading(true);
		Globals::PC->ServerLoadingScreenDropped();
		PlayerState->bHasStartedPlaying = true;
		PlayerState->OnRep_bHasStartedPlaying();
		Globals::PC->bHasInitiallySpawned = true;
		Globals::PC->bReadyToStartMatch = true;
		Globals::PC->bClientPawnIsLoaded = true;
		Globals::PC->bHasClientFinishedLoading = true;
		Globals::PC->bHasServerFinishedLoading = true;
		//Globals::QB->OnRep_PrimaryQuickBar();
		//Globals::Pawn->SetReplicateMovement(true);
		//Globals::Pawn->OnRep_ReplicateMovement();
		Globals::Pawn->OnRep_JumpFlashCount();
		//Globals::Pawn->OnRep_CurrentWeapon();


	    auto IDKAthenaPlayerState = reinterpret_cast<AFortPlayerStateAthena*>(UObject::FindObject<UFortEngine>("FortEngine_")->GameInstance->LocalPlayers[0]->PlayerController->PlayerState);

		auto PlayerStateAthena = reinterpret_cast<AFortPlayerStateAthena*>(Globals::PC->PlayerState);

		PlayerStateAthena->MaxShield = 100;
		PlayerStateAthena->CurrentShield = 100;
		PlayerStateAthena->TeamIndex = EFortTeam::HumanPvP_Team1;
		//PlayerStateAthena->SquadId = IDKAthenaPlayerState->SquadId;
		PlayerStateAthena->OnRep_PlayerTeam();
		PlayerStateAthena->OnRep_SquadId();

		//Globals::Pawn->PawnUniqueID = rand() % 1000;
		//Globals::Pawn->OnRep_PawnUniqueID();
		//Globals::Pawn->OnRep_PlayerState();
		//Globals::PC->OnRep_PlayerState();

		//Globals::PC->Possess(Globals::Pawn);
		//Globals::Pawn->OnRep_PawnUniqueID();
		//Globals::Pawn->OnRep_PlayerState();
		//Globals::PC->PlayerState = Globals::PC->PlayerState;
		//Globals::PC->OnRep_PlayerState();

		//Globals::Pawn->SetMaxHealth(420);
		//Globals::Pawn->SetHealth(420);

		//Globals::Pawn->PlayerState->bIsABot = false;
		//Globals::Pawn->PlayerState->bIsSpectator = false;


        if (PlayerController->Pawn)
        {
			PlayerController->ClientSetViewTarget(PlayerController->Pawn, FViewTargetTransitionParams());
			
            NetReplicator->ReplicateToClient(PlayerController->Pawn, Connection);
        }
		
		if (CurrentGameModeClass == AFortGameModeAthena::StaticClass())
		{
			PlayerController->ClientReturnToMainMenu(L"ATHENA GAMEMODE NOT IMPLEMENTED!\n");
        }

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