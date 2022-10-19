#pragma once

namespace app
{
	class WorldAreaMapDioramaInfo
	{
	public:
		csl::math::Vector3 m_Position{};
		float m_CursorX{};
		float m_CursorY{};
		int32_t field_18{};
		int32_t field_1C{};
		uint8_t m_RedRing{};
		bool m_IsActUnlocked{};
		bool m_IsActCompleted{};
		bool m_IsOnlyNormalPlay{};
		uint8_t field_24{};
		uint8_t field_25{};
		uint8_t m_Rank{};
		char field_27{};
		int m_Phantoms{};
		int field_2C{};

	public:
		WorldAreaMapDioramaInfo() {}

		WorldAreaMapDioramaInfo(csl::math::Vector3 position, float cursorCoordX, float cursorCoordY,
			int a4, int a5,
			uint8_t redRing, bool isUnlocked, bool isComplete, bool isOnlyNormalPlay,
			uint8_t a6, uint8_t a7, uint8_t rank, uint8_t a9, int phantoms, int a10)
		{
			m_Position = position;
			m_CursorX = cursorCoordX;
			m_CursorY = cursorCoordY;
			field_18 = a4;
			field_1C = a5;
			m_RedRing = redRing;
			m_IsActUnlocked = isUnlocked;
			m_IsActCompleted = isComplete;
			m_IsOnlyNormalPlay = isOnlyNormalPlay;
			field_24 = a6;
			field_25 = a7;
			m_Rank = rank;
			field_27 = a9;
			m_Phantoms = phantoms;
			field_2C = a10;
		}

		csl::math::Vector3& getPosition()
		{
			return this->m_Position;
		}

		bool getIsActUnlocked()
		{
			return this->m_IsActUnlocked;
		}

		bool getIsActCompleted()
		{
			return this->m_IsActCompleted;
		}

		void setIsActUnlocked(bool isUnlocked)
		{
			this->m_IsActUnlocked = isUnlocked;
		}

		void setIsActCompleted(bool isCompleted)
		{
			this->m_IsActCompleted = isCompleted;
		}

		void setPosition(csl::math::Vector3 position)
		{
			this->m_Position = position;
		}
	};
}