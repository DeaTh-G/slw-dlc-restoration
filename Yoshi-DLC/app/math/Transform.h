#pragma once

namespace app
{
    namespace math
    {
        class Transform
        {
        public:
            csl::math::Vector3 Position;
            csl::math::Quaternion Rotation;
            csl::math::Vector3 Scale;
            bool IsDirty;
            
            inline static FUNCTION_PTR(csl::math::Matrix34*, __thiscall, GetTransformMatrix, ASLR(0x00532BD0), Transform* This, csl::math::Matrix34* transformMatrix);
            void SetFlag(bool isDirty) { IsDirty = isDirty; }
        };
    }
}