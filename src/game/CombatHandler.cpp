/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS and LICENSE files for Copyright information.
 */

#include "Common.h"
#include "Log.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "ObjectAccessor.h"
#include "CreatureAI.h"
#include "ObjectGuid.h"

void WorldSession::HandleAttackSwingOpcode(WorldPacket& recv_data)
{
    ObjectGuid guid;
    recv_data >> guid;

    DEBUG_LOG("WORLD: Recvd CMSG_ATTACKSWING Message %s", guid.GetString().c_str());

    Unit* pEnemy = ObjectAccessor::GetUnit(*_player, guid.GetRawValue());

    if (!pEnemy)
    {
        if (!guid.IsUnit())
            sLog.outError("WORLD: %s isn't player, pet or creature", guid.GetString().c_str());
        else
            sLog.outError("WORLD: Enemy %s not found", guid.GetString().c_str());

        // stop attack state at client
        SendAttackStop(NULL);
        return;
    }

    if (_player->IsFriendlyTo(pEnemy) || pEnemy->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE))
    {
        sLog.outError("WORLD: Enemy %s is friendly", guid.GetString().c_str());

        // stop attack state at client
        SendAttackStop(pEnemy);
        return;
    }

    if (!pEnemy->isAlive())
    {
        // client can generate swing to known dead target if autoswitch between autoshot and autohit is enabled in client options
        // stop attack state at client
        SendAttackStop(pEnemy);
        return;
    }

    _player->Attack(pEnemy, true);
}

void WorldSession::HandleAttackStopOpcode(WorldPacket & /*recv_data*/)
{
    GetPlayer()->AttackStop();
}

void WorldSession::HandleSetSheathedOpcode(WorldPacket& recv_data)
{
    uint32 sheathed;
    recv_data >> sheathed;

    DEBUG_LOG("WORLD: Received opcode CMSG_SETSHEATHED guidlow: %s - value: %u", GetPlayer()->GetGUIDLow(), sheathed);

    if (sheathed >= MAX_SHEATH_STATE)
    {
        sLog.outError("Unknown sheath state %u ??", sheathed);
        return;
    }

    GetPlayer()->SetSheath(SheathState(sheathed));
}

void WorldSession::SendAttackStop(Unit const* enemy)
{
    WorldPacket data(SMSG_ATTACKSTOP, (4 + 20));            // we guess size
    data << GetPlayer()->GetPackGUID();
    data << (enemy ? enemy->GetPackGUID() : PackedGuid());  // must be packed guid
    data << uint32(0);                                      // unk, can be 1 also
    SendPacket(&data);
}

