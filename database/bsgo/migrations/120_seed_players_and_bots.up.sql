
-------------
-- PLAYERS --
-------------
-- player: colo
INSERT INTO player ("name", "password", "faction")
  VALUES ('colo', 'aze', 'colonial');

INSERT INTO player_resource ("player", "resource", "amount")
  VALUES (
    (SELECT id FROM player WHERE name = 'colo'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    100501
  );
INSERT INTO player_resource ("player", "resource", "amount")
  VALUES (
    (SELECT id FROM player WHERE name = 'colo'),
    (SELECT id FROM resource WHERE name = 'titane'),
    1017
  );

INSERT INTO player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Medium range cannon'),
    (SELECT id FROM player WHERE name = 'colo'),
    2
  );
INSERT INTO player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Short range cannon'),
    (SELECT id FROM player WHERE name = 'colo'),
    1
  );
INSERT INTO player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Long range cannon'),
    (SELECT id FROM player WHERE name = 'colo'),
    6
  );
INSERT INTO player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Long range cannon'),
    (SELECT id FROM player WHERE name = 'colo'),
    7
  );

INSERT INTO player_computer ("computer", "player", "level")
  VALUES (
    (SELECT id FROM computer WHERE name = 'Weapon buff'),
    (SELECT id FROM player WHERE name = 'colo'),
    1
  );
INSERT INTO player_computer ("computer", "player", "level")
  VALUES (
    (SELECT id FROM computer WHERE name = 'Scan'),
    (SELECT id FROM player WHERE name = 'colo'),
    5
  );
INSERT INTO player_computer ("computer", "player", "level")
  VALUES (
    (SELECT id FROM computer WHERE name = 'Weapon buff'),
    (SELECT id FROM player WHERE name = 'colo'),
    6
  );

-- player: colo2
INSERT INTO player ("name", "password", "faction")
  VALUES ('colo2', 'aze', 'colonial');

INSERT INTO player_resource ("player", "resource", "amount")
  VALUES (
    (SELECT id FROM player WHERE name = 'colo2'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    4567
  );

INSERT INTO player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Long range cannon'),
    (SELECT id FROM player WHERE name = 'colo2'),
    8
  );

INSERT INTO player_computer ("computer", "player", "level")
  VALUES (
    (SELECT id FROM computer WHERE name = 'Scan'),
    (SELECT id FROM player WHERE name = 'colo2'),
    5
  );

-- player: toast
INSERT INTO player ("name", "password", "faction")
  VALUES ('toast', 'aze', 'cylon');

INSERT INTO player_resource ("player", "resource", "amount")
  VALUES (
    (SELECT id FROM player WHERE name = 'toast'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    1234
  );
INSERT INTO player_resource ("player", "resource", "amount")
  VALUES (
    (SELECT id FROM player WHERE name = 'toast'),
    (SELECT id FROM resource WHERE name = 'titane'),
    56789
  );

INSERT INTO player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Medium range cannon'),
    (SELECT id FROM player WHERE name = 'toast'),
    3
  );

-------------
--  BOTS   --
-------------
-- system: Munnin, ship: Cylon Raider
INSERT INTO player ("name", "password", "faction")
  VALUES ('munnin_cylon_war_raider', 'dummy', 'cylon');

INSERT INTO player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Medium range cannon'),
    -- TODO: Should be replaced with the actual player id
    NULL,
    20
  );