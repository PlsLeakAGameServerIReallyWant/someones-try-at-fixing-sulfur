#pragma once

using namespace SDK;

namespace Replication
{
	namespace Functions
	{
		bool(*ReplicateActor)(UActorChannel*);
		__int64(*SetChannelActor)(UActorChannel*, AActor*);
		UChannel* (*CreateChannel)(UNetConnection*, int, bool, int32_t);
	}

	namespace Enums
	{
		enum EChannelType
		{
			CHTYPE_None = 0,
			CHTYPE_Control = 1,
			CHTYPE_Actor = 2,
			CHTYPE_File = 3,
			CHTYPE_Voice = 4,
			CHTYPE_MAX = 8
		};
	}

	namespace Movement
	{
		namespace Functions
		{
			void (*ClientSendAdjustment)(APlayerController*);
		}
	}
}

class Replicator
{
private:
	UNetDriver* NetDriver;

public:
	Replicator(UNetDriver* InNetDriver)
	{
		NetDriver = InNetDriver;
	}

	bool IsNetDriverValid() const
	{
		return (NetDriver != nullptr);
	}

	UNetDriver* GetNetDriver()
	{
		return NetDriver;
	}

	bool Replicate(AActor* InActor)
	{
		int32_t ClientsThatReplicatedIt = 0;

		if (IsNetDriverValid()) {
			for (int i = 0; i < NetDriver->ClientConnections.Num(); i++)
			{
				auto Connection = NetDriver->ClientConnections[i];

				if (Connection)
				{
					auto Channel = (UActorChannel*)(Replication::Functions::CreateChannel(Connection, Replication::Enums::EChannelType::CHTYPE_Actor, true, -1));
					
					if (Channel) {
						Replication::Functions::SetChannelActor(Channel, InActor);
						Channel->Connection = Connection;

						Replication::Functions::ReplicateActor(Channel);

						SULFUR_LOG("ReplicatingActor: " << InActor->GetName() << " To: " << Connection->GetName());

						ClientsThatReplicatedIt++;
					}
				}
			}
		}

		if (IsNetDriverValid()) {
			auto Connections = NetDriver->ClientConnections.Num();

			if (Connections == ClientsThatReplicatedIt)
			{
				return true;
			}
		}

		return false;
	}

	UActorChannel* ReplicateToClient(AActor* InActor, UNetConnection* InConnection)
	{
		auto Channel = (UActorChannel*)(Replication::Functions::CreateChannel(InConnection, Replication::Enums::EChannelType::CHTYPE_Actor, true, -1));

		if (Channel) {
			Replication::Functions::SetChannelActor(Channel, InActor);
			Channel->Connection = InConnection;

			if (Replication::Functions::ReplicateActor(Channel)) {
				SULFUR_LOG("ReplicatingActor: " << InActor->GetName() << " To: " << InConnection->GetName());
				return Channel;
			}
		}

		return NULL;
	}

	void InitalizeConnection(UNetConnection* InConnection)
	{
		TArray<AActor*> Actors;
		Globals::GPS->STATIC_GetAllActorsOfClass(Globals::World, AActor::StaticClass(), &Actors);

		for (int i = 0; i < Actors.Num(); i++)
		{
			auto Actor = Actors[i];
			
			if (Actor)
			{
				if (Actor->bReplicates && Actor->NetDormancy != ENetDormancy::DORM_Initial && !Actor->bNetStartup)
					this->ReplicateToClient(Actor, InConnection);
			}
		}
	}

	UActorChannel* FindChannel(AActor* Actor, UNetConnection* Connection)
	{
		for (int i = 0; i < Connection->OpenChannels.Num(); i++)
		{
			auto Channel = Connection->OpenChannels[i];
			
			if (Channel && Channel->Class)
			{
				if (Channel->Class->GetFullName() == ACTOR_CHANNEL_CLASS)
				{
					if (((UActorChannel*)Channel)->Actor == Actor)
						return ((UActorChannel*)Channel);
				}
			}
		}

		return NULL;
	}

	void Tick(float DeltaSeconds)
	{
		for (int i = 0; i < NetDriver->ClientConnections.Num(); i++)
		{
			UNetConnection* const NetConnection = NetDriver->ClientConnections[i];
			APlayerController* const PC = NetConnection->PlayerController;

			if (PC)
			{
				if (PC->PlayerState)
				std::cout << PC->PlayerState->PlayerName.ToString() << "'s Ping: " << std::to_string(PC->PlayerState->Ping) << std::endl;

				Replication::Movement::Functions::ClientSendAdjustment(PC);
			}
			else
				continue;

			for (int ChildIdx = 0; ChildIdx < NetConnection->Children.Num(); ++ChildIdx)
			{
				UNetConnection* ChildConnection = NetConnection->Children[ChildIdx];
				if (ChildConnection && ChildConnection->PlayerController && ChildConnection->ViewTarget)
				{
					Replication::Movement::Functions::ClientSendAdjustment(ChildConnection->PlayerController);
				}
			}

			TArray<AActor*> Actors;
			Globals::GPS->STATIC_GetAllActorsOfClass(Globals::World, AActor::StaticClass(), &Actors);

			for (int j = 0; j < Actors.Num(); j++)
			{
				auto Actor = Actors[j];
				
				if (Actor->bReplicates && Actor->NetDormancy != ENetDormancy::DORM_Initial && !Actor->bNetStartup && Actor->Class->GetFullName().find("Pawn") == std::string::npos)
				{
					auto Channel = this->FindChannel(Actor, NetConnection);

					if (Channel == NULL)
						Channel = this->ReplicateToClient(Actor, NetConnection);

					if (Channel)
					{
						Replication::Functions::ReplicateActor(Channel);
					}
				}
			}
		}
	}

	static void InitOffsets()
	{
		SULFUR_LOG("Setting up Replication Offsets!");

		auto BaseAddr = Util::BaseAddress();

		Replication::Functions::CreateChannel = decltype(Replication::Functions::CreateChannel)(BaseAddr + CREATE_CHANNEL_OFFSET);
		Replication::Functions::SetChannelActor = decltype(Replication::Functions::SetChannelActor)(BaseAddr + SET_CHANNEL_ACTOR_OFFSET);
		Replication::Functions::ReplicateActor = decltype(Replication::Functions::ReplicateActor)(BaseAddr + REPLICATE_ACTOR_OFFSET);
		Replication::Movement::Functions::ClientSendAdjustment = decltype(Replication::Movement::Functions::ClientSendAdjustment)(BaseAddr + CLIENT_SEND_AUDJUSTMENTS_OFFSET);
	}
};