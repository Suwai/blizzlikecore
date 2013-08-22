UPDATE `version` SET `db_version` = 'BDB_20130822';

-- Update ScriptName
UPDATE `creature_template` SET `ScriptName` = 'mob_ashtongue_defender' WHERE `entry` = '23216';
UPDATE `creature_template` SET `ScriptName` = 'mob_ashtongue_spiritbinder' WHERE `entry` IN (23254, 23524);
UPDATE `creature_template` SET `ScriptName` = 'mob_ashtongue_elementalist' WHERE `entry` = '23523';
UPDATE `creature_template` SET `ScriptName` = 'mob_ashtongue_rogue' WHERE `entry` = '21591';