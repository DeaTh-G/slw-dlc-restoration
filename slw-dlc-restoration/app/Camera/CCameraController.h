#pragma once

namespace app
{
    namespace Camera
    {
        class EnterEventArgs;
        class SPlayer3DCameraParam;
        class SPlayer3DCameraParamDashMode;

        class CCameraController : public hh::base::CRefCountObject
        {
        private:
            inline static FUNCTION_PTR(void, __thiscall, __ct, ASLR(0x00419B30), CCameraController* This);
            inline static FUNCTION_PTR(void, __thiscall, __dt, ASLR(0x00419BB0), CCameraController* This, size_t flags);

        public:
            int field_08{};

        private:
            float Fovy = 45;
            csl::math::Vector3 Position{};
            csl::math::Vector3 field_20 = Vector3(0, 1, 0);
            csl::math::Vector3 TargetPosition{};
        
        public:
            int field_40{};
            int field_44{};
            float field_48{};
            float field_4C{};
            char field_50{};
            bool IsActive{};
            INSERT_PADDING(2);

        private:
            /// <summary>
            /// CollisionEnable - 0x1
            /// PhantomCamera - 0x2
            /// DebugCamera - 0x4
            /// MultiCamera - 0x8
            /// Preview - 0x10
            /// </summary>
            int Flags{};
            INSERT_PADDING(12);

        protected:
            CCameraController() { __ct(this); }

            void Destructor(size_t deletingFlags) override { __dt(this, deletingFlags); }

            virtual void ResetCamera() {}
            virtual void RestartCamera() {}
            virtual void Update(const fnd::SUpdateInfo& updateInfo) {}
            virtual void OnEnter(EnterEventArgs& args) { SetActive(true); }
            virtual void OnLeave() { SetActive(false); }
            virtual void BeginBoostCamera() {}
            virtual void EndBoostCamera() {}
            virtual void BeginQuickStep() {}
            virtual void EndQuickStep() {}
            virtual void Set3DCameraParam(const SPlayer3DCameraParam& cameraParam) {}
            virtual void Set3DCameraParamDashMode(const SPlayer3DCameraParamDashMode& cameraParam) {}
            virtual void SetCameraReset() {}
            virtual bool IsDistFar() { return false; }
            virtual void FocusChange() {}
            virtual bool ProcessMessage(fnd::Message& message) { return false; }
            void SetCameraParam(const csl::math::Vector3& position, const csl::math::Vector3& a2, const csl::math::Vector3& targetPosition)
            {
                Position = position;
                field_20 = a2;
                TargetPosition = targetPosition;
            }

        public: 
            void SetActive(bool isActive) { IsActive = isActive; }
            void SetFovy(float fovy) { Fovy = fovy; }
            void SetCollisionEnable()
            {
                if (Flags)
                    Flags |= 1;
                else
                    Flags &= ~1;
            }
            void SetPhantomCamera() { Flags |= 2; }
            void SetDebugCamera() { Flags |= 4; }
            void SetMultiCamera() { Flags |= 8; }
            void SetPreview() { Flags |= 16; }
            float GetFovy() { return Fovy; }
            bool IsCollisionEnable() { return (Flags & 1) != 0; }
            bool IsPhantomCamera() { return (Flags & 2) != 0; }
            bool IsDebugCamera() { return (Flags & 4) != 0; }
            bool IsMultiCamera() { return (Flags & 8) != 0; }
            bool IsPreview() { return (Flags & 16) != 0; }
        };
    }
}