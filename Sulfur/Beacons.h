#pragma once

using namespace SDK;

namespace Beacons
{
	namespace Functions
	{
		bool(*InitHost)(AOnlineBeaconHost*);
	}
}

class Beacon
{
private:
	AOnlineBeaconHost* BeaconHost;
public:
	
	Beacon(int Port)
	{
		SULFUR_LOG("Setting up BeaconHost!");

		auto SpawnTransform = FTransform();
		SpawnTransform.Rotation = FQuat();
		SpawnTransform.Scale3D = FVector(0, 0, 0);
		SpawnTransform.Translation = FVector(0, 0, 5000);

		Globals::GPS = decltype(Globals::GPS)(UGameplayStatics::StaticClass());

		auto SpawningBeaconActor = Globals::GPS->STATIC_BeginSpawningActorFromClass(Globals::World, AOnlineBeaconHost::StaticClass(), SpawnTransform, true, nullptr);
		BeaconHost = reinterpret_cast<AOnlineBeaconHost*>(Globals::GPS->STATIC_FinishSpawningActor(SpawningBeaconActor, SpawnTransform));

		BeaconHost->ListenPort = 7777;

		if (Beacons::Functions::InitHost(BeaconHost)) {
			BeaconHost->BeaconState = 0;
			SULFUR_LOG("BeaconHost is now listening and accepting requests!");
		}
	}

	~Beacon()
	{
		SULFUR_LOG("Destructing BeaconHost!");

		BeaconHost->K2_DestroyActor();
		BeaconHost = NULL;
	}

	bool IsBeaconValid() const
	{
		return (BeaconHost != nullptr);
	}

	AOnlineBeaconHost* GetBeaconHost()
	{
		return BeaconHost;
	}

	UNetDriver* GetNetDriver()
	{
		if (IsBeaconValid()) {
			return BeaconHost->NetDriver;
		}
	}

	TArray<UNetConnection*> GetClientConnections()
	{
		if (IsBeaconValid()) {
			return GetNetDriver()->ClientConnections;
		}
	}

	UWorld* GetWorldFromNetDriver()
	{
		if (IsBeaconValid()) {
			return GetNetDriver()->World;
		}
	}

	static void InitOffsets()
	{
		SULFUR_LOG("Setting up beacon offsets!");

		auto BaseAddr = Util::BaseAddress();

		Beacons::Functions::InitHost = decltype(Beacons::Functions::InitHost)(BaseAddr + INITHOST_OFFSET);
	}
};