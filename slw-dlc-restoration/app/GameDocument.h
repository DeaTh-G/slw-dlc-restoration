#pragma once

namespace app
{
    class GameDocument
    {
    private:
        inline static FUNCTION_PTR(unsigned int, __thiscall, f_GetGroupActorID, ASLR(0x0090B2C0), GameDocument* This, unsigned int groupID);


    public: 
        inline static FUNCTION_PTR(int*, __thiscall, GetServiceByClass, ASLR(0x0090B2E0), GameDocument* This, void* Class);
        inline static FUNCTION_PTR(void, __thiscall, AddGameObject, ASLR(0x0090B3C0), GameDocument* This, void* obj);

        unsigned int GetGroupActorID(unsigned int groupID) { return f_GetGroupActorID(this, groupID); }
    };

    static GameDocument** Document = (GameDocument**)ASLR(0x00FEFEF4);
}