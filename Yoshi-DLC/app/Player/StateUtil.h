#pragma once

namespace app
{
    namespace Player
    {
        class StateUtil
        {
        public:
            static void ScatterRingForDamage();

            inline static FUNCTION_PTR(unsigned int, __cdecl, IsLandOnMovableGround, ASLR(0x008D6510), int* stateGOC);
            inline static FUNCTION_PTR(unsigned int, __cdecl, IsDead, ASLR(0x008D8C10), int* stateGOC);
            inline static FUNCTION_PTR(unsigned int, __cdecl, SendMission, ASLR(0x008D70C0), int* stateGOC, int a2);
            inline static FUNCTION_PTR(unsigned int, __cdecl, AddRingNum, ASLR(0x008D7BA0), int* stateGOC, int a2);
            inline static FUNCTION_PTR(bool, __cdecl, IsNowPhantom, ASLR(0x008D68F0), CStateGOC* stateGOC);

            static bool IsHeartLife(CStateGOC* This)
            {
                Player::PluginStateHeartLife* pluginState = (Player::PluginStateHeartLife*)app::Player::CStateGOC::GetStatePluginPtr(This, PluginStateHeartLifeString);
                if (!pluginState)
                    return false;

                return Player::PluginStateHeartLife::GetNumHearts(pluginState) > 1;
            }

            static int ReduceHeartLife(CStateGOC* This)
            {
                Player::PluginStateHeartLife* pluginState = (Player::PluginStateHeartLife*)app::Player::CStateGOC::GetStatePluginPtr(This, PluginStateHeartLifeString);
                if (!pluginState || IsNowPhantom(This) && Player::PluginStateHeartLife::GetNumHearts(pluginState) <= 1)
                    return 1;
                else
                    return Player::PluginStateHeartLife::Damaged(pluginState);
            }

            static int AllRecoveryHeartLife(CStateGOC* This)
            {
                Player::PluginStateHeartLife* pluginState = (Player::PluginStateHeartLife*)app::Player::CStateGOC::GetStatePluginPtr(This, PluginStateHeartLifeString);
                if (!pluginState)
                    return 1;

                Player::PluginStateHeartLife::AllRecovery(pluginState);
            }

            static int RecoveryHeartLife(CStateGOC* This)
            {
                Player::PluginStateHeartLife* pluginState = (Player::PluginStateHeartLife*)app::Player::CStateGOC::GetStatePluginPtr(This, PluginStateHeartLifeString);
                if (!pluginState)
                    return 1;

                Player::PluginStateHeartLife::Recovery(pluginState);
            }

            static int IncrementMaxHeartLife(CStateGOC* This)
            {
                Player::PluginStateHeartLife* pluginState = (Player::PluginStateHeartLife*)app::Player::CStateGOC::GetStatePluginPtr(This, PluginStateHeartLifeString);
                if (!pluginState)
                    return 1;

                Player::PluginStateHeartLife::IncrementMaxHearts(pluginState);
            }
        };
    }
}