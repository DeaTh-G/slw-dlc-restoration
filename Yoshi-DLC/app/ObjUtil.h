#pragma once

namespace app
{
    namespace ObjUtil
    {
        inline static FUNCTION_PTR(CObjInfo*, __cdecl, GetObjectInfo, ASLR(0x0073D380), GameDocument* gameDocument, const char* infoName);
        inline static FUNCTION_PTR(int*, __cdecl, GetPackFile, ASLR(0x0073D830), int* file, const char* name);
        inline static FUNCTION_PTR(char*, __cdecl, GetStagePackName, ASLR(0x0073D2B0), GameDocument* gameDocument);
        inline static FUNCTION_PTR(int*, __cdecl, GetModelResource, ASLR(0x0073D490), void* dataPtr, const char* name, int* packFile);
        inline static FUNCTION_PTR(int*, __cdecl, GetShadowModel, ASLR(0x0073D2F0), res::ResShadowModel* dataPtr, const char* name, int packFile);
        inline static FUNCTION_PTR(int*, __cdecl, GetSkeletonResource, ASLR(0x0073D670), void* dataPtr, const char* name, int packFile);
        inline static FUNCTION_PTR(int*, __cdecl, GetAnimationResource, ASLR(0x0073D630), void* dataPtr, const char* name, int* packFile);
        inline static FUNCTION_PTR(int*, __cdecl, GetAnimationScriptResource, ASLR(0x0073D7B0), void* dataPtr, const char* name, int packFile);
        inline static FUNCTION_PTR(int*, __cdecl, GetTexSrtAnimationResource, ASLR(0x0073D730), void* dataPtr, const char* name, int packFile);
        inline static FUNCTION_PTR(int*, __cdecl, GetPhysicsMeshResource, ASLR(0x0073D6B0), void* dataPtr, const char* name, int packFile);
        inline static FUNCTION_PTR(void, __cdecl, SetupCollisionFilter, ASLR(0x0073D260), int filter, game::CollisionObjCInfo* collision);
        inline static FUNCTION_PTR(int, __cdecl, GetPlayerNo, ASLR(0x0073DDD0), int a1, int a2);
        inline static FUNCTION_PTR(int*, __cdecl, GetPlayerInformation, ASLR(0x0073DE10), GameDocument* a1, int PlayerNumber);
        inline static FUNCTION_PTR(int, __cdecl, AddScorePlayerAction, ASLR(0x0073EA60), GameObject* Object, const char* name, int playerNo);
        inline static FUNCTION_PTR(int, __cdecl, AddScore, ASLR(0x0073EB10), GameObject* Object, const char* name, fnd::Message* message);
        inline static FUNCTION_PTR(int, __cdecl, AddScore2, ASLR(0x0073E940), GameObject* Object, const char* name, int a3, csl::math::Vector3* a4); // Overload to AddScore
        inline static FUNCTION_PTR(int, __cdecl, GetSetObjectTransform, ASLR(0x0073E040), GameDocument* gameDocument, int* objectID, csl::math::Vector3* position, csl::math::Quaternion* rotation);
        inline static FUNCTION_PTR(int, __cdecl, SendMessageImmToGameActor, ASLR(0x0073DB70), GameObject* gameObject, fnd::Message* message);
        inline static FUNCTION_PTR(int, __cdecl, SendMessageImmToSetObject, ASLR(0x0073DC90), GameObject* gameObject, int* objectID, fnd::Message* message, int a4);
        inline static FUNCTION_PTR(int, __cdecl, SendMessageImmToCamera, ASLR(0x0073DBC0), GameObject* gameObject, int playerNo, fnd::Message* message);
        inline static FUNCTION_PTR(int, __cdecl, SetPropertyLockonTarget, ASLR(0x0073D280), GameObject* This);
        inline static FUNCTION_PTR(int, __cdecl, SetEnableColliShape, ASLR(0x0073D8D0), int* This, int colliNo, bool isEnable);
        inline static FUNCTION_PTR(bool, __cdecl, CheckShapeUserID, ASLR(0x0073D8B0), int a1, int a2);
        inline static FUNCTION_PTR(int*, __cdecl, GetSVPath, ASLR(0x0073E6C0), GameDocument* gameDocumnet, csl::math::Vector3* a1, csl::math::Vector3* a2);
        inline static FUNCTION_PTR(int*, __cdecl, RaycastHitCollision, ASLR(0x0073D980), GameDocument* gameDocumnet, csl::math::Vector3* a1, csl::math::Vector3* a2, int a3);

        static int SendMessageImmToPlayer(GameObject* gameObject, int playerNo, fnd::Message* message)
        {
            void* levelInfo = CLevelInfo::GetService(*Document);
            int playerID = CLevelInfo::GetPlayerID(levelInfo, playerNo);
            return gameObject->SendMessageImm(playerID, message);
        }
    };
}