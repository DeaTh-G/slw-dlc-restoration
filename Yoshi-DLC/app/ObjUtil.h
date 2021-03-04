#pragma once

namespace app
{
	class ObjUtil
	{
	public:
		inline static FUNCTION_PTR(int*, __cdecl, GetObjectInfo, ASLR(0x0073D380), GameDocument* gameDocument, const char* infoName);
		inline static FUNCTION_PTR(int*, __cdecl, GetPackFile, ASLR(0x0073D830), int* file, const char* name);
		inline static FUNCTION_PTR(char*, __cdecl, GetStagePackName, ASLR(0x0073D2B0), GameDocument* gameDocument);
		inline static FUNCTION_PTR(int*, __cdecl, GetModelResource, ASLR(0x0073D490), void* dataPtr, const char* name, int* packFile);
		inline static FUNCTION_PTR(int*, __cdecl, GetSkeletonResource, ASLR(0x0073D670), void* dataPtr, const char* name, int packFile);
		inline static FUNCTION_PTR(int*, __cdecl, GetAnimationResource, ASLR(0x0073D630), void* dataPtr, const char* name, int* packFile);
		inline static FUNCTION_PTR(int*, __cdecl, GetAnimationScriptResource, ASLR(0x0073D7B0), void* dataPtr, const char* name, int packFile);
		inline static FUNCTION_PTR(void, __cdecl, SetupCollisionFilter, ASLR(0x0073D260), int filter, app::game::CollisionObjCInfo* collision);
		inline static FUNCTION_PTR(int, __cdecl, GetPlayerNo, ASLR(0x0073DDD0), int a1, int a2);
		inline static FUNCTION_PTR(int*, __cdecl, GetPlayerInformation, ASLR(0x0073DE10), GameDocument* a1, int PlayerNumber);
		inline static FUNCTION_PTR(int, __cdecl, AddScorePlayerAction, ASLR(0x0073EA60), GameObject* Object, const char* name, int playerNo);
		inline static FUNCTION_PTR(int, __cdecl, AddScore, ASLR(0x0073EB10), GameObject* Object, const char* name, fnd::Message* message);
		inline static FUNCTION_PTR(int, __cdecl, GetSetObjectTransform, ASLR(0x0073E040), GameDocument* gameDocument, int* objectID, Vector3* position, csl::math::Quaternion* rotation);
		inline static FUNCTION_PTR(int, __cdecl, SendMessageImmToGameActor, ASLR(0x0073DB70), GameObject* gameObject, int* message);
		inline static FUNCTION_PTR(int, __cdecl, SendMessageImmToSetObject, ASLR(0x0073DC90), GameObject* gameObject, int* objectID, int* message, int a4);
		inline static FUNCTION_PTR(int, __cdecl, SendMessageImmToPlayer, ASLR(0x0073DF60), GameObject* gameObject, int* message);
		inline static FUNCTION_PTR(int, __cdecl, GetPlayerActorID, ASLR(0x0073DDF0), GameDocument* gameDocument, int playerNo);
		inline static FUNCTION_PTR(int, __cdecl, SetPropertyLockonTarget, ASLR(0x0073D280), GameObject* This);
	};
}