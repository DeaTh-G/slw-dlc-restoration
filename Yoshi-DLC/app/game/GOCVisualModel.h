#pragma once

namespace app
{
    namespace fnd
    {
        class GOCVisualModel
        {
        public:
            class VisualDescription
            {
            public:
                int Model;
                int Skeleton;
                int field_08;
                int Animation;
                int LightQualityType;
                HFrame* Parent;
                float ZIndex;
                int field_1C;
                int field_20;
                int field_24;
                int field_28;
                int field_2C;
                int field_30;
                int field_34;
                int field_38;
            
                inline static FUNCTION_PTR(VisualDescription*, __thiscall, __ct, ASLR(0x00495480), VisualDescription* This);
            };

            inline static FUNCTION_PTR(void, __thiscall, Setup, ASLR(0x00495850), int* This, VisualDescription* visualDescriptor);
            inline static FUNCTION_PTR(void, __thiscall, AttachAnimation, ASLR(0x004951B0), int* This, int* gocAnimation);
            inline static FUNCTION_PTR(void, __thiscall, GetNodeTransform, ASLR(0x004951F0), int* This, int a2, const char* nodeName, math::Transform* transform);
            inline static FUNCTION_PTR(math::Transform*, __thiscall, GetNodeTransformID, ASLR(0x004951D0), int* This, int a2, int a3, math::Transform* transform); // Overloads GetNodeTransform
            inline static FUNCTION_PTR(void, __thiscall, SetNodeTransform, ASLR(0x00495240), int* This, int a2, const char* nodeName, math::Transform* transform);
            inline static FUNCTION_PTR(void*, __thiscall, SetTexSrtAnimation, ASLR(0x00495890), int* This, void* animation);
            inline static FUNCTION_PTR(void*, __thiscall, GetTexSrtControl, ASLR(0x004954E0), int* This);
        };

        class GOCVisualTransformed
        {
        public:
            inline static FUNCTION_PTR(void, __thiscall, SetLocalTranslation, ASLR(0x00495EA0), int* This, csl::math::Vector3* translation);
            inline static FUNCTION_PTR(void, __thiscall, SetLocalRotation, ASLR(0x00495ED0), int* This, csl::math::Quaternion* rotation);
            inline static FUNCTION_PTR(void, __thiscall, SetLocalRotationXYZ, ASLR(0x00495F00), int* This, float X, float Y, float Z);
            inline static FUNCTION_PTR(void, __thiscall, SetLocalScale, ASLR(0x00496240), int* This, csl::math::Vector3* scale);
        };
    }
}