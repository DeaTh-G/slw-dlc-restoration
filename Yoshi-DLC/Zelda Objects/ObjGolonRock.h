#pragma once

namespace app
{
    class ObjGolonRock;

	namespace golon_rock
	{
        struct GolonRockCreateInfo
        {
            csl::math::Matrix34 TransformMatrix{};
            float NegativeSpeed{};
            bool IsCheckFall{};
            INSERT_PADDING(3);

            GolonRockCreateInfo() {}
            GolonRockCreateInfo(const csl::math::Matrix34& matrix, const float speed, const bool isCheckFall)
            {
                TransformMatrix = matrix;
                NegativeSpeed = speed;
                IsCheckFall = isCheckFall;
            }
        };

        /*ObjGolonRock**/ void CreateGolonRock(GameDocument& gameDocument, const GolonRockCreateInfo& cInfo);
	}
}

inline static /*app::ObjGolonRock**/ void app::golon_rock::CreateGolonRock(GameDocument& gameDocument, const GolonRockCreateInfo& cInfo)
{
    /*ObjGolonRock* object = new ObjGolonRock(gameDocument, cInfo);
    if (object)
        GameDocument::AddGameObject(*(GameDocument**)&gameDocument, object);

    return object;*/
}