#pragma once

namespace app
{
    namespace game
    {
        struct alignas(16) PathRaycastInput
        {
            csl::math::Vector3 StartPostition;
            csl::math::Vector3 EndPosition;
            char field_20;
            INSERT_PADDING(3);
            int field_24;
        };

        struct alignas(16) PathRaycastOutput
        {
            int* PathObject;
            float Distance;
            float field_08;
            float field_0C;
            float field_10;
            float field_14;
            INSERT_PADDING(8);
        };

        class PathManager
        {
        private:
            static void* staticClass() { return (void*)ASLR(0x00FD9C54); };
            inline static FUNCTION_PTR(int*, __thiscall, f_GetPathObject, ASLR(0x004D9A30), void* This, int pathID);


        public:
            inline static FUNCTION_PTR(int, __thiscall, CastRay, ASLR(0x004D9730), void* This, PathRaycastInput* inputRay, PathRaycastOutput* outputRay);

            static PathManager* GetService(GameDocument* gameDocument)
            {
                sizeof(PathRaycastInput);
                void* managerClass = staticClass();
                PathManager* service = (PathManager*)GameDocument::GetServiceByClass(gameDocument, managerClass);
                return service;
            }

            int* GetPathObject(int pathID) { return f_GetPathObject(this, pathID); }
        };
    }
}