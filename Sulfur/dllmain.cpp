#include <Windows.h>
#include <iostream>
#include <thread>

#include "minhook/MinHook.h"
#pragma comment(lib, "minhook/minhook.lib")

#include "SDK.hpp"
#include "Globals.h"
#include "Constants.h"
#include "Util.h"
#include "Player.h"
#include "Beacons.h"
#include "Replication.h"
#include "NetHooks.h"
#include "Hooks.h"

using namespace SDK;

DWORD WINAPI MainThread(LPVOID)
{
    Util::InitConsole();

    SULFUR_LOG("Setting Up!");

    auto BaseAddr = Util::BaseAddress();
    auto GObjectsAddress = BaseAddr + GOBJECTS_OFFSET;
    auto FNameToStringAddress = BaseAddr + FNAMETOSTRING_OFFSET;
    auto FreeMemoryAddress = BaseAddr + FREEMEMORY_OFFSET;

    SDK::UObject::GObjects = decltype(SDK::UObject::GObjects)(GObjectsAddress);
    SDK::FNameToString = decltype(SDK::FNameToString)(FNameToStringAddress);
    SDK::FreeInternal = decltype(SDK::FreeInternal)(FreeMemoryAddress);

    auto FortEngine = SDK::UObject::FindObject<UFortEngine>("FortEngine_");
    Globals::FortEngine = FortEngine;
    Globals::World = FortEngine->GameViewport->World;
    Globals::GPS = reinterpret_cast<UGameplayStatics*>(UGameplayStatics::StaticClass());
    Globals::PC = reinterpret_cast<AFortPlayerController*>(FortEngine->GameInstance->LocalPlayers[0]->PlayerController);

    auto NewConsole = Globals::GPS->STATIC_SpawnObject(UFortConsole::StaticClass(), FortEngine->GameViewport);
    FortEngine->GameViewport->ViewportConsole = (UFortConsole*)(NewConsole);

    auto NewCheatManager = Globals::GPS->STATIC_SpawnObject(UCheatManager::StaticClass(), FortEngine->GameInstance->LocalPlayers[0]->PlayerController);
    FortEngine->GameInstance->LocalPlayers[0]->PlayerController->CheatManager = (UCheatManager*)(NewCheatManager);

    MH_Initialize();

    Hooks::Init();

    SULFUR_LOG("Setup!");

    return 0;
}

BOOL APIENTRY DllMain(HMODULE mod, DWORD reason, LPVOID res)
{
    if (reason == 1)
        CreateThread(0, 0, MainThread, mod, 0, 0);

    return TRUE;
}