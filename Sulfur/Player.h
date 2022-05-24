#pragma once

using namespace SDK;

/* TODO(JACOBB) : Finish the player class for now we are just going to use
* no player class!
*/

class Player
{
private:
	PLAYER_CONTROLLER_CLASS* PlayerController;
	PLAYER_CLASS* PlayerPawn;
public:
	Player(PLAYER_CONTROLLER_CLASS* InController, FVector InLocation, FRotator InRotator)
	{
		PlayerController = InController;
		PlayerPawn = reinterpret_cast<PLAYER_CLASS*>(Util::SpawnActor(PLAYER_CLASS::StaticClass(), InLocation, InRotator));
	}

	PLAYER_CONTROLLER_CLASS* GetPlayerController()
	{
		return PlayerController;
	}

	PLAYER_CLASS* GetPlayerPawn()
	{
		return PlayerPawn;
	}

	bool IsPlayerPawnValid() const
	{
		return (PlayerPawn != nullptr);
	}

	bool IsPlayerControllerValid() const
	{
		return (PlayerController != nullptr);
	}


};