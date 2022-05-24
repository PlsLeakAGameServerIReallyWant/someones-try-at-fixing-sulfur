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

// BlueprintGeneratedClass StoreCardReveal_Parent.StoreCardReveal_Parent_C
// 0x0010 (0x0378 - 0x0368)
class AStoreCardReveal_Parent_C : public AActor
{
public:
	struct FPointerToUberGraphFrame                    UberGraphFrame;                                           // 0x0368(0x0008) (Transient, DuplicateTransient)
	class USceneComponent*                             PinataSceneRoot;                                          // 0x0370(0x0008) (BlueprintVisible, ZeroConstructor, IsPlainOldData)

	static UClass* StaticClass()
	{
		static auto ptr = UObject::FindClass("BlueprintGeneratedClass StoreCardReveal_Parent.StoreCardReveal_Parent_C");
		return ptr;
	}


	void UserConstructionScript();
	void InitiatePinata();
	void ExecuteUbergraph_StoreCardReveal_Parent(int EntryPoint);
};


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
