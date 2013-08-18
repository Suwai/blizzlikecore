UPDATE `version` SET `db_version` = 'BDB_20130817';

-- Update
-- Mob Magtheridon
UPDATE `creature_template` SET `ScriptName` = 'mob_magtheridon_trigger' WHERE `entry` = '19703';


-- Update
-- old `equipment_id` = '0' `mechanic_immune_mask` = '619659263', `flags_extra` = '2'
-- Lord Sanguinar
UPDATE `creature_template` SET `equipment_id` = '2217', `mechanic_immune_mask` = '653213695', `flags_extra` = '4259842' WHERE `entry` = '20060';
-- Grand Astromancer Capernian
UPDATE `creature_template` SET `equipment_id` = '2089', `mechanic_immune_mask` = '653213695', `flags_extra` = '4259842' WHERE `entry` = '20062';
-- Master Engineer Telonicus
UPDATE `creature_template` SET `equipment_id` = '2088', `mechanic_immune_mask` = '619659263', `flags_extra` = '4259842' WHERE `entry` = '20063';
-- Thaladred the Darkener
UPDATE `creature_template` SET `equipment_id` = '2091', `mechanic_immune_mask` = '619659263', `flags_extra` = '5308416' WHERE `entry` = '20064';
-- Kael'thas Sunstrider
UPDATE `creature_template` SET `equipment_id` = '0', `mechanic_immune_mask` = '617299803', `flags_extra` = '4259841' WHERE `entry` = '19622';


-- Al'ar Phoenix God Fix
-- old `inhabitType` = '3'
UPDATE `creature_template` SET `inhabitType` = '4' WHERE `entry` = '19514';

-- Ember of Al'ar Fix
-- old `inhabitType` = '3'
UPDATE `creature_template` SET `inhabitType` = '4' WHERE `entry` = '19551';


-- Update spell_script_target
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES (30631, 1, 17454);
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES (36717, 1, 21761);



-- Magtheridon
UPDATE `creature_template` SET `mingold`=4800000, `maxgold`=5200000 WHERE `entry`=17257;
-- Gruul the Dragonkiller
UPDATE `creature_template` SET `mingold`=2300000, `maxgold`=2700000 WHERE `entry`=19044;
-- Doom Lord Kazzak
UPDATE `creature_template` SET `mingold`=4800000, `maxgold`=5200000 WHERE `entry`=18728;
-- Moroes, Maiden of Virtue, The Curator, Nightbane, Netherspite, Shade of Aran, Terestian Illhoof, Prince Malchezaar
UPDATE `creature_template` SET `mingold`=370000, `maxgold`=410000 WHERE `entry` IN (15687,16457,15691,15688,16524,15689,17225, 15690);


-- Nightbane Fix
-- old `inhabitType` = '3'
UPDATE `creature_template` SET `inhabitType` = '4', `unit_flags` = '0' WHERE `entry` = '17225';


