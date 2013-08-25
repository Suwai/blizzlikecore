/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS and LICENSE files for Copyright information.
 */

/* ScriptData
Name: boss_shade_of_akama
Complete(%): 95
Comment: Seems to be complete.
Category: Black Temple
EndScriptData */

#include "ScriptPCH.h"
#include "black_temple.h"

#define GOSSIP_ITEM     "We are ready to fight alongside you, Akama"

// Locations
const float Z1                 = 118.543144f;
const float Z2                 = 120.783768f;
const float Z_SPAWN            = 113.537949f;
const float AGGRO_X            = 482.793182f;
const float AGGRO_Y            = 401.270172f;
const float AGGRO_Z            = 112.783928f;
const float AKAMA_X            = 514.583984f;
const float AKAMA_Y            = 400.601013f;
const float AKAMA_Z            = 112.783997f;

enum Says
{
    SAY_DEATH                  = -1564013,
    SAY_LOW_HEALTH             = -1564014,
    // Ending cinematic text
    SAY_FREE                   = -1564015,
    SAY_BROKEN_FREE_01         = -1564016,
    SAY_BROKEN_FREE_02         = -1564017
};

struct Location
{
    float x, y, o, z;
};

static Location SpawnLocations[]=
{
    {498.652740f, 461.728119f, 0},
    {498.505003f, 339.619324f, 0}
};

static Location AkamaWP[]=
{
    {482.352448f, 401.162720f, 0, 112.783928f},
    {469.597443f, 402.264404f, 0, 118.537910f}
};

static Location BrokenCoords[]=
{
    {541.375916f, 401.439575f, M_PI, 112.783997f},              // The place where Akama channels
    {534.130005f, 352.394531f, 2.164150f, 112.783737f},         // Behind a 'pillar' which is behind the east alcove
    {499.621185f, 341.534729f, 1.652856f, 112.783730f},         // East Alcove
    {499.151093f, 461.036438f, 4.770888f, 112.78370f},          // West Alcove
};

static Location BrokenWP[]=
{
    {492.491638f, 400.744690f, 3.122336f, 112.783737f},
    {494.335724f, 382.221771f, 2.676230f, 112.783737f},
    {489.555939f, 373.507202f, 2.416263f, 112.783737f},
    {491.136353f, 427.868774f, 3.519748f, 112.783737f},
};

enum Spells
{
 // SPELL_VERTEX_SHADE_BLACK   = 39833,

    SPELL_SHADE_SOUL_CHANNEL   = 40401,
    SPELL_SHADE_SOUL_CHANNEL_2 = 40520,

    SPELL_DESTRUCTIVE_POISON   = 40874,
 // SPELL_CHAIN_LIGHTNING      = 40536,

    SPELL_AKAMA_SOUL_CHANNEL   = 40447,
    SPELL_AKAMA_SOUL_RETRIEVE  = 40902,

 // AKAMA_SOUL_EXPEL           = 40855,
    SPELL_DEBILITATIG_STRIKE   = 41178,
    SPELL_SHIELD_BASH          = 41180,
    SPELL_HEROIC_STRIKE        = 41975,

    // SpiritbinderSpells
    SPELL_CHAIN_HEAL           = 42027,
    SPELL_SSPIRIT_HEAL         = 42317,
    SPELL_SPIRIT_MEND          = 42025,

    // ElementalistSpells
    SPELL_RAIN_OF_FIRE         = 42023,
    SPELL_LIGHTNING_BOLT       = 42024,

    // RogueSpells
    SPELL_DEBILITATING_POISON  = 41978,
    SPELL_EVISCERATE           = 41177,
    SPELL_DUAL_WIELD           = 29651,

    // Channeler entry
    CREATURE_CHANNELER         = 23421,
    CREATURE_SORCERER          = 23215,
    CREATURE_DEFENDER          = 23216,
    CREATURE_BROKEN            = 23319
};

const uint32 spawnEntries[4]= { 23523, 23318, 23524 };

struct mob_ashtongue_channelerAI : public ScriptedAI
{
    mob_ashtongue_channelerAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = (ScriptedInstance*)c->GetInstanceData();
        ShadeGUID = 0;
    }

    ScriptedInstance* pInstance;
    uint64 ShadeGUID;

    void Reset()
    {
        me->RemoveAurasDueToSpell(SPELL_SHADE_SOUL_CHANNEL);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        me->setActive(true);
    }

    void JustDied(Unit* /*killer*/);
    void OnAuraRemove(Aura* aura, bool stackRemove);
    void EnterCombat(Unit* /*who*/) {}
    void AttackStart(Unit* /*who*/) {}
    void MoveInLineOfSight(Unit* /*who*/) {}

    void UpdateAI(const uint32 diff)
    {
        if (ShadeGUID)
        {
            if (!me->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT))
            {
                me->RemoveAurasDueToSpell(SPELL_SHADE_SOUL_CHANNEL);
                if (Unit* shade = me->GetUnit(*me, ShadeGUID))
                {
                    if (shade->isAlive())
                        DoCast(shade, SPELL_SHADE_SOUL_CHANNEL);
                }
            }
        }
        else
        {
            if (pInstance)
                ShadeGUID = pInstance->GetData64(DATA_SHADEOFAKAMA);
        }
    }
};

struct mob_ashtongue_defenderAI : public ScriptedAI
{
    mob_ashtongue_defenderAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = (ScriptedInstance*)c->GetInstanceData();
    }

    ScriptedInstance* pInstance;

    uint32 m_debilStrikeTimer;
    uint32 m_shieldBashTimer;
    uint32 m_checkTimer;

    void Reset()
    {
        me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, false);
        me->ApplySpellImmune(0, IMMUNITY_EFFECT,SPELL_EFFECT_ATTACK_ME, false);

        m_debilStrikeTimer = 10000;
        m_shieldBashTimer = 1000;
        m_checkTimer = 10000;

        me->setActive(true);
    }

    void EnterCombat(Unit* pWho)
    {
        DoZoneInCombat();
    }

    void DoMeleeAttackIfReady()
    {
        if (me->hasUnitState(UNIT_STAT_CASTING))
            return;

        //Make sure our attack is ready and we aren't currently casting before checking distance
        if (me->isAttackReady())
        {
            //If we are within range melee the target
            if (me->IsWithinMeleeRange(me->getVictim()))
            {
                me->AttackerStateUpdate(me->getVictim());
                me->resetAttackTimer();

                if(IS_CREATURE_GUID(me->getVictim()->GetGUID()))
                    DoCast(me->getVictim(), SPELL_HEROIC_STRIKE, true);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        if (m_debilStrikeTimer < diff)
        {
            DoCast(me->getVictim(), SPELL_DEBILITATIG_STRIKE);
            m_debilStrikeTimer = 20000;
        }
        else
            m_debilStrikeTimer -= diff;

        if (m_shieldBashTimer < diff)
        {
            if (me->getVictim() && me->getVictim()->hasUnitState(UNIT_STAT_CASTING))
            {
                DoCast(me->getVictim(), SPELL_SHIELD_BASH);
                m_shieldBashTimer = 10000;
            }
        }
        else
            m_shieldBashTimer -= diff;

        if (m_checkTimer < diff)
        {
            if(!pInstance)
                return;

            if(Creature* pAkama = me->GetCreature(*me, pInstance->GetData64(DATA_SHADEOFAKAMA)))
            {
                if(!pAkama->isAlive())
                {
                    me->Kill(me, false);
                    me->RemoveCorpse();
                }
            }
            m_checkTimer = 5000;
        }
        else
            m_checkTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct mob_ashtongue_spiritbinderAI : public ScriptedAI
{
    mob_ashtongue_spiritbinderAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = (ScriptedInstance*)c->GetInstanceData();
    }

    ScriptedInstance* pInstance;

    uint32 m_chainHealTimer;
    uint32 m_spiritHealTimer;
    uint32 m_spiritMendTimer;
    uint32 m_checkTimer;

    void Reset()
    {
        m_chainHealTimer  = urand(10000, 15000);
        m_spiritHealTimer = urand(7000, 10000);
        m_spiritMendTimer = urand(14000, 20000);
        m_checkTimer = 5000;

        me->setActive(true);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (me->getVictim())
            return;

        if (pWho->GetTypeId() == TYPEID_PLAYER)
        {
            // drop PointMovement since it has higher priority than chase :P
            me->GetMotionMaster()->MoveIdle();
            AttackStart(pWho);
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (Creature* pAkama = me->GetCreature(*me, pInstance->GetData64(DATA_AKAMA_SHADE)))
            AttackStart(pAkama);
    }

    /*Unit* FindSpiritHealTarget()
    {
        Unit* pTarget = NULL;

        std::list<Creature*> m_sorcerrers = FindAllCreaturesWithEntry(CREATURE_SORCERER, 100.0f);
        std::list<Creature*> m_channelers = FindAllCreaturesWithEntry(CREATURE_CHANNELER, 100.0f);

        if (!m_sorcerrers.empty())
        {
            pTarget = *m_sorcerrers.begin();

            for (std::list<Creature*>::iterator i = m_sorcerrers.begin(); i != m_sorcerrers.end(); i++)
            {
                if (!(*i)->isAlive())
                    continue;

                if ((*i)->GetHealth() < pTarget->GetHealth())
                    pTarget = *i;
            }
        }

        if (!m_channelers.empty())
        {
            if(!pTarget)
                pTarget = *m_sorcerrers.begin();

            for (std::list<Creature*>::iterator i = m_sorcerrers.begin(); i != m_sorcerrers.end(); i++)
            {
                if (!(*i)->isAlive())
                    continue;

                if ((*i)->GetHealth() < pTarget->GetHealth())
                    pTarget = *i;
            }
        }

        if (pTarget->isAlive() && pTarget->IsInWorld())
            return pTarget;
        else
            return NULL;
    }*/

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        if (m_chainHealTimer < diff)
        {
            DoCast(me, SPELL_CHAIN_HEAL, false);
            m_chainHealTimer = 20000;
        }
        else
            m_chainHealTimer -= diff;

        if (m_spiritMendTimer < diff)
        {
            DoCast(me, SPELL_SPIRIT_MEND, false);
            m_spiritMendTimer = 20000;
        }
        else
            m_spiritMendTimer -= diff;

        if (m_spiritHealTimer < diff)
        {
            //if(Unit* pFriend = FindSpiritHealTarget())
            //{
                DoCast(NULL, SPELL_SSPIRIT_HEAL, false);
                m_spiritHealTimer = 10000;
            //}
            //else
                //m_spiritHealTimer = 5000;
        }
        else
            m_spiritHealTimer -= diff;

        if (m_checkTimer < diff)
        {
            if(!pInstance)
                return;

            if(Creature* pAkama = me->GetCreature(*me, pInstance->GetData64(DATA_SHADEOFAKAMA)))
            {
                if(!pAkama->isAlive())
                {
                    me->Kill(me, false);
                    me->RemoveCorpse();
                }
            }
            m_checkTimer = 5000;
        }
        else
            m_checkTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct mob_ashtongue_elementalistAI : public ScriptedAI
{
    mob_ashtongue_elementalistAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = (ScriptedInstance*)c->GetInstanceData();
    }

    ScriptedInstance* pInstance;

    uint32 m_rainofFireTimer;
    uint32 m_lightningBoltTimer;
    uint32 m_checkTimer;

    void Reset()
    {
        m_rainofFireTimer  = urand(5000, 18000);
        m_lightningBoltTimer = urand(2000, 4000);
        m_checkTimer = 5000;

        me->setActive(true);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (me->getVictim())
            return;

        if (pWho->GetTypeId() == TYPEID_PLAYER)
        {
            // drop PointMovement since it has higher priority than chase :P
            me->GetMotionMaster()->MoveIdle();
            AttackStart(pWho);
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (Creature* pAkama = me->GetCreature(*me, pInstance->GetData64(DATA_AKAMA_SHADE)))
            AttackStart(pAkama);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        if (m_lightningBoltTimer < diff)
        {
            DoCast(me->getVictim(), SPELL_LIGHTNING_BOLT, false);
            m_lightningBoltTimer = 10000;
        }
        else
            m_lightningBoltTimer -= diff;

        if (m_rainofFireTimer < diff)
        {
            DoZoneInCombat();
            if(Unit* pEnemy = SelectUnit(SELECT_TARGET_RANDOM, 1))
            {
                DoCast(pEnemy, SPELL_RAIN_OF_FIRE, false);
                m_rainofFireTimer = 15000;
            }
        }
        else
            m_rainofFireTimer -= diff;

        if (m_checkTimer < diff)
        {
            if (!pInstance)
                return;

            if (Creature* pAkama = me->GetCreature(*me, pInstance->GetData64(DATA_SHADEOFAKAMA)))
            {
                if (!pAkama->isAlive())
                {
                    me->Kill(me, false);
                    me->RemoveCorpse();
                }
            }
            m_checkTimer = 5000;
        }
        else
            m_checkTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct mob_ashtongue_rogueAI : public ScriptedAI
{
    mob_ashtongue_rogueAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = (ScriptedInstance*)c->GetInstanceData();
    }

    ScriptedInstance* pInstance;

    uint32 m_debilPoisonTimer;
    uint32 m_eviscerateTimer;
    uint32 m_checkTimer;

    void Reset()
    {
        m_debilPoisonTimer  = urand(5000, 15000);
        m_eviscerateTimer = urand(2000, 7000);
        m_checkTimer = 5000;

        me->setActive(true);
        DoCast(me, SPELL_DUAL_WIELD);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (me->getVictim())
            return;

        if (pWho->GetTypeId() == TYPEID_PLAYER)
        {
            // drop PointMovement since it has higher priority than chase :P
            me->GetMotionMaster()->MoveIdle();
            AttackStart(pWho);
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (Creature* pAkama = me->GetCreature(*me, pInstance->GetData64(DATA_AKAMA_SHADE)))
            AttackStart(pAkama);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        if (m_debilPoisonTimer < diff)
        {
            DoCast(me->getVictim(), SPELL_DEBILITATING_POISON, false);
            m_debilPoisonTimer = 15000;
        }
        else
            m_debilPoisonTimer -= diff;

        if (m_eviscerateTimer < diff)
        {
            DoCast(me->getVictim(), SPELL_EVISCERATE, false);
            m_eviscerateTimer = 10000;
        }
        else
            m_eviscerateTimer -= diff;

        if (m_checkTimer < diff)
        {
            if (!pInstance)
                return;

            if (Creature* pAkama = me->GetCreature(*me, pInstance->GetData64(DATA_SHADEOFAKAMA)))
            {
                if(!pAkama->isAlive())
                {
                    me->Kill(me, false);
                    me->RemoveCorpse();
                }
            }
            m_checkTimer = 5000;
        }
        else
            m_checkTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct mob_ashtongue_sorcererAI : public ScriptedAI
{
    mob_ashtongue_sorcererAI(Creature* c) : ScriptedAI(c) {ShadeGUID = 0;}

    uint64 ShadeGUID;
    uint32 CheckTimer;
    bool StartBanishing;

    void Reset()
    {
        StartBanishing = false;
        CheckTimer = 5000;
        me->setActive(true);
    }

    void JustDied(Unit* /*killer*/);
    void EnterCombat(Unit* /*who*/) {}
    void AttackStart(Unit* /*who*/) {}
    void MoveInLineOfSight(Unit* /*who*/) {}
    void UpdateAI(const uint32 diff)
    {
        if (StartBanishing)
            return;

        if (CheckTimer <= diff)
        {
            Creature* Shade = Unit::GetCreature((*me), ShadeGUID);
            if (Shade && Shade->isAlive() && me->isAlive())
            {
                if (me->IsWithinDist(Shade, 20, false))
                {
                    me->GetMotionMaster()->MoveIdle();
                    DoCast(Shade, SPELL_SHADE_SOUL_CHANNEL, true);
                    DoCast(Shade, SPELL_SHADE_SOUL_CHANNEL_2, true);

                    StartBanishing = true;
                }
            }
            CheckTimer = 2000;
        } else CheckTimer -= diff;
    }
};

struct boss_shade_of_akamaAI : public ScriptedAI
{
    boss_shade_of_akamaAI(Creature* c) : ScriptedAI(c), summons(me)
    {
        pInstance = c->GetInstanceData();
        AkamaGUID = pInstance ? pInstance->GetData64(DATA_AKAMA_SHADE) : 0;
        me->setActive(true); //if view distance is too low
        me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
        me->ApplySpellImmune(0, IMMUNITY_EFFECT,SPELL_EFFECT_ATTACK_ME, true);
    }

    ScriptedInstance* pInstance;

    std::list<uint64> Channelers;
    std::list<uint64> Sorcerers;
    uint64 AkamaGUID;

    uint32 SorcererCount;
    uint32 DeathCount;

    uint32 ReduceHealthTimer;
    uint32 SummonTimer;
    uint32 ResetTimer;
    uint32 DefenderTimer;                                   // They are on a flat 15 second timer, independant of the other summon creature timer.

    bool IsBanished;
    bool HasKilledAkama;
    bool reseting;
    bool GridSearcherSucceeded;
    bool HasKilledAkamaAndReseting;
    bool StartCombat;
    SummonList summons;

    void Reset()
    {
        reseting = true;
        StartCombat = false;
        HasKilledAkamaAndReseting = false;

        GridSearcherSucceeded = false;

        Sorcerers.clear();
        summons.DespawnAll();//despawn all adds

        if (Creature* Akama = Unit::GetCreature(*me, AkamaGUID))
        {
            if (Akama->isDead())
            {
                Akama->Respawn();//respawn akama if dead
                Akama->AI()->EnterEvadeMode();
                Akama->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);//turn gossip on so players can restart the event
            }
        }
        SorcererCount = 0;
        DeathCount = 0;

        SummonTimer = 10000;
        ReduceHealthTimer = 0;
        ResetTimer = 60000;
        DefenderTimer = 15000;

        IsBanished = true;
        HasKilledAkama = false;

        me->SetVisibility(VISIBILITY_ON);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STUN);

        if (pInstance && me->isAlive())
            pInstance->SetData(DATA_SHADEOFAKAMAEVENT, NOT_STARTED);

        reseting = false;

        me->setActive(true);
    }
    void JustDied(Unit* /*killer*/)
    {
        summons.DespawnAll();
    }
    void JustSummoned(Creature* summon)
    {
        if (summon->GetEntry() == CREATURE_DEFENDER || summon->GetEntry() == 23523 || summon->GetEntry() == 23318 || summon->GetEntry() == 23524)
            summons.Summon(summon);
    }
    void SummonedCreatureDespawn(Creature* summon)
    {
        if (summon->GetEntry() == CREATURE_DEFENDER || summon->GetEntry() == 23523 || summon->GetEntry() == 23318 || summon->GetEntry() == 23524)
            summons.Despawn(summon);
    }

    void MoveInLineOfSight(Unit* /*who*/)
    {
        if (!GridSearcherSucceeded)
        {
            FindChannelers();

            if (!Channelers.empty())
            {
                for (std::list<uint64>::iterator itr = Channelers.begin(); itr != Channelers.end(); ++itr)
                {
                    Creature* Channeler = (Unit::GetCreature(*me, *itr));
                    if (Channeler)
                    {
                        if (Channeler->isDead())
                        {
                            Channeler->RemoveCorpse();
                            Channeler->Respawn();
                            Channeler->InterruptNonMeleeSpells(true);
                            Channeler->RemoveAurasDueToSpell(SPELL_SHADE_SOUL_CHANNEL);
                        }

                        if (Channeler->isAlive())
                        {
                            Channeler->CastSpell(me, SPELL_SHADE_SOUL_CHANNEL, true);
                            Channeler->CastSpell(me, SPELL_SHADE_SOUL_CHANNEL_2, true);
                            GridSearcherSucceeded = true;
                        }
                    }
                }
            } else error_log("BSCR ERROR: No Channelers are stored in the list. This encounter will not work properly");
        }
    }

    void EnterCombat(Unit* who) { }

    void AttackStart(Unit* who)
    {
        if (!who || IsBanished)
            return;

        if (who->isTargetableForAttack() && who != me)
            DoStartMovement(who);
    }

    void IncrementDeathCount(uint64 guid = 0)               // If guid is set, will remove it from list of sorcerer
    {
        if (reseting)
            return;

        debug_log("BSCR: Increasing Death Count for Shade of Akama encounter");
        ++DeathCount;
        me->RemoveSingleAuraFromStack(SPELL_SHADE_SOUL_CHANNEL_2, 0);
        if (guid)
        {
            if (Sorcerers.empty())
                error_log("BSCR ERROR: Shade of Akama - attempt to remove guid %u from Sorcerers list but list is already empty", guid);
            else  Sorcerers.remove(guid);
        }
    }

    void SummonCreature()
    {
        uint32 random = rand()%2;
        float X = SpawnLocations[random].x;
        float Y = SpawnLocations[random].y;
        // max of 6 sorcerers can be summoned
        if ((rand()%3 == 0) && (DeathCount > 0) && (SorcererCount < 7))
        {
            Creature* Sorcerer = me->SummonCreature(CREATURE_SORCERER, X, Y, Z_SPAWN, 0, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (Sorcerer)
            {
                CAST_AI(mob_ashtongue_sorcererAI, Sorcerer->AI())->ShadeGUID = me->GetGUID();
                Sorcerer->RemoveUnitMovementFlag(MOVEFLAG_WALK_MODE);
                Sorcerer->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                Sorcerer->SetUInt64Value(UNIT_FIELD_TARGET, me->GetGUID());
                Sorcerers.push_back(Sorcerer->GetGUID());
                --DeathCount;
                ++SorcererCount;
            }
        }
        else
        {
        for (uint8 pos = 0; pos < 2; ++pos)
        {
            X = SpawnLocations[pos].x;
            Y = SpawnLocations[pos].y;

            for (uint8 i = 0; i < 3; ++i)
            {
                Creature* Spawn = me->SummonCreature(spawnEntries[i], X, Y, Z_SPAWN, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                if (Spawn)
                {
                    Spawn->RemoveUnitMovementFlag(MOVEFLAG_WALK_MODE);
                    Spawn->GetMotionMaster()->MovePoint(0, AGGRO_X, AGGRO_Y, AGGRO_Z);
                    Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 1);
                    Spawn->AI()->AttackStart(pTarget);
                    DoZoneInCombat(Spawn);
                }
            }
        }
        }
    }

    void FindChannelers()
    {
        std::list<Creature*> ChannelerList;
        me->GetCreatureListWithEntryInGrid(ChannelerList, CREATURE_CHANNELER, 50.0f);

        if (!ChannelerList.empty())
        {
            for (std::list<Creature*>::iterator itr = ChannelerList.begin(); itr != ChannelerList.end(); ++itr)
            {
                CAST_AI(mob_ashtongue_channelerAI, (*itr)->AI())->ShadeGUID = me->GetGUID();
                Channelers.push_back((*itr)->GetGUID());
                debug_log("BSCR: Shade of Akama Grid Search found channeler %u. Adding to list", (*itr)->GetGUID());
            }
        }
        else error_log("BSCR ERROR: Grid Search was unable to find any channelers. Shade of Akama encounter will be buggy");
    }

    void SetSelectableChannelers()
    {
        FindChannelers();

        if (Channelers.empty() || pInstance->GetData(DATA_SHADEOFAKAMAEVENT) == NOT_STARTED)
            return;

        for (std::list<uint64>::iterator itr = Channelers.begin(); itr != Channelers.end(); ++itr)
            if (Creature* Channeler = (Unit::GetCreature(*me, *itr)))
                Channeler->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void SetAkamaGUID(uint64 guid) { AkamaGUID = guid; }

    void UpdateAI(const uint32 diff)
    {
        if (!StartCombat)
            return;

        if (IsBanished)
        {
            // Akama is set in the threatlist so when we reset, we make sure that he is not included in our check
            if (me->getThreatManager().getThreatList().size() < 2)
            {
                EnterEvadeMode();
                return;
            }

            if (DefenderTimer <= diff)
            {
                uint32 ran = rand()%2;
                Creature* Defender = me->SummonCreature(CREATURE_DEFENDER, SpawnLocations[ran].x, SpawnLocations[ran].y, Z_SPAWN, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 25000);
                if (Defender)
                {
                    Defender->RemoveUnitMovementFlag(MOVEFLAG_WALK_MODE);
                    bool move = true;
                    if (AkamaGUID)
                    {
                        if (Creature* Akama = Unit::GetCreature(*me, AkamaGUID))
                        {
                            float x, y, z;
                            Akama->GetPosition(x,y,z);
                            // They move towards AKama
                            Defender->GetMotionMaster()->MovePoint(0, x, y, z);
                            Defender->AI()->AttackStart(Akama);
                        } else move = false;
                    } else move = false;
                    if (!move)
                        Defender->GetMotionMaster()->MovePoint(0, AKAMA_X, AKAMA_Y, AKAMA_Z);
                }
                DefenderTimer = 15000;
            } else DefenderTimer -= diff;

            if (SummonTimer <= diff)
            {
                SummonCreature();
                SummonTimer = 35000;
            } else SummonTimer -= diff;

            if (DeathCount >= 6)
            {
                if (AkamaGUID)
                {
                    Creature* Akama = Unit::GetCreature((*me), AkamaGUID);
                    if (Akama && Akama->isAlive())
                    {
                        IsBanished = false;
                        me->GetMotionMaster()->MoveChase(Akama);
                        // Shade should move to Akama, not the other way around
                        Akama->GetMotionMaster()->MoveIdle();
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        // Crazy amount of threat
                        me->AddThreat(Akama, 10000000.0f);
                        Akama->AddThreat(me, 10000000.0f);
                        me->Attack(Akama, true);
                        Akama->Attack(me, true);
                    }
                }
            }
        }
        else                                                // No longer banished, let's fight Akama now
        {
            if (ReduceHealthTimer <= diff)
            {
                if (AkamaGUID)
                {
                    Creature* Akama = Unit::GetCreature((*me), AkamaGUID);
                    if (Akama && Akama->isAlive())
                    {
                        //10 % less health every few seconds.
                        me->DealDamage(Akama, Akama->GetMaxHealth()/10, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        ReduceHealthTimer = 12000;
                    }
                }
            } else ReduceHealthTimer -= diff;

            if (HasKilledAkama)
            {
                if (!HasKilledAkamaAndReseting)//do not let players kill Shade if Akama is dead and Shade is waiting for ResetTimer!! event would bug
                {
                    HasKilledAkamaAndReseting = true;
                    me->RemoveAllAuras();
                    me->DeleteThreatList();
                    me->CombatStop();
                    //me->SetHealth(me->GetMaxHealth());
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->GetMotionMaster()->MoveTargetedHome();
                }
                if (ResetTimer <= diff)
                {
                    EnterEvadeMode();// Reset a little while after killing Akama, evade and respawn Akama
                    return;
                } else ResetTimer -= diff;
            }
        }

        DoMeleeAttackIfReady();
    }
};

void mob_ashtongue_channelerAI::JustDied(Unit* /*killer*/)
{
    Creature* Shade = (Unit::GetCreature((*me), ShadeGUID));
    if (Shade && Shade->isAlive())
        CAST_AI(boss_shade_of_akamaAI, Shade->AI())->IncrementDeathCount();
    else error_log("BSCR ERROR: Channeler dead but unable to increment DeathCount for Shade of Akama.");
}

void mob_ashtongue_channelerAI::OnAuraRemove(Aura* aura, bool stackRemove)
{
    if (aura->GetSpellProto()->Id == SPELL_SHADE_SOUL_CHANNEL)
    {
        if (Unit* shade = me->GetUnit(*me, ShadeGUID))
            shade->RemoveSingleAuraFromStack(SPELL_SHADE_SOUL_CHANNEL_2, 0);
    }
}

void mob_ashtongue_sorcererAI::JustDied(Unit* /*killer*/)
{
    Creature* Shade = (Unit::GetCreature((*me), ShadeGUID));
    if (Shade && Shade->isAlive())
        CAST_AI(boss_shade_of_akamaAI, Shade->AI())->IncrementDeathCount(me->GetGUID());
    else error_log("BSCR ERROR: Sorcerer dead but unable to increment DeathCount for Shade of Akama.");
}

struct npc_akamaAI : public ScriptedAI
{
    npc_akamaAI(Creature* c) : ScriptedAI(c), summons(me)
    {
        ShadeHasDied = false;
        StartCombat = false;
        pInstance = c->GetInstanceData();
        ShadeGUID = pInstance ? pInstance->GetData64(DATA_SHADEOFAKAMA) : NOT_STARTED;
        me->setActive(true);
        EventBegun = false;
        CastSoulRetrieveTimer = 0;
        SoulRetrieveTimer = 0;
        SummonBrokenTimer = 0;
        EndingTalkCount = 0;
        WayPointId = 0;
        BrokenSummonIndex = 0;
        BrokenList.clear();
        HasYelledOnce = false;
    }

    ScriptedInstance* pInstance;

    uint64 ShadeGUID;

    uint32 DestructivePoisonTimer;
    uint32 LightningBoltTimer;
    uint32 CheckTimer;
    uint32 CastSoulRetrieveTimer;
    uint32 SoulRetrieveTimer;
    uint32 SummonBrokenTimer;
    uint32 EndingTalkCount;
    uint32 WayPointId;
    uint32 BrokenSummonIndex;

    std::list<uint64> BrokenList;

    bool EventBegun;
    bool ShadeHasDied;
    bool StartCombat;
    bool HasYelledOnce;
    SummonList summons;

    void Reset()
    {
        DestructivePoisonTimer = 15000;
        LightningBoltTimer = 10000;
        CheckTimer = 2000;

        if (!EventBegun)
        {
            me->SetUInt32Value(UNIT_NPC_FLAGS, 0);      // Database sometimes has very very strange values
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }
        summons.DespawnAll();

        me->setActive(true);
    }

    void JustSummoned(Creature* summon)
    {
        if (summon->GetEntry() == CREATURE_BROKEN)
            summons.Summon(summon);
    }
    void SummonedCreatureDespawn(Creature* summon)
    {
        if (summon->GetEntry() == CREATURE_BROKEN)
            summons.Despawn(summon);
    }

    void EnterCombat(Unit* /*who*/) {}

    void BeginEvent(Player* pl)
    {
        if (!pInstance)
            return;

        ShadeGUID = pInstance->GetData64(DATA_SHADEOFAKAMA);
        if (!ShadeGUID)
            return;

        Creature* Shade = (Unit::GetCreature((*me), ShadeGUID));
        if (Shade)
        {
            pInstance->SetData(DATA_SHADEOFAKAMAEVENT, IN_PROGRESS);
            // Prevent players from trying to restart event
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            CAST_AI(boss_shade_of_akamaAI, Shade->AI())->SetSelectableChannelers();
            CAST_AI(boss_shade_of_akamaAI, Shade->AI())->SetAkamaGUID(me->GetGUID());
            CAST_AI(boss_shade_of_akamaAI, Shade->AI())->StartCombat = true;
            Shade->AddThreat(me, 1000000.0f);
            Shade->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
            Shade->SetUInt64Value(UNIT_FIELD_TARGET, me->GetGUID());
            if (pl) Shade->AddThreat(pl, 1.0f);
            DoZoneInCombat(Shade);
            EventBegun = true;
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;

        switch (id)
        {
        case 0: ++WayPointId; break;

        case 1:
            if (Creature* Shade = Unit::GetCreature(*me, ShadeGUID))
            {
                me->SetUInt64Value(UNIT_FIELD_TARGET, ShadeGUID);
                DoCast(Shade, SPELL_AKAMA_SOUL_RETRIEVE);
                EndingTalkCount = 0;
                SoulRetrieveTimer = 16000;
            }
            break;
        }
    }

    void JustDied(Unit* /*killer*/)
    {
        DoScriptText(SAY_DEATH, me);
        EventBegun = false;
        ShadeHasDied = false;
        StartCombat = false;
        CastSoulRetrieveTimer = 0;
        SoulRetrieveTimer = 0;
        SummonBrokenTimer = 0;
        EndingTalkCount = 0;
        WayPointId = 0;
        BrokenSummonIndex = 0;
        BrokenList.clear();
        HasYelledOnce = false;
        Creature* Shade = Unit::GetCreature((*me), ShadeGUID);
        if (Shade && Shade->isAlive())
            CAST_AI(boss_shade_of_akamaAI, Shade->AI())->HasKilledAkama = true;
        summons.DespawnAll();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!EventBegun)
            return;

        if ((me->GetHealth()*100 / me->GetMaxHealth()) < 15 && !HasYelledOnce)
        {
            DoScriptText(SAY_LOW_HEALTH, me);
            HasYelledOnce = true;
        }

        if (ShadeGUID && !StartCombat)
        {
            Creature* Shade = (Unit::GetCreature((*me), ShadeGUID));
            if (Shade && Shade->isAlive())
            {
                if (CAST_AI(boss_shade_of_akamaAI, Shade->AI())->IsBanished)
                {
                    if (CastSoulRetrieveTimer <= diff)
                    {
                        DoCast(Shade, SPELL_AKAMA_SOUL_CHANNEL);
                        CastSoulRetrieveTimer = 60000;
                    } else CastSoulRetrieveTimer -= diff;
                }
                else
                {
                    me->InterruptNonMeleeSpells(false);
                    StartCombat = true;
                }
            }
        }

        if (ShadeHasDied && (WayPointId == 1))
        {
            if (pInstance)
                pInstance->SetData(DATA_SHADEOFAKAMAEVENT, DONE);
            me->GetMotionMaster()->MovePoint(WayPointId, AkamaWP[1].x, AkamaWP[1].y, AkamaWP[1].z);
            ++WayPointId;
        }

        if (!ShadeHasDied && StartCombat)
        {
            if (CheckTimer <= diff)
            {
                if (ShadeGUID)
                {
                    Creature* Shade = Unit::GetCreature((*me), ShadeGUID);
                    if (Shade && !Shade->isAlive())
                    {
                        ShadeHasDied = true;
                        WayPointId = 0;
                        me->CombatStop();
                        me->SetUnitMovementFlags(MOVEFLAG_WALK_MODE);
                        me->GetMotionMaster()->MovePoint(WayPointId, AkamaWP[0].x, AkamaWP[0].y, AkamaWP[0].z);
                    }
                    if (Shade && Shade->isAlive())
                    {
                        if (Shade->getThreatManager().getThreatList().size() < 2)
                            Shade->AI()->EnterEvadeMode();
                    }
                }
                CheckTimer = 5000;
            } else CheckTimer -= diff;
        }

        if (SummonBrokenTimer && BrokenSummonIndex < 4)
        {
            if (SummonBrokenTimer <= diff)
            {
                for (uint8 i = 0; i < 4; ++i)
                {
                    float x = BrokenCoords[BrokenSummonIndex].x + (i*5);
                    float y = BrokenCoords[BrokenSummonIndex].y + (1*5);
                    float z = BrokenCoords[BrokenSummonIndex].z;
                    float o = BrokenCoords[BrokenSummonIndex].o;
                    Creature* Broken = me->SummonCreature(CREATURE_BROKEN, x, y, z, o, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 360000);
                    if (Broken)
                    {
                        float wx = BrokenWP[BrokenSummonIndex].x + (i*5);
                        float wy = BrokenWP[BrokenSummonIndex].y + (i*5);
                        float wz = BrokenWP[BrokenSummonIndex].z;
                        Broken->GetMotionMaster()->MovePoint(0, wx, wy, wz);
                        Broken->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        BrokenList.push_back(Broken->GetGUID());
                    }
                }
                ++BrokenSummonIndex;
                SummonBrokenTimer = 1000;
            } else SummonBrokenTimer -= diff;
        }

        if (SoulRetrieveTimer)
            if (SoulRetrieveTimer <= diff)
            {
                switch (EndingTalkCount)
                {
                case 0:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
                    ++EndingTalkCount;
                    SoulRetrieveTimer = 2000;
                    SummonBrokenTimer = 1;
                    break;
                case 1:
                    DoScriptText(SAY_FREE, me);
                    ++EndingTalkCount;
                    SoulRetrieveTimer = 25000;
                    break;
                case 2:
                    if (!BrokenList.empty())
                    {
                        bool Yelled = false;
                        for (std::list<uint64>::iterator itr = BrokenList.begin(); itr != BrokenList.end(); ++itr)
                            if (Creature* pUnit = Unit::GetCreature(*me, *itr))
                            {
                                if (!Yelled)
                                {
                                    DoScriptText(SAY_BROKEN_FREE_01, pUnit);
                                    Yelled = true;
                                }
                                pUnit->HandleEmoteCommand(EMOTE_ONESHOT_KNEEL);
                            }
                    }
                    ++EndingTalkCount;
                    SoulRetrieveTimer = 1500;
                    break;
                case 3:
                    if (!BrokenList.empty())
                    {
                        for (std::list<uint64>::iterator itr = BrokenList.begin(); itr != BrokenList.end(); ++itr)
                            if (Creature* pUnit = Unit::GetCreature(*me, *itr))
                                // This is the incorrect spell, but can't seem to find the right one.
                                pUnit->CastSpell(pUnit, 39656, true);
                    }
                    ++EndingTalkCount;
                    SoulRetrieveTimer = 5000;
                    break;
                case 4:
                    if (!BrokenList.empty())
                    {
                        for (std::list<uint64>::iterator itr = BrokenList.begin(); itr != BrokenList.end(); ++itr)
                            if (Creature* pUnit = Unit::GetCreature((*me), *itr))
                                pUnit->MonsterYell(SAY_BROKEN_FREE_02, LANG_UNIVERSAL, 0);
                    }
                    SoulRetrieveTimer = 0;
                    break;
                }
            } else SoulRetrieveTimer -= diff;

        if (!UpdateVictim())
            return;

        if (DestructivePoisonTimer <= diff)
        {
            Creature* Shade = Unit::GetCreature((*me), ShadeGUID);
            if (Shade && Shade->isAlive())
                DoCast(Shade, SPELL_DESTRUCTIVE_POISON);
            DestructivePoisonTimer = 15000;
        } else DestructivePoisonTimer -= diff;

        if (LightningBoltTimer <= diff)
        {
            DoCast(me->getVictim(), SPELL_LIGHTNING_BOLT);
            LightningBoltTimer = 10000;
        } else LightningBoltTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_shade_of_akama(Creature* pCreature)
{
    return new boss_shade_of_akamaAI (pCreature);
}

CreatureAI* GetAI_mob_ashtongue_channeler(Creature* pCreature)
{
    return new mob_ashtongue_channelerAI (pCreature);
}

CreatureAI* GetAI_mob_ashtongue_sorcerer(Creature* pCreature)
{
    return new mob_ashtongue_sorcererAI (pCreature);
}

CreatureAI* GetAI_mob_ashtongue_defender(Creature* _Creature)
{
    return new mob_ashtongue_defenderAI (_Creature);
}

CreatureAI* GetAI_mob_ashtongue_spiritbinder(Creature* _Creature)
{
    return new mob_ashtongue_spiritbinderAI (_Creature);
}

CreatureAI* GetAI_mob_ashtongue_elementalist(Creature* _Creature)
{
    return new mob_ashtongue_elementalistAI (_Creature);
}

CreatureAI* GetAI_mob_ashtongue_rogue(Creature* _Creature)
{
    return new mob_ashtongue_rogueAI (_Creature);
}

CreatureAI* GetAI_npc_akama_shade(Creature* pCreature)
{
    return new npc_akamaAI (pCreature);
}

bool GossipSelect_npc_akama(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)               //Fight time
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        CAST_AI(npc_akamaAI, pCreature->AI())->BeginEvent(pPlayer);
    }

    return true;
}

bool GossipHello_npc_akama(Player* player, Creature* pCreature)
{
    ScriptedInstance* pInstance = (pCreature->GetInstanceData());

    if (pInstance && pInstance->GetData(DATA_SHADEOFAKAMAEVENT) == NOT_STARTED)
    {
        player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    }

    player->SEND_GOSSIP_MENU(907, pCreature->GetGUID());
    return true;
}

void AddSC_boss_shade_of_akama()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_shade_of_akama";
    newscript->GetAI = &GetAI_boss_shade_of_akama;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ashtongue_channeler";
    newscript->GetAI = &GetAI_mob_ashtongue_channeler;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ashtongue_sorcerer";
    newscript->GetAI = &GetAI_mob_ashtongue_sorcerer;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ashtongue_defender";
    newscript->GetAI = &GetAI_mob_ashtongue_defender;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ashtongue_spiritbinder";
    newscript->GetAI = &GetAI_mob_ashtongue_spiritbinder;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ashtongue_elementalist";
    newscript->GetAI = &GetAI_mob_ashtongue_elementalist;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ashtongue_rogue";
    newscript->GetAI = &GetAI_mob_ashtongue_rogue;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_akama_shade";
    newscript->GetAI = &GetAI_npc_akama_shade;
    newscript->pGossipHello = &GossipHello_npc_akama;
    newscript->pGossipSelect = &GossipSelect_npc_akama;
    newscript->RegisterSelf();
}
