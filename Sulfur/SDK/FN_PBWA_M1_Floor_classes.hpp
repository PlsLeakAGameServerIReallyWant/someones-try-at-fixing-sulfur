#pragma once

// Fortnite (2.4.2) SDK

#ifdef _MSC_VER
	#pragma pack(push, 0x8)
#endif

#include "../SDK.hpp"

namespace SDK
{
//---------------------------------------------------------------------------
//Classes
//---------------------------------------------------------------------------

// BlueprintGeneratedClass PBWA_M1_Floor.PBWA_M1_Floor_C
// 0x0000 (0x0E10 - 0x0E10)
class APBWA_M1_Floor_C : public ABuildingFloor
{
public:

	static UClass* StaticClass()
	{
		static auto ptr = UObject::FindClass("BlueprintGeneratedClass PBWA_M1_Floor.PBWA_M1_Floor_C");
		return ptr;
	}

};


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