-- Boss Shade of Aran Fix
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` = '18254';


-- DB/Waypoints: Add: Missing waypoints for Mmmrrrggglll
SET @MURLOC         :=  15937; -- Mmmrrrggglll
SET @GUID           :=  41792;
SET @PATH           := 417920;
-- Set creature location
UPDATE `creature` SET `MovementType`=2,`position_x`=8869.872,`position_y`=-5775.960,`position_z`=0.405, `spawnDist`=0, `currentwaypoint`=1 WHERE `guid`=@GUID;
DELETE FROM `creature_addon` WHERE `guid`=@GUID;
INSERT INTO `creature_addon` (`guid`, `path_id`, `bytes2`) VALUES (@GUID, @PATH, 1);
-- Waypoint Data
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH, 1, 8880.688, -5754.608, 0.25531, 0, 0, 0, 100, 0),
(@PATH, 2, 8873.341, -5735.012, 0.43308, 0, 0, 0, 100, 0),
(@PATH, 3, 8862.790, -5722.625, 0.42263, 0, 0, 0, 100, 0),
(@PATH, 4, 8846.429, -5720.661, 0.49396, 0, 0, 0, 100, 0),
(@PATH, 5, 8836.920, -5726.737, 0.83736, 0, 0, 0, 100, 0),
(@PATH, 6, 8828.789, -5743.622, 0.82590, 0, 0, 0, 100, 0),
(@PATH, 7, 8818.041, -5755.890, 0.64673, 0, 0, 0, 100, 0),
(@PATH, 8, 8804.548, -5762.792, 0.51199, 0, 0, 0, 100, 0),
(@PATH, 9, 8791.789, -5762.271, 0.45083, 0, 0, 0, 100, 0),
(@PATH, 10, 8777.225, -5753.970, 0.55571, 0, 0, 0, 100, 0),
(@PATH, 11, 8776.860, -5738.229, 0.61609, 0, 0, 0, 100, 0),
(@PATH, 12, 8785.234, -5722.790, 0.75764, 0, 0, 0, 100, 0),
(@PATH, 13, 8786.402, -5701.790, 0.67103, 0, 0, 0, 100, 0),
(@PATH, 14, 8771.277, -5696.080, 0.67596, 0, 0, 0, 100, 0),
(@PATH, 15, 8758.604, -5694.220, 0.55669, 0, 0, 0, 100, 0),
(@PATH, 16, 8746.010, -5704.514, 0.50786, 0, 0, 0, 100, 0),
(@PATH, 17, 8735.815, -5722.942, 0.94607, 0, 0, 0, 100, 0),
(@PATH, 18, 8720.269, -5730.856, 0.89406, 0, 0, 0, 100, 0),
(@PATH, 19, 8706.965, -5730.076, 0.77437, 0, 0, 0, 100, 0),
(@PATH, 20, 8693.575, -5720.971, 0.86999, 0, 0, 0, 100, 0),
(@PATH, 21, 8679.183, -5710.540, 1.10842, 0, 0, 0, 100, 0),
(@PATH, 22, 8656.022, -5704.337, 1.80973, 0, 0, 0, 100, 0),
(@PATH, 23, 8640.976, -5691.507, 0.47690, 0, 0, 0, 100, 0),
(@PATH, 24, 8635.418, -5689.706, 0.53144, 0, 0, 0, 100, 0),
(@PATH, 25, 8620.438, -5687.679, 0.86486, 0, 0, 0, 100, 0),
(@PATH, 26, 8601.687, -5688.443, 0.49338, 36000, 0, 0, 100, 0),
(@PATH, 27, 8619.985, -5679.970, 0.75128, 0, 0, 0, 100, 0),
(@PATH, 28, 8635.668, -5671.577, 0.53144, 0, 0, 0, 100, 0),
(@PATH, 29, 8648.297, -5670.150, 0.49488, 0, 0, 0, 100, 0),
(@PATH, 30, 8664.989, -5679.421, 0.55752, 0, 0, 0, 100, 0),
(@PATH, 31, 8679.183, -5710.540, 1.10842, 0, 0, 0, 100, 0),
(@PATH, 32, 8693.575, -5720.971, 0.86999, 0, 0, 0, 100, 0),
(@PATH, 33, 8706.965, -5730.076, 0.77437, 0, 0, 0, 100, 0),
(@PATH, 34, 8721.716, -5745.012, 0.62278, 0, 0, 0, 100, 0),
(@PATH, 35, 8735.529, -5753.923, 1.24721, 0, 0, 0, 100, 0),
(@PATH, 36, 8767.193, -5758.179, 0.50536, 0, 0, 0, 100, 0),
(@PATH, 37, 8791.789, -5762.271, 0.45083, 0, 0, 0, 100, 0),
(@PATH, 38, 8804.548, -5762.792, 0.51199, 0, 0, 0, 100, 0),
(@PATH, 39, 8818.041, -5755.890, 0.64673, 0, 0, 0, 100, 0),
(@PATH, 40, 8828.789, -5743.622, 0.82590, 0, 0, 0, 100, 0),
(@PATH, 41, 8836.920, -5726.737, 0.83736, 0, 0, 0, 100, 0),
(@PATH, 42, 8846.429, -5720.661, 0.49396, 0, 0, 0, 100, 0),
(@PATH, 43, 8873.341, -5735.012, 0.43308, 0, 0, 0, 100, 0),
(@PATH, 44, 8880.688, -5754.608, 0.25531, 60000, 0, 0, 100, 0);


-- Boss Kil'Jaeden - 1st phase corrections
-- correct CREATURE_HAND_OF_THE_DECEIVER [25558]
-- this need correction --------------
REPLACE INTO `creature_equip_template`
            (`entry`,
			 `equipmodel1`,
--			 `equipmodel2`,
			 `equipmodel3`,
			 `equipinfo1`,
--			 `equipinfo2`,
			 `equipinfo3`,
			 `equipslot1`,
--			 `equipslot2`,
			 `equipslot3`)
VALUES ('25588',
		'47994',
--		'47994',
		'47994',
		'33490690',
--		'33490690',
		'33490436',
		'269',						
--		'17',						
		'1038');					
-- ------------------------------------	
UPDATE `creature_template`
SET `equipment_id` = '25588',
	`minhealth` = '180810',
	`maxhealth` = '180810',
	`minmana` = '165450',
	`maxmana` = '165450',
	`minlevel` = '72',
	`maxlevel` = '72'
where `entry` = '25588';
-- correct CREATURE_VOLATILE_FELFIRE_FIEND [25598]
UPDATE `creature_template`
SET `minhealth` = '1307',
	`maxhealth` = '1307',
	`minmana` = '16155',
	`maxmana` = '16200',
	`minlevel` = '71',
	`maxlevel` = '71'
where `entry` = '25598';
-- correct CREATURE_FELFIRE_PORTAL [25603]
UPDATE `creature_template`
SET `minlevel` = '71',
	`maxlevel` = '71'
where `entry` = '25603';


-- Set BlizzLikeCore ACID Release Version Information
UPDATE `version` SET `script_version` = 'ACID 2.0.7 - Full Release 6670b67073 for BizzLikeCore';
-- ------------------------------------	
REPLACE INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
-- Demonic Vapor (Trail)
('2526701','25267','1','0','100','2','10000','10000','0','0','11','45400','0','0','0','0','0','0','0','0','0','0','Demonic Vapor (Trail) - Cast Summon Blazing Dead on OOC Timer'),
-- Unyielding Dead
('2526801','25268','11','0','100','2','0','0','0','0','38','0','0','0','11','45415','0','0','0','0','0','0','Unyielding Dead - Set in Combat with Zone and Cast Blazing Dead Passive on Spawned');
-- ------------------------------------	
UPDATE `creature_template` SET `AIName` = 'EventAI' WHERE `entry` IN (25267,25268);


-- Heroic boss flags_extra fix (TY clarkname) (old value: `flags_extra` = '0')
UPDATE `creature_template` SET `flags_extra` = '1' WHERE `entry` IN (20568,20183,20184,20169,20168);


-- Heroic boss respawn fix (TY clarkname) (old value: `spawntimesecs` = '25')
UPDATE `creature` SET `spawntimesecs` = '300' WHERE `id` = '20923';