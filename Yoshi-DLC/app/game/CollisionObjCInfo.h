#pragma once

/* Heavily based on Sajidur78's LWAPI:
   https://github.com/Sajidur78/LWAPI/blob/master/Sonic2013/System/goc/goc_Collider.h */
namespace app
{
    namespace game
    {
        class CollisionShapeType
        {
        public:
            enum class ShapeType : char
            {
                TYPE_SPHERE,
                TYPE_BOX,
                TYPE_CAPSULE,
                TYPE_CYLINDER,
                TYPE_MESH,
                TYPE_RIGIDBODY,
                TYPE_INVALID
            };
        };

        class CollisionObjCInfo
        {
        public:
            char field_00;
            char field_01;
            short field_02;
            int field_04;
            int field_08;
            int field_0C;
            fnd::HFrame* Parent;
            int field_14;
            int field_18;
            int field_1C;
            csl::math::Vector3 Position;
            csl::math::Quaternion Rotation;

            inline static FUNCTION_PTR(int*, __thiscall, __ct, ASLR(0x004B6190), CollisionObjCInfo* This);
            inline static FUNCTION_PTR(void, __thiscall, SetLocalPosition, ASLR(0x004B61D0), CollisionObjCInfo* This, csl::math::Vector3* position);
            inline static FUNCTION_PTR(void, __thiscall, SetLocalRotation, ASLR(0x004B61F0), CollisionObjCInfo* This, csl::math::Quaternion* rotation);
        };

        class CharacterRigidBodyCInfo : public CollisionObjCInfo
        {
        private:
            inline static FUNCTION_PTR(CharacterRigidBodyCInfo*, __thiscall, __ct, ASLR(0x004B8450), CharacterRigidBodyCInfo* This);

        public:
            float field_40{};
            float field_44{};
            float field_48{};
            float field_4C{};
            float field_50{};
            INSERT_PADDING(12);

            CharacterRigidBodyCInfo()
            {
                __ct(this);
            }
        };

        class ColliShapeCInfo : public CollisionObjCInfo
        {
        public:
            CollisionShapeType::ShapeType ShapeType;
            char MotionType;
            INSERT_PADDING(2);
            int field_44;
            int field_48;
            int field_4C;
        };

        class ColliSphereShapeCInfo : public ColliShapeCInfo
        {
        public:
            float Radius;
            int field_54;
        };

        class ColliBoxShapeCInfo : public ColliShapeCInfo
        {
        public:
            csl::math::Vector3 Size;
        };

        class ColliCapsuleShapeCInfo : public ColliShapeCInfo
        {
        public:
            float Radius;
            float Height;
        };

        class ColliCylinderShapeCInfo : public ColliShapeCInfo
        {
        public:
            float Radius;
            float Height;
        };

        class ColliMeshShapeCInfo : public ColliShapeCInfo
        {
        public:
            int* Mesh;
        };
    }
}