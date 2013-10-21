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