INSERT INTO `blizzlike_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES
(11000, '|cffffcc00[Broadcaster]: |cff00ff00%s|r', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

CREATE TABLE IF NOT EXISTS `autobroadcast` (
  `id` int(11) NOT NULL auto_increment,
  `text` longtext NOT NULL,
  `next` int(11) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ;