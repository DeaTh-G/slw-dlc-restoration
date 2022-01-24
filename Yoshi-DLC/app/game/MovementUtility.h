#pragma once

namespace app
{
    namespace game
    {
        class MovementUtility
        {
        public:
            struct SideviewInterpolateInfo
            {
                csl::math::Vector3 field_00{};
                csl::math::Vector3 field_10{};
                PathEvaluator field_20{};
            };
        };
    }
}