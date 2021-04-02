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
            char IsInAir;
            INSERT_PADDING(15);
        };

        std::vector<ObjEgg*> EggsP1;
        std::vector<ObjEgg*> EggsP2;
        boost::circular_buffer<LocusData> GhostDataP1;
        boost::circular_buffer<LocusData> GhostDataP2;
        float field_64P1{};
        float field_64P2{};
        char IsSpaceShrink{};
        char FlowerCount{};
        INSERT_PADDING(2);

        EggManager() {}

        void StartExtrication(int playerNo);

    private:
        static void* staticClass() { return (void*)ASLR(0x00FEE764); }

        void DoCheckReleaseAllEgg(const fnd::SUpdateInfo updateInfo, int playerNo);
        void DoCheckClearAllEggEndExtrication(int playerNo);

        bool IsLocusDataInGroundToAir(int playerNo)
        {
            sizeof(EggManager);
            LocusData data{};
            bool IsInAir = 0;
            bool flag = 0;

            int* playerInfo = ObjUtil::GetPlayerInformation(Document, playerNo);
            if (playerInfo)
            {
                if (!playerNo)
                {
                    for (size_t i = 0; i < EggsP1.size() * 10 + 1; i++)
                    {
                        if (CheckAccessLocusData(&data, i, playerNo))
                            if (data.IsInAir && flag)
                                IsInAir = true;
                            else
                                flag = 1;
                    }
                }
                else
                {
                    for (size_t i = 0; i < EggsP2.size() * 10 + 1; i++)
                    {
                        if (CheckAccessLocusData(&data, i, playerNo))
                            if (data.IsInAir && flag)
                                IsInAir = true;
                            else
                                flag = 1;
                    }
                }
            }

            return IsInAir;
        }

        void UpdateLocusPos(const fnd::SUpdateInfo& updateInfo, int playerNo)
        {
            int* playerInfo = ObjUtil::GetPlayerInformation(Document, playerNo);
            if (!playerInfo)
                return;

            csl::math::Vector3 playerPos { *(csl::math::Vector3*)(playerInfo + 4) };
            csl::math::Vector3 ghostPos{};
            csl::math::Vector3 ghostPos2{};
            csl::math::Vector3 posDiff{};
            csl::math::Vector3 posDiff2{};
            float diffMagnitude{};
            float diff2Magnitude{};

            if (!playerNo)
            {
                if (GhostDataP1.size() > 1)
                {
                    ghostPos = GhostDataP1.at(0).Position;
                    math::Vector3Subtract(&playerPos, &ghostPos, &posDiff);
                    math::Vector3SquareMagnitude(&posDiff, &diffMagnitude);
                    ghostPos2 = GhostDataP1.at(1).Position;
                    math::Vector3Subtract(&ghostPos, &ghostPos2, &posDiff2);
                    math::Vector3SquareMagnitude(&posDiff2, &diff2Magnitude);

                    if (0.09f <= diffMagnitude || 0.000001f < (diffMagnitude - diff2Magnitude))
                        field_64P1 = 0.3f;
                    else
                    {
                        field_64P1 -= updateInfo.deltaTime;
                        if (field_64P1 < 0 && !IsLocusDataInGroundToAir(playerNo))
                            return;
                    }
                }

                LocusData data{ playerPos, *(csl::math::Quaternion*)(playerInfo + 8),
                    *((char*)playerInfo + 0x144) ^ 1 };
                GhostDataP1.push_front(data);
            }
            else
            {
                if (GhostDataP2.size() > 1)
                {
                    ghostPos = GhostDataP2.at(0).Position;
                    math::Vector3Subtract(&playerPos, &ghostPos, &posDiff);
                    math::Vector3SquareMagnitude(&posDiff, &diffMagnitude);
                    ghostPos2 = GhostDataP2.at(1).Position;
                    math::Vector3Subtract(&ghostPos, &ghostPos2, &posDiff2);
                    math::Vector3SquareMagnitude(&posDiff2, &diff2Magnitude);

                    if (0.09f <= diffMagnitude || 0.000001f < (diffMagnitude - diff2Magnitude))
                        field_64P2 = 0.3f;
                    else
                    {
                        field_64P2 -= updateInfo.deltaTime;
                        if (field_64P2 < 0 && !IsLocusDataInGroundToAir(playerNo))
                            return;
                    }
                }

                LocusData data{ playerPos, *(csl::math::Quaternion*)(playerInfo + 8),
                    *((char*)playerInfo + 0x144) ^ 1 };
                GhostDataP2.push_front(data);
            }
        }

        char AddSpaceCount(int playerNo);
        char SubSpaceCount(int playerNo);

        void UpdateEggSpace(int playerNo)
        {
            IsSpaceShrink &= ~4;

            int* playerInfo = ObjUtil::GetPlayerInformation(Document, playerNo);
            if (!playerInfo)
                return;

            if (!playerNo)
            {
                if (*((char*)playerInfo + 0x145) && *((char*)playerInfo + 0x144))
                {
                    SubSpaceCount(playerNo);
                    return;
                }

                if (IsSpaceShrink & 1)
                {
                    SubSpaceCount(playerNo);
                    return;
                }

                if (field_64P1 > 0)
                {
                    AddSpaceCount(playerNo);
                    return;
                }
            }
            else
            {
                if (*((char*)playerInfo + 0x145) && *((char*)playerInfo + 0x144))
                {
                    SubSpaceCount(playerNo);
                    return;
                }

                if (IsSpaceShrink & 1)
                {
                    SubSpaceCount(playerNo);
                    return;
                }

                if (field_64P2 > 0)
                {
                    AddSpaceCount(playerNo);
                    return;
                }
            }
        }

    public:
        bool AddEgg(ObjEgg* egg);

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

        void GetTargetDataFromLocusIndex(LocusData* locus, int i, bool* a3, float* magnitude, char playerNo)
        {
            LocusData data1{};
            LocusData data2{};
            csl::math::Vector3 positionDifference{};
            float localMagnitude{};

            int* playerInfo = ObjUtil::GetPlayerInformation(Document, playerNo);
            if (!playerInfo)
                return;

            if (CheckAccessLocusData(&data1, i, playerNo) && !*a3 &&
                i - 1 >= 0 && CheckAccessLocusData(&data2, i - 1, playerNo))
            {
                math::Vector3Subtract(&data1.Position, &data2.Position, &positionDifference);
                math::Vector3SquareMagnitude(&positionDifference, &localMagnitude);
                if (!playerNo)
                {
                    if (localMagnitude > 0.3f && (field_64P1 > 0 || IsSpaceShrink & 4))
                    {
                        *a3 = true;
                        if (magnitude)
                            *magnitude = localMagnitude;
                    }
                }
            }

            *locus = data1;
        }
        
        void GetTargetData(LocusData* locus, int index, bool* a3, float* magnitude, char playerNo)
        {
            int dataIndex = GetTargetLocusIndex(index, playerNo);
            GetTargetDataFromLocusIndex(locus, dataIndex, a3, magnitude, playerNo);
        }

        void SetForceSpaceShrink(bool isShrink)
        {
            if (isShrink)
                IsSpaceShrink |= 1;
            else
                IsSpaceShrink &= ~1;
        }

        bool IsEndExtrication(int playerNo)
        {
            if (!playerNo)
                return EggsP1.empty();
            else
                return EggsP2.empty();
        }

        void TakeYoshiSpecialFlower(char flowerID) { FlowerCount |= 1 << flowerID; }
        bool IsYoshiSpecialFlowerTaked(char flowerID) { return (FlowerCount >> flowerID) & 1; }

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

            EggsP1.clear();
            EggsP2.clear();

            field_64P1 = 0;
            field_64P2 = 0;
            IsSpaceShrink = false;
            FlowerCount = 0;
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            UpdateLocusPos(updateInfo, 0);
            UpdateLocusPos(updateInfo, 1);
            UpdateEggSpace(0);
            UpdateEggSpace(1);
            DoCheckReleaseAllEgg(updateInfo, 0);
            DoCheckReleaseAllEgg(updateInfo, 1);
            DoCheckClearAllEggEndExtrication(0);
            DoCheckClearAllEggEndExtrication(1);
        }
    };
    
    inline static EggManager* construct_EggManager(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) EggManager();
    }
}