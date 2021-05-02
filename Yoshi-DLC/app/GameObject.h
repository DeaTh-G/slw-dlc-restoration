#pragma once

namespace app
{
    class GameObject : public hh::base::CRefCountObject, public fnd::CLeafActor
    {
    private:
        inline static FUNCTION_PTR(bool, __thiscall, f_ActorProc, ASLR(0x0049D0A0), CActor* This, int id, void* data);
        inline static FUNCTION_PTR(bool, __thiscall, f_ProcessMessage, ASLR(0x0049CD10), CActor* This, fnd::Message& message);
        inline static FUNCTION_PTR(void, __thiscall, __ct, ASLR(0x0049CD60), GameObject* This);
        inline static FUNCTION_PTR(void, __thiscall, __dt, ASLR(0x0049D070), GameObject* This, size_t flags);
        inline static FUNCTION_PTR(GameObject*, __cdecl, f_new, ASLR(0x0049CAD0), size_t size);

    public:
        inline static FUNCTION_PTR(void, __thiscall, SetObjectCategory, ASLR(0x0049CAB0), GameObject* This, char category);
        inline static FUNCTION_PTR(int*, __thiscall, GetGOC, ASLR(0x0049D430), GameObject* This, char* component);
        inline static FUNCTION_PTR(bool, __thiscall, Kill, ASLR(0x0049CCA0), GameObject* This);
        inline static FUNCTION_PTR(bool, __thiscall, Sleep, ASLR(0x0049CCB0), GameObject* This);
        inline static FUNCTION_PTR(int, __thiscall, Resume, ASLR(0x0049CCC0), GameObject* This);
        inline static FUNCTION_PTR(bool, __thiscall, IsKilled, ASLR(0x0079A150), GameObject* This);
        inline static FUNCTION_PTR(csl::fnd::IAllocator*, __cdecl, GetAllocator, ASLR(0x0049CC90));

        int field_24[0x2B]{};

        GameObject() { __ct(this); }

        GameObject(size_t dummy) {}

        void Destructor(size_t deletingFlags) override
        {
            __dt(this, deletingFlags);
        }

        virtual void AddCallback(GameDocument* gameDocument) {};
        virtual void RemoveCallback(GameDocument* gameDocument) {};
        virtual void UpdatePhase(const int& updateInfo, int phase) {};
        bool ActorProc(int id, void* data) override { return f_ActorProc(this, id, data); }
        bool ProcessMessage(fnd::Message& message) override { return f_ProcessMessage(this, message); };

        void* operator new(size_t size) { return f_new(size); }

        void operator delete(void* obj)
        {
            (*(*(csl::fnd::IAllocator***)ASLR(0x00FD3FC4) + 3))->Free(obj);
        }
    };

    class GameObjectHandleBase
    {
    protected:
        size_t ObjectHandle{};
        void* pEntry{};

        void Set(const GameObject* obj)
        {
            if (obj)
            {
                pEntry = (void*)obj->field_24[3];
                ObjectHandle = obj->field_24[2];
            }
            else
            {
                pEntry = nullptr;
                ObjectHandle = 0;
            }
        }

    public:
        bool IsValid() const
        {
            if (!pEntry)
                return false;

            return (*(int*)pEntry == ObjectHandle) && *((int*)pEntry + 1);
        }

        GameObject* Get() const
        {
            if (!IsValid())
                return nullptr;

            return (GameObject*)(*((int**)pEntry + 1));
        }

        GameObjectHandleBase& operator=(const GameObject* obj)
        {
            Set(obj);
            return *this;
        }

    public:
        inline static FUNCTION_PTR(GameObject*, __thiscall, __ct, ASLR(0x0049D490), void* This, GameObject* a1);
    };
}