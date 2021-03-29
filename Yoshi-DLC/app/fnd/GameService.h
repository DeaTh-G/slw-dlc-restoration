#pragma once

namespace app
{
    namespace fnd
    {
        class GameService : public ReferencedObject, public CLeafActor
        {
        private:
            inline static FUNCTION_PTR(bool, __thiscall, f_ActorProc, ASLR(0x004A1BF0), CActor* This, int id, void* data);
            inline static FUNCTION_PTR(GameService*, __thiscall, __ct, ASLR(0x004A1C20), GameService* This, int a2);

        protected:
            GameDocument* Document;
            GameServiceClass* ServiceClass;
            void* field_04;
            void* field_08;
            unsigned int flags;

        public:
            GameService()
            {
                __ct(this, 0);
            }

            virtual void ResolveAccessibleSerivce(GameDocument& document) {}
            virtual void Load() {}
            virtual void EndLoad() {}
            virtual void PrepareToStartGame(bool a1) {}
            virtual void StartGame(bool a1) {}
            virtual void InitByScript(int& luaScript) {}
            virtual void OnAddedToGame() {}
            virtual void OnRemovedFromGame() {}
            virtual void UpdateFinal(int& updateInfo) {}

            bool ActorProc(int id, void* data) override { return f_ActorProc(this, id, data); }
        };
    }
}