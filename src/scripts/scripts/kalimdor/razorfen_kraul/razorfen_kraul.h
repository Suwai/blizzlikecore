/* This file is part of the BlizzLikeCore Project. See CREDITS and LICENSE files
 * This program is free software licensed under GPL version 3
 * Please see the included README file for more information */

#ifndef DEF_RFK_H
#define DEF_RFK_H

enum
{
    MAX_ENCOUNTER     = 1,

    TYPE_AGATHELOS    = 1,

    GO_AGATHELOS_WARD = 21099,

    NPC_WARD_KEEPER   = 4625
};

class BLIZZLIKE_DLL_DECL instance_razorfen_kraul : public ScriptedInstance
{
    public:
        instance_razorfen_kraul(Map* pMap);
        ~instance_razorfen_kraul() {}

        void Initialize() override;

        void OnObjectCreate(GameObject* pGo) override;
        void OnCreatureCreate(Creature* pCreature) override;

        void SetData(uint32 uiType, uint32 uiData) override;
        uint32 GetData(uint32 uiType) const override;

        const char* Save() const override { return m_strInstData.c_str(); }
        void Load(const char* chrIn) override;

    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        uint8 m_uiWardKeepersRemaining;
};
#endif
