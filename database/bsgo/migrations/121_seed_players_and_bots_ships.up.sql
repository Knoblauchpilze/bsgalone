
-- player: colo, ship: Viper Mark II, system: Munnin
INSERT INTO player_ship ("ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
      (SELECT id FROM ship WHERE name = 'Viper Mark II'),
      (SELECT id FROM player WHERE name = 'colo'),
      'my-awesome-ship',
      true,
      369.2,
      11.9,
      0.0,
      0.0,
      0.0
  );
INSERT INTO ship_system ("ship", "system", "docked")
  VALUES (
    (SELECT ps.id FROM player_ship AS ps LEFT JOIN player AS p ON p.id = ps.player WHERE p.name = 'colo' AND ps.active = true),
    (SELECT id FROM system WHERE name = 'Munnin'),
    true
  );

INSERT INTO ship_weapon ("ship", "weapon", "slot")
  VALUES (
    (SELECT ps.id FROM player_ship AS ps LEFT JOIN player AS p ON p.id = ps.player LEFT JOIN ship AS s ON s.id = ps.ship WHERE p.name = 'colo' AND s.name = 'Viper Mark II'),
    (SELECT pw.id FROM player_weapon AS pw LEFT JOIN player AS p ON p.id = pw.player LEFT JOIN weapon AS w ON w.id = pw.weapon WHERE p.name = 'colo' AND w.name = 'Medium range cannon'),
    (SELECT ss.id FROM ship_slot AS ss LEFT JOIN ship AS s ON s.id = ss.ship WHERE s.name = 'Viper Mark II' AND ss.type = 'weapon' AND ss.x_pos = -0.5)
  );
INSERT INTO ship_weapon ("ship", "weapon", "slot")
  VALUES (
    (SELECT ps.id FROM player_ship AS ps LEFT JOIN player AS p ON p.id = ps.player LEFT JOIN ship AS s ON s.id = ps.ship WHERE p.name = 'colo' AND s.name = 'Viper Mark II'),
    (SELECT pw.id FROM player_weapon AS pw LEFT JOIN player AS p ON p.id = pw.player LEFT JOIN weapon AS w ON w.id = pw.weapon WHERE p.name = 'colo' AND w.name = 'Short range cannon'),
    (SELECT ss.id FROM ship_slot AS ss LEFT JOIN ship AS s ON s.id = ss.ship WHERE s.name = 'Viper Mark II' AND ss.type = 'weapon' AND ss.x_pos = 0.0)
  );

INSERT INTO ship_computer ("ship", "computer")
  VALUES (
    (SELECT ps.id FROM player_ship AS ps LEFT JOIN player AS p ON p.id = ps.player LEFT JOIN ship AS s ON s.id = ps.ship WHERE p.name = 'colo' AND s.name = 'Viper Mark II'),
    (SELECT pc.id FROM player_computer AS pc LEFT JOIN player AS p ON p.id = pc.player LEFT JOIN computer AS c ON c.id = pc.computer WHERE p.name = 'colo' AND c.name = 'Weapon buff' LIMIT 1)
  );
INSERT INTO ship_computer ("ship", "computer")
  VALUES (
    (SELECT ps.id FROM player_ship AS ps LEFT JOIN player AS p ON p.id = ps.player LEFT JOIN ship AS s ON s.id = ps.ship WHERE p.name = 'colo' AND s.name = 'Viper Mark II'),
    (SELECT pc.id FROM player_computer AS pc LEFT JOIN player AS p ON p.id = pc.player LEFT JOIN computer AS c ON c.id = pc.computer WHERE p.name = 'colo' AND c.name = 'Scan')
  );

-- player: colo, ship: Jotunn
INSERT INTO player_ship ("ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
      (SELECT id FROM ship WHERE name = 'Jotunn'),
      (SELECT id FROM player WHERE name = 'colo'),
      'Jotunn',
      false,
      3000.0,
      500.0,
      0.0,
      0.0,
      0.0
  );

-- player: colo2, ship: Viper Mark VII, system: 169 aretis
INSERT INTO player_ship ("ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
      (SELECT id FROM ship WHERE name = 'Viper Mark VII'),
      (SELECT id FROM player WHERE name = 'colo2'),
      'another-ship',
      true,
      10.2,
      120.9,
      0.0,
      0.0,
      0.0
  );
INSERT INTO ship_system ("ship", "system", "docked")
  VALUES (
    (SELECT ps.id FROM player_ship AS ps LEFT JOIN player AS p ON p.id = ps.player WHERE p.name = 'colo2' AND ps.active = true),
    (SELECT id FROM system WHERE name = '169 aretis'),
    true
  );

INSERT INTO ship_weapon ("ship", "weapon", "slot")
  VALUES (
    (SELECT ps.id FROM player_ship AS ps LEFT JOIN player AS p ON p.id = ps.player LEFT JOIN ship AS s ON s.id = ps.ship WHERE p.name = 'colo2' AND s.name = 'Viper Mark VII'),
    (SELECT pw.id FROM player_weapon AS pw LEFT JOIN player AS p ON p.id = pw.player LEFT JOIN weapon AS w ON w.id = pw.weapon WHERE p.name = 'colo2' AND w.name = 'Long range cannon'),
    (SELECT ss.id FROM ship_slot AS ss LEFT JOIN ship AS s ON s.id = ss.ship WHERE s.name = 'Viper Mark II' AND ss.type = 'weapon' AND ss.x_pos = 0.0)
  );

INSERT INTO ship_computer ("ship", "computer")
  VALUES (
    (SELECT ps.id FROM player_ship AS ps LEFT JOIN player AS p ON p.id = ps.player LEFT JOIN ship AS s ON s.id = ps.ship WHERE p.name = 'colo2' AND s.name = 'Viper Mark VII'),
    (SELECT pc.id FROM player_computer AS pc LEFT JOIN player AS p ON p.id = pc.player LEFT JOIN computer AS c ON c.id = pc.computer WHERE p.name = 'colo2' AND c.name = 'Scan')
  );

-- player: toast, ship: Cylon Raider, system: Munnin
INSERT INTO player_ship ("ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
      (SELECT id FROM ship WHERE name = 'Cylon Raider'),
      (SELECT id FROM player WHERE name = 'toast'),
      'my-annoying-ship',
      true,
      369.2,
      99.9,
      0.0,
      0.0,
      0.0
  );
INSERT INTO ship_system ("ship", "system", "docked")
  VALUES (
    (SELECT ps.id FROM player_ship AS ps LEFT JOIN player AS p ON p.id = ps.player WHERE p.name = 'toast' AND ps.active = true),
    (SELECT id FROM system WHERE name = 'Munnin'),
    true
  );

INSERT INTO ship_weapon ("ship", "weapon", "slot")
  VALUES (
    (SELECT ps.id FROM player_ship AS ps LEFT JOIN player AS p ON p.id = ps.player LEFT JOIN ship AS s ON s.id = ps.ship WHERE p.name = 'toast' AND s.name = 'Cylon Raider'),
    (SELECT pw.id FROM player_weapon AS pw LEFT JOIN player AS p ON p.id = pw.player LEFT JOIN weapon AS w ON w.id = pw.weapon WHERE p.name = 'toast' AND w.name = 'Medium range cannon'),
    (SELECT ss.id FROM ship_slot AS ss LEFT JOIN ship AS s ON s.id = ss.ship WHERE s.name = 'Viper Mark II' AND ss.type = 'weapon' AND ss.x_pos = 0.0)
  );

-- player: munnin_cylon_war_raider, ship: Cylon Raider, system: Munnin
INSERT INTO player_ship ("ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
      (SELECT id FROM ship WHERE name = 'Cylon Raider'),
    -- TODO: Should be replaced with the actual player id
      NULL,
      'AI Cylon Raider',
      true,
      280.0,
      23.2,
      0.0,
      0.0,
      0.0
  );
INSERT INTO ship_system ("ship", "system", "docked")
  VALUES (
    -- TODO: Should be replaced with the actual player id
    (SELECT id FROM player_ship WHERE player is NULL),
    (SELECT id FROM system WHERE name = 'Munnin'),
    false
  );

INSERT INTO ship_weapon ("ship", "weapon", "slot")
  VALUES (
    (SELECT id FROM player_ship WHERE player is NULL),
    (SELECT pw.id FROM player_weapon AS pw LEFT JOIN weapon AS w ON w.id = pw.weapon WHERE pw.player is NULL AND w.name = 'Medium range cannon'),
    (SELECT ss.id FROM ship_slot AS ss LEFT JOIN ship AS s ON s.id = ss.ship WHERE s.name = 'Cylon Raider' AND ss.type = 'weapon' AND ss.x_pos = -0.5)
  );

INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    -- TODO: Should be replaced with the actual player id
    (SELECT id FROM player_ship WHERE player is NULL),
    0,
    3.0,
    10.0,
    0.0
  );
INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    -- TODO: Should be replaced with the actual player id
    (SELECT id FROM player_ship WHERE player is NULL),
    1,
    4.0,
    -8.0,
    0.0
  );
INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    -- TODO: Should be replaced with the actual player id
    (SELECT id FROM player_ship WHERE player is NULL),
    2,
    -7.0,
    -2.0,
    0.0
  );
INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    -- TODO: Should be replaced with the actual player id
    (SELECT id FROM player_ship WHERE player is NULL),
    3,
    -5.0,
    6.0,
    0.0
  );
