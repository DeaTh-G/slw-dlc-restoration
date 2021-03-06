#pragma once

namespace app
{
	namespace game
	{
		class ColliderShapeType
		{
		public:
			enum ShapeType
			{
				Stone = 0x00000000,
				Earth = 0x01000000,
				Wood = 0x02000000,
				Grass = 0x03000000,
				Iron = 0x04000000,
				Beachsand = 0x05000000,
				Wetsand = 0x06000000,
				ENUM_07 = 0x07000000,
				Ford = 0x08000000,
				Cloud = 0x09000000,
				ENUM_0A = 0x0A000000,
				Snow = 0x0B000000,
				Stopdirt = 0x0C000000,
				Sandhole = 0x0D000000,
				Dirt = 0x0E000000,
				Slime = 0x0F000000,
				Ice = 0x10000000,
				Water = 0x11000000,
				Sea = 0x12000000,
				ENUM_13 = 0x13000000,
				Oil = 0x14000000,
				Poision = 0x15000000,
				Lava = 0x16000000,
				ENUM_17 = 0x17000000,
				ENUM_18 = 0x18000000,
				ENUM_19 = 0x19000000,
				ENUM_1A = 0x1A000000,
				ENUM_1B = 0x1B000000,
				ENUM_1C = 0x1C000000,
				ENUM_1D = 0x1D000000,
				ENUM_1E = 0x1E000000,
				Invisible = 0x1F000000,
				Wiremesh = 0x20000000,
			};
		};
	}
}