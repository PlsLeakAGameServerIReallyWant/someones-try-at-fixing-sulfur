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

// BlueprintGeneratedClass S_TRV_Water_Shallow_Half_x2.S_TRV_Water_Shallow_Half_x2_C
// 0x0000 (0x042B - 0x042B)
class AS_TRV_Water_Shallow_Half_x2_C : public A_WaterMeshBlueprintMaster_C
{
public:

	static UClass* StaticClass()
	{
		static auto ptr = UObject::FindClass("BlueprintGeneratedClass S_TRV_Water_Shallow_Half_x2.S_TRV_Water_Shallow_Half_x2_C");
		return ptr;
	}

};


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif