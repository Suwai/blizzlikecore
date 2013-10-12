-- Vale Moth Fix
-- old `inhabitType` = '3'
UPDATE `creature_template` SET `inhabitType` = '4' WHERE `entry` = '16520';
-- Ember of Al'ar
-- old `inhabitType` = '3'
UPDATE `creature_template` SET `inhabitType` = '4' WHERE `entry` = '19551';

-- go_silithyst_geyser
UPDATE `gameobject_template` SET ScriptName = 'go_silithyst_geyser' WHERE `entry` = '181598';