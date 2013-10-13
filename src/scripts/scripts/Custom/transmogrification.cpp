/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS and LICENSE files for Copyright information.
 */

/*####
##TRANSMOGRIFICATION
####*/

#include "ScriptMgr.h"
#include "precompiled.h"
#include "Config/Config.h"

#define transmog_cost 20000 // 2 gold

bool GossipHello_transmog(Player* player, Creature* _Creature)
{
    player->ADD_GOSSIP_ITEM(0, "Christmas Outfit (2 gold)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM(0, "Mage - Tier 3 (2 gold)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM(0, "Warrior - Tier 3 (2 gold)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM(0, "Paladin - Tier 3 (2 gold)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM(0, "Warlock - Tier 3 (2 gold)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);

    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetObjectGuid());
    return true;
}

/*void UpdateGear(Player* player)//It is uncertain errors
{
    // Send update to player
    //player->SendUpdateToPlayer(player);

    // Force client to reload this player
    WorldPacket data(SMSG_FORCE_DISPLAY_UPDATE, 8);
    data << player->GetObjectGuid();
    player->SendMessageToSet(&data,true);

    // Vanish visual spell
    //player->CastSpell(player,24222,true);
}*/

bool GossipSelect_transmog(Player* player, Creature* _creature, uint32 sender, uint32 action)
{
    if(player->GetMoney() > transmog_cost)
	{
        player->ModifyMoney(-transmog_cost);
        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_1_0 , 21524);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_3_0 , 0);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_5_0 , 34085);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_6_0 , 0);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_7_0 , 0);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_8_0 , 34086);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_9_0 , 0);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_10_0 , 0);
            break;
            case GOSSIP_ACTION_INFO_DEF + 2:
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_1_0 , 22498);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_3_0 , 22499);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_5_0 , 22496);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_6_0 , 22502);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_7_0 , 22497);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_8_0 , 22500);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_9_0 , 22503);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_10_0 , 22501);
            break;
            case GOSSIP_ACTION_INFO_DEF + 3:
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_1_0 , 22418);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_3_0 , 22419);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_5_0 , 22416);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_6_0 , 22422);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_7_0 , 22417);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_8_0 , 22420);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_9_0 , 22423);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_10_0 , 22421);
            break;
            case GOSSIP_ACTION_INFO_DEF + 4:
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_1_0 , 22428);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_3_0 , 22429);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_5_0 , 22425);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_6_0 , 22431);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_7_0 , 22427);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_8_0 , 22430);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_9_0 , 22424);
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_10_0 , 22426);
           break;
            case GOSSIP_ACTION_INFO_DEF + 5:
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_1_0 , 22506); // helm
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_3_0 , 22507); // shoulder
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_5_0 , 22504); // chest
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_7_0 , 22505); // pants
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_8_0 , 22508); // Boots
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_10_0 , 22509); // gloves
            break;
        }
        player->PlayerTalkClass->CloseGossip();

		player->CastSpell(player,24222,true);//temporary

        //UpdateGear(player);//It is uncertain errors
        return true;
    }
    else
    player->PlayerTalkClass->CloseGossip();
    player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, _creature, 0, 0);
    return true;
}


void AddSC_transmog()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name="transmog";
    newscript->pGossipHello =                  &GossipHello_transmog;
    newscript->pGossipSelect =                &GossipSelect_transmog;
    newscript->RegisterSelf();
}