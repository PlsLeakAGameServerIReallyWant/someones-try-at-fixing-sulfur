#pragma once

#include "SDK.hpp"

#include "minhook/MinHook.h"

#pragma comment(lib, "minhook/minhook.lib")

using namespace SDK;

namespace Hooks
{
	bool bIsReady = false;
	bool bHasSpawned = false;
	bool bIsInGame = false;
	bool bHasInitedTheBeacon = false;

	LPVOID(*ProcessEvent)(void*, void*, void*);
	LPVOID ProcessEventHook(UObject* pObject, UFunction* pFunction, LPVOID pParams)
	{
		SULFUR_LOG(pFunction->GetName());
		if (pFunction->GetName().find("BP_PlayButton") != std::string::npos)
		{
			Globals::PC->SwitchLevel(TEXT("Athena_Faceoff?game=Engine.GameMode"));
			bIsReady = true;
		}

		if (pFunction->GetName().find("ReceiveTick") != std::string::npos && pObject == Globals::PC && NetHooks::BeaconHost != NULL)
		{
			if (NetHooks::BeaconHost->IsBeaconValid())
			{
				if (NetHooks::BeaconHost->GetNetDriver())
				{
					if (NetHooks::BeaconHost->GetNetDriver()->ClientConnections.Num() != 0)
					{
						auto DeltaSeconds = ((AActor_ReceiveTick_Params*)pParams)->DeltaSeconds;
						NetHooks::NetReplicator->Tick(DeltaSeconds);
					}
				}
			}
		}

		//if (pFunction->GetName().find("Tick") != std::string::npos)
		{
			if (GetAsyncKeyState(VK_F1) & 0x1)
			{
				if (!bHasInitedTheBeacon) {
					Replicator::InitOffsets();
					Beacon::InitOffsets();

					NetHooks::Init();
					bHasInitedTheBeacon = true;
				}
			}

			if (GetAsyncKeyState(VK_F2) & 0x1 && Globals::PC)
			{
				if (Globals::PC->Pawn)
				{
					auto Location = Globals::PC->Pawn->K2_GetActorLocation();
					auto NewFortPickup = reinterpret_cast<AFortPickup*>(Util::SpawnActor(AFortPickup::StaticClass(), Location, FRotator()));

					NewFortPickup->PrimaryPickupItemEntry.Count = 1;
					NewFortPickup->PrimaryPickupItemEntry.ItemDefinition = UObject::FindObject<UFortWeaponItemDefinition>("WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03");
					NewFortPickup->OnRep_PrimaryPickupItemEntry();

					NewFortPickup->TossPickup(Location, nullptr, 1, true);

					NetHooks::NetReplicator->Replicate(NewFortPickup); //Replicates the pickup
				}
			}

			if (GetAsyncKeyState(VK_F3) & 0x1 && Globals::PC)
			{
				if (Globals::PC->Pawn)
				{
					auto Location = Globals::PC->Pawn->K2_GetActorLocation();
					auto NewFortPickup = reinterpret_cast<AFortPickup*>(Util::SpawnActor(AFortPickup::StaticClass(), Location, FRotator()));

					NewFortPickup->PrimaryPickupItemEntry.Count = 1;
					NewFortPickup->PrimaryPickupItemEntry.ItemDefinition = UObject::FindObject<UFortWeaponItemDefinition>("WID_Edged_Sword_Medium_Laser_SR_Ore_T06.WID_Edged_Sword_Medium_Laser_SR_Ore_T06");
					NewFortPickup->OnRep_PrimaryPickupItemEntry();

					NewFortPickup->TossPickup(Location, nullptr, 1, true);

					NetHooks::NetReplicator->Replicate(NewFortPickup); //Replicates the pickup
				}
			}

			if (GetAsyncKeyState(VK_F4) & 0x1 && Globals::PC)
			{
				if (Globals::PC->Pawn)
				{
					auto Location = Globals::PC->Pawn->K2_GetActorLocation();
					auto NewFortPickup = reinterpret_cast<AFortPickup*>(Util::SpawnActor(AFortPickup::StaticClass(), Location, FRotator()));

					NewFortPickup->PrimaryPickupItemEntry.Count = 1;
					NewFortPickup->PrimaryPickupItemEntry.ItemDefinition = UObject::FindObject<UFortWeaponItemDefinition>("ShotGod.ShotGod");
					NewFortPickup->OnRep_PrimaryPickupItemEntry();

					NewFortPickup->TossPickup(Location, nullptr, 1, true);

					NetHooks::NetReplicator->Replicate(NewFortPickup); //Replicates the pickup
				}
			}

			if (GetAsyncKeyState(VK_F5) & 0x1 && Globals::PC)
			{
				if (Globals::PC->Pawn)
				{
					auto Location = Globals::PC->Pawn->K2_GetActorLocation();
					auto NewFortPickup = reinterpret_cast<AFortPickup*>(Util::SpawnActor(AFortPickup::StaticClass(), Location, FRotator()));

					NewFortPickup->PrimaryPickupItemEntry.Count = 1;
					NewFortPickup->PrimaryPickupItemEntry.ItemDefinition = UObject::FindObject<UFortWeaponItemDefinition>("TestGod.TestGod");
					NewFortPickup->OnRep_PrimaryPickupItemEntry();

					NewFortPickup->TossPickup(Location, nullptr, 1, true);

					NetHooks::NetReplicator->Replicate(NewFortPickup); //Replicates the pickup
				}
			}

			if (GetAsyncKeyState(VK_F6) & 0x1 && Globals::PC)
			{
				if (Globals::PC->Pawn)
				{
					auto Location = Globals::PC->Pawn->K2_GetActorLocation();
					auto NewFortPickup = reinterpret_cast<AFortPickup*>(Util::SpawnActor(AFortPickup::StaticClass(), Location, FRotator()));

					NewFortPickup->PrimaryPickupItemEntry.Count = 1;
					NewFortPickup->PrimaryPickupItemEntry.ItemDefinition = UObject::FindObject<UFortWeaponItemDefinition>("WID_Troll_Launcher_Durability.WID_Troll_Launcher_Durability");
					NewFortPickup->OnRep_PrimaryPickupItemEntry();

					NewFortPickup->TossPickup(Location, nullptr, 1, true);

					NetHooks::NetReplicator->Replicate(NewFortPickup); //Replicates the pickup
				}
			}

			if (GetAsyncKeyState(VK_F7) & 0x1 && Globals::PC)
			{
				if (Globals::PC->Pawn)
				{
					auto Location = Globals::PC->Pawn->K2_GetActorLocation();
					auto NewFortPickup = reinterpret_cast<AFortPickup*>(Util::SpawnActor(AFortPickup::StaticClass(), Location, FRotator()));

					NewFortPickup->PrimaryPickupItemEntry.Count = 1;
					NewFortPickup->PrimaryPickupItemEntry.ItemDefinition = UObject::FindObject<UFortWeaponItemDefinition>("PermaniteItemData.PermaniteItemData");
					NewFortPickup->OnRep_PrimaryPickupItemEntry();

					NewFortPickup->TossPickup(Location, nullptr, 1, true);

					NetHooks::NetReplicator->Replicate(NewFortPickup); //Replicates the pickup
				}
			}

			if (GetAsyncKeyState(VK_F8) & 0x1 && Globals::PC)
			{
				if (Globals::PC->Pawn)
				{
					auto Location = Globals::PC->Pawn->K2_GetActorLocation();
					auto NewFortPickup = reinterpret_cast<AFortPickup*>(Util::SpawnActor(AFortPickup::StaticClass(), Location, FRotator()));

					NewFortPickup->PrimaryPickupItemEntry.Count = 1;
					NewFortPickup->PrimaryPickupItemEntry.ItemDefinition = UObject::FindObject<UFortWeaponItemDefinition>("WannaGun.Wannagun");
					NewFortPickup->OnRep_PrimaryPickupItemEntry();

					NewFortPickup->TossPickup(Location, nullptr, 1, true);

					NetHooks::NetReplicator->Replicate(NewFortPickup); //Replicates the pickup
				}
			}
			if (GetAsyncKeyState(VK_F9) & 0x1 && Globals::PC) {

				Globals::PC->Possess(Globals::Pawn);
				SULFUR_LOG("Possessed Player!");
		}
			if (GetAsyncKeyState(VK_F12) & 0x1 && Globals::PC) {

				Globals::PC->UnPossess();
				SULFUR_LOG("Unpossessed Player!");

			}
		}
		if (GetAsyncKeyState(VK_F10) & 0x01)
		{
			auto FortEngine = UObject::FindObject<UFortEngine>("FortEngine_");
			Globals::PC = reinterpret_cast<AFortPlayerController*>(FortEngine->GameInstance->LocalPlayers[0]->PlayerController);
			auto GPS = reinterpret_cast<UGameplayStatics*>(UGameplayStatics::StaticClass());
			auto FortCheatManager = reinterpret_cast<UFortCheatManager*>(Globals::PC->CheatManager);

			auto NewCheatManager = GPS->STATIC_SpawnObject(UCheatManager::StaticClass(), FortEngine->GameInstance->LocalPlayers[0]->PlayerController);
			FortEngine->GameInstance->LocalPlayers[0]->PlayerController->CheatManager = (UCheatManager*)(NewCheatManager);

			Globals::PC->CheatManager->DestroyAll(AFortHLODSMActor::StaticClass());

			auto FortHero = UObject::FindObject<UFortHero>("FortHero Transient.FortHero_");

			TArray<AActor*> OutActors;
			GPS->STATIC_GetAllActorsOfClass(FortEngine->GameViewport->World, APlayerStart::StaticClass(), &OutActors);
			auto ActorsNum = OutActors.Num();
			auto ActorToUseNum = rand() % ActorsNum;
			auto ActorToUse = OutActors[ActorToUseNum];

			auto SpawnTransform = FTransform();
			SpawnTransform.Scale3D = FVector(1, 1, 1);
			SpawnTransform.Rotation = FQuat();
			SpawnTransform.Translation = ActorToUse->K2_GetActorLocation();

			auto NewConsole = GPS->STATIC_SpawnObject(UFortConsole::StaticClass(), FortEngine->GameViewport);
			FortEngine->GameViewport->ViewportConsole = static_cast<UFortConsole*>(NewConsole);

			auto SpawningActor = GPS->STATIC_BeginSpawningActorFromClass(FortEngine->GameViewport->World, APlayerPawn_Generic_C::StaticClass(), SpawnTransform, false, nullptr);
			Globals::PlayerPawn = reinterpret_cast<AFortPlayerPawn*>(GPS->STATIC_FinishSpawningActor(SpawningActor, SpawnTransform));
			//Globals::PlayerPawn->bCanBeDamaged = false;

			auto FortPlayerStateZone = reinterpret_cast<AFortPlayerStateZone*>(Globals::PC->PlayerState);

			Globals::PC->Possess(Globals::PlayerPawn);

			Globals::PlayerPawn->EquipWeaponDefinition(UObject::FindObject<UFortWeaponItemDefinition>("WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03"), FGuid());

			FortPlayerStateZone->CharacterParts[0] = UObject::FindObject<UCustomCharacterPart>("CustomCharacterPart F_Med_Head1.F_Med_Head1");
			FortPlayerStateZone->CharacterParts[1] = UObject::FindObject<UCustomCharacterPart>("CustomCharacterPart F_Med_Soldier_01.F_Med_Soldier_01");
			FortPlayerStateZone->OnRep_CharacterParts();

			bIsInGame = true;
			bIsReady = false;
			bHasSpawned = true;
		}

		/*if (pFunction->GetName().find("ReadyToStartMatch") != std::string::npos && bIsReady)
		{
			Globals::FortEngine = UObject::FindObject<UFortEngine>("FortEngine_");
			Globals::World = Globals::FortEngine->GameViewport->World;
			Globals::PC = reinterpret_cast<AFortPlayerController*>(Globals::FortEngine->GameInstance->LocalPlayers[0]->PlayerController);

			if (!bHasSpawned) {
				Globals::Pawn = reinterpret_cast<PLAYER_CLASS*>(Util::SpawnActor(PLAYER_CLASS::StaticClass(), FVector(0, 0, 0), FRotator()));

				auto PlayerState = reinterpret_cast<PLAYER_STATE_CLASS*>(Globals::PC->PlayerState);
				PlayerState->CharacterParts[0] = UObject::FindObject<UCustomCharacterPart>("CustomCharacterPart F_Med_Head1.F_Med_Head1");
				PlayerState->CharacterParts[1] = UObject::FindObject<UCustomCharacterPart>("CustomCharacterPart F_Med_Soldier_01.F_Med_Soldier_01");
				PlayerState->OnRep_CharacterParts();

				Globals::PC->Possess(Globals::Pawn);

				bIsReady = false;
				bIsInGame = true;
				bHasSpawned = true;

				Globals::PC->ServerReadyToStartMatch();
				static_cast<AGameMode*>(Globals::World->AuthorityGameMode)->StartMatch();
			}
		} */

		if (pFunction->GetName().find("LoadingScreenDropped") != std::string::npos)
		{
		}

		if (pFunction->GetName().find("ServerShortTimeout") != std::string::npos) {
			return NULL;
		}
		if (pFunction->GetName().find("PostLogin") != std::string::npos) {
			SULFUR_LOG("PostLogin Function Call Received");
			auto LoginParams = (SDK::AGameModeBase_K2_PostLogin_Params*)pParams;
			Util::SpawnActor(PLAYER_CLASS::StaticClass(), FVector(0, 0, 2900), FRotator());
			LoginParams->NewPlayer->Possess(Globals::Pawn);
		}

		return ProcessEvent(pObject, pFunction, pParams);
	}

	static void Init()
	{
		auto FEVFT = *reinterpret_cast<void***>(Globals::FortEngine);
		auto PEAddr = FEVFT[64];

		MH_CreateHook(reinterpret_cast<LPVOID>(PEAddr), ProcessEventHook, reinterpret_cast<LPVOID*>(&ProcessEvent));
		MH_EnableHook(reinterpret_cast<LPVOID>(PEAddr));
	}
}