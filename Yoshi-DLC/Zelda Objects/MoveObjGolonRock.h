#pragma once

namespace app
{
	namespace game
	{
        class alignas(16) MoveObjGolonRock : public MoveController
        {
        private:
            float field_10{};
            float field_14{};
            float field_18{};
            float field_1C{};
            float field_20{};
            float field_24{};
            bool field_28{};
            INSERT_PADDING(3);
            float field_2C{};
            float field_30{};
            bool field_34{};
            INSERT_PADDING(3);
            bool field_38{};
            INSERT_PADDING(3);
            PathEvaluator PathEvaluator{};
            float field_4C{};
            float field_50{};
            float field_54{};
            float field_58{};
            int field_5C{};
            csl::math::Vector3 field_60{};
            int* PhysicsRaycastJob{};
            float field_78{};
            float field_7C{};
            short field_80{};
            short field_82{};
            int field_84{};
            short field_88{};
            short field_8A{};
            int field_8C{};
            int field_90{};
            int field_94{};
            int field_98{};
            int field_9C{};
        };
	}
}