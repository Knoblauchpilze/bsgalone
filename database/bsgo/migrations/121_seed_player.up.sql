
-- player: color
INSERT INTO player ("name", "password", "faction")
  VALUES ('colo', 'aze', 'colonial');

INSERT INTO player_resource ("player", "resource", "amount")
  VALUES (
    (SELECT id FROM player WHERE name = 'colo'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    100501.2
  );
INSERT INTO player_resource ("player", "resource", "amount")
  VALUES (
    (SELECT id FROM player WHERE name = 'colo'),
    (SELECT id FROM resource WHERE name = 'titane'),
    1017.2
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
    4567.0
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
    1234.2
  );
INSERT INTO player_resource ("player", "resource", "amount")
  VALUES (
    (SELECT id FROM player WHERE name = 'toast'),
    (SELECT id FROM resource WHERE name = 'titane'),
    56789.2
  );

INSERT INTO player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Medium range cannon'),
    (SELECT id FROM player WHERE name = 'toast'),
    3
  );
