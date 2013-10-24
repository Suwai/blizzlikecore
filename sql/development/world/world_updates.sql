ALTER TABLE db_version CHANGE COLUMN required_world_13_09_2013_12250317 required_world_13_09_2013_12250317 bit;

-- Vale Moth Fix
-- old `inhabitType` = '3'
UPDATE `creature_template` SET `inhabitType` = '4' WHERE `entry` = '16520';
-- Ember of Al'ar
-- old `inhabitType` = '3'
UPDATE `creature_template` SET `inhabitType` = '4' WHERE `entry` = '19551';

-- Go Silithyst Geyser
UPDATE `gameobject_template` SET `ScriptName` = 'go_silithyst_geyser' WHERE `entry` = '181598';
-- Help Account Create
UPDATE `command` SET help = 'Syntax: .account create $account $password [$expansion]\r\n\r\nCreate account and set password to it. Optionally, you may also set another expansion for this account than the defined default value.' WHERE name = 'account create';
-- NPC Netherspite Portal
UPDATE `creature_template` SET `ScriptName` = 'npc_netherspite_portal' WHERE `entry` IN (17367,17368,17369);

-- Update:Mission Gateways Murketh and Shaadraz (by ysfl <ysfllxcn@live.com>)
REPLACE INTO `creature_ai_scripts` (`id`, `creature_id`, `event_type`, `event_inverse_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action1_type`, `action1_param1`, `action1_param2`, `action1_param3`, `action2_type`, `action2_param1`, `action2_param2`, `action2_param3`, `action3_type`, `action3_param1`, `action3_param2`, `action3_param3`, `comment`) VALUES (1929201, 19292, 8, 0, 100, 0, 33655, -1, 0, 0, 33, 19292, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Legion Transporter: Beta - Quest Credit on Dropping The Nether Modulator Spellhit (Quest: 10146)');
REPLACE INTO `creature_ai_scripts` (`id`, `creature_id`, `event_type`, `event_inverse_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action1_type`, `action1_param1`, `action1_param2`, `action1_param3`, `action2_type`, `action2_param1`, `action2_param2`, `action2_param3`, `action3_type`, `action3_param1`, `action3_param2`, `action3_param3`, `comment`) VALUES (1929101, 19291, 8, 0, 100, 0, 33655, -1, 0, 0, 33, 19291, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Legion Transporter: Alpha - Quest Credit on Dropping The Nether Modulator Spellhit (Quest: 10129)');
UPDATE `creature_template` SET `AIName`='EventAI' WHERE  `entry`=19291;
UPDATE `creature_template` SET `AIName`='EventAI' WHERE  `entry`=19292;
-- Repair_Quest_GUID_9723_NPC
UPDATE `creature_template` SET `minlevel` = '60', `maxlevel` = '60', `maxhealth` = '3451', `maxmana` = '2475' WHERE `entry` = '25223';

-- Fix some errors (by Vstar <vstar0v0@hotmail.com>)
UPDATE `creature_template_addon` SET `b2_0_sheath` = '1', `auras` = '1787 1 1787 0 18950 0 18950 1' WHERE `entry` = '16164';