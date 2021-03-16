#pragma once

namespace app
{
    class ObjEgg;

    class EggManager : public fnd::GameService
    {
    public:
        struct LocusData
        {
            csl::math::Vector3 Position;
            csl::math::Quaternion Rotation;
            char field_20;
            INSERT_PADDING(13);
        };

        std::vector<ObjEgg*> Eggs;
        boost::circular_buffer<LocusData> GhostDataP1;
        boost::circular_buffer<LocusData> GhostDataP2;
        float field_64{};
        char IsSpaceShrink{};
        char FlowerCount{};
        INSERT_PADDING(2);

        EggManager()
        {
            // TODO: CHECK IF CRASH
            //Locus.pAllocator = (*(csl::fnd::IAllocator**)ASLR(0x00FD3FC4) + 3);
        }

    private:
        static void* staticClass() { return (void*)ASLR(0x00FEE764); }

        void UpdateLocusPos()
        {
            int* p1_info = ObjUtil::GetPlayerInformation(Document, 0);
            if (!p1_info)
                return;

            if (GhostDataP1.size() <= 1)
            {
                LocusData data { *(csl::math::Vector3*)(p1_info + 4),
                    *(csl::math::Quaternion*)(p1_info + 8), *(p1_info + 0x51) ^ 1 };

                GhostDataP1.push_front(data);
            }

            LocusData data{ *(csl::math::Vector3*)(p1_info + 4),
                *(csl::math::Quaternion*)(p1_info + 8), 0 };

            GhostDataP1.push_front(data);

            int* p2_info = ObjUtil::GetPlayerInformation(Document, 1);
            if (!p2_info)
                return;

            if (GhostDataP2.size() <= 1)
            {
                LocusData data{ *(csl::math::Vector3*)(p2_info + 4),
                    *(csl::math::Quaternion*)(p2_info + 8), *(p2_info + 0x51) ^ 1 };

                GhostDataP2.push_front(data);
            }


            data = LocusData { *(csl::math::Vector3*)(p2_info + 4),
                *(csl::math::Quaternion*)(p2_info + 8), 0 };

            GhostDataP2.push_front(data);
        }

        char AddSpaceCount(int playerNo);
        char SubSpaceCount(int playerNo);

        void UpdateEggSpace()
        {
            IsSpaceShrink &= ~4;
            int* p1_info = ObjUtil::GetPlayerInformation(Document, 0);
            int* p2_info = ObjUtil::GetPlayerInformation(Document, 1);

            /*if (p1_info && *(char*)(p1_info + 0x52))
                SubSpaceCount(0);

            if (p2_info && *(char*)(p2_info + 0x52))
                SubSpaceCount(1);

            if (IsSpaceShrink & 1)
            {
                SubSpaceCount(0);
                SubSpaceCount(1);
            }*/

            if (field_64 > 0)
            {
                AddSpaceCount(0);
                AddSpaceCount(1);
            }
        }

    public:
        bool AddEgg(ObjEgg* egg)
        {
            if (Eggs.size() >= 0xC)
                return false;
            
            Eggs.push_back(egg);
            return true;
        }

        bool CheckAccessLocusData(LocusData* locus, int i, int playerNo)
        {
            if (!playerNo)
            {
                if (!GhostDataP1.size())
                    return false;

                if (i < GhostDataP1.size())
                    *locus = GhostDataP1.at(i);
                else
                    *locus = GhostDataP1.back();
            }
            else
            {
                if (!GhostDataP2.size())
                    return false;

                if (i < GhostDataP2.size())
                    *locus = GhostDataP2.at(i);
                else
                    *locus = GhostDataP2.back();
            }

            return true;
        }

        int GetTargetLocusIndex(int index, char playerNo);

        void GetTargetDataFromLocusIndex(LocusData* locus, int i, bool a3, float* magnitude, char playerNo)
        {
            LocusData data1{};
            LocusData data2{};

            int* playerInfo = ObjUtil::GetPlayerInformation(Document, playerNo);
            if (!playerInfo)
                return;

            if (!CheckAccessLocusData(&data1, i, playerNo) || a3 || !CheckAccessLocusData(&data2, i, playerNo))
                return;

            *locus = data1;

            csl::math::Vector3 positionDifference{};
            float localMagnitude{};
            math::Vector3Subtract(&data1.Position, &data2.Position, &positionDifference);
            math::Vector3Magnitude(&positionDifference, &localMagnitude);

            if (localMagnitude > 0.3f && (field_64 > 0 || (IsSpaceShrink >> 2) & 1))
            {
                a3 = true;
                if (magnitude)
                    *magnitude = localMagnitude;
            }
        }
        
        void GetTargetData(LocusData* locus, int index, bool a3, float* magnitude, char playerNo)
        {
            int dataIndex = GetTargetLocusIndex(index, playerNo);
            GetTargetDataFromLocusIndex(locus, dataIndex, a3, magnitude, playerNo);
        }

        void SetForceSpaceShrink(bool isShrink)
        {
            if (isShrink)
                IsSpaceShrink |= 2;
            else
                IsSpaceShrink &= ~2;
        }

        void TakeYoshiSpecialFlower(char flowerID) { FlowerCount |= 1 << flowerID; }
        unsigned int IsYoshiSpecialFlowerTaked(char flowerID) { return (FlowerCount >> flowerID) & 1; }

        static EggManager* GetService(GameDocument* gameDocument)
        {
            void* managerClass = staticClass();
            EggManager* service = (EggManager*)GameDocument::GetServiceByClass(gameDocument, managerClass);
            return service;
        }

        void StartGame(bool a1) override
        {
            GhostDataP1.clear();
            GhostDataP1.set_capacity(egg::GetMaxLocusPositionNum());

            GhostDataP2.clear();
            GhostDataP2.set_capacity(egg::GetMaxLocusPositionNum());

            Eggs.clear();
        
            field_64 = 0;
            IsSpaceShrink = false;
            FlowerCount = 0;
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            UpdateLocusPos();
            UpdateEggSpace();
        }
    };
    
    inline static EggManager* construct_EggManager(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) EggManager();
    }
}