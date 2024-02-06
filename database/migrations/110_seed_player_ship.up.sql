
INSERT INTO public.player_ship ("ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
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
INSERT INTO public.player_ship ("ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
      (SELECT id FROM ship WHERE name = 'Cylon Raider'),
      NULL,
      'AI Cylon Raider',
      true,
      280.0,
      23.2,
      0.0,
      0.0,
      0.0
  );
INSERT INTO public.player_ship ("ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
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
INSERT INTO public.player_ship ("ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
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
INSERT INTO public.player_ship ("ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
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

INSERT INTO public.player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Medium range cannon'),
    (SELECT id FROM player WHERE name = 'colo'),
    2
  );
INSERT INTO public.player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Short range cannon'),
    (SELECT id FROM player WHERE name = 'colo'),
    1
  );
INSERT INTO public.player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Long range cannon'),
    (SELECT id FROM player WHERE name = 'colo'),
    6
  );
INSERT INTO public.player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Long range cannon'),
    (SELECT id FROM player WHERE name = 'colo'),
    7
  );
INSERT INTO public.player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Long range cannon'),
    (SELECT id FROM player WHERE name = 'colo2'),
    8
  );
INSERT INTO public.player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Medium range cannon'),
    (SELECT id FROM player WHERE name = 'toast'),
    3
  );
INSERT INTO public.player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Medium range cannon'),
    NULL,
    20
  );

INSERT INTO public.ship_weapon ("ship", "weapon", "slot")
  VALUES (
    (SELECT player_ship.id FROM player_ship LEFT JOIN player ON player.id = player_ship.player LEFT JOIN ship ON player_ship.ship = ship.id WHERE player.name = 'colo' AND ship.name = 'Viper Mark II'),
    (SELECT player_weapon.id FROM player_weapon LEFT JOIN player ON player.id = player_weapon.player LEFT JOIN weapon ON player_weapon.weapon = weapon.id WHERE player.name = 'colo' AND weapon.name = 'Medium range cannon'),
    (SELECT ship_slot.id FROM ship_slot LEFT JOIN ship ON ship_slot.ship = ship.id WHERE ship.name = 'Viper Mark II' AND ship_slot.type = 'weapon' AND ship_slot.x_pos = -0.5)
  );
INSERT INTO public.ship_weapon ("ship", "weapon", "slot")
  VALUES (
    (SELECT player_ship.id FROM player_ship LEFT JOIN player ON player.id = player_ship.player LEFT JOIN ship ON player_ship.ship = ship.id WHERE player.name = 'colo' AND ship.name = 'Viper Mark II'),
    (SELECT player_weapon.id FROM player_weapon LEFT JOIN player ON player.id = player_weapon.player LEFT JOIN weapon ON player_weapon.weapon = weapon.id WHERE player.name = 'colo' AND weapon.name = 'Short range cannon'),
    (SELECT ship_slot.id FROM ship_slot LEFT JOIN ship ON ship_slot.ship = ship.id WHERE ship.name = 'Viper Mark II' AND ship_slot.type = 'weapon' AND ship_slot.x_pos = 0.0)
  );
INSERT INTO public.ship_weapon ("ship", "weapon", "slot")
  VALUES (
    (SELECT player_ship.id FROM player_ship LEFT JOIN player ON player.id = player_ship.player LEFT JOIN ship ON player_ship.ship = ship.id WHERE player.name = 'colo2' AND ship.name = 'Viper Mark VII'),
    (SELECT player_weapon.id FROM player_weapon LEFT JOIN player ON player.id = player_weapon.player LEFT JOIN weapon ON player_weapon.weapon = weapon.id WHERE player.name = 'colo2' AND weapon.name = 'Long range cannon'),
    (SELECT ship_slot.id FROM ship_slot LEFT JOIN ship ON ship_slot.ship = ship.id WHERE ship.name = 'Viper Mark II' AND ship_slot.type = 'weapon' AND ship_slot.x_pos = 0.0)
  );
INSERT INTO public.ship_weapon ("ship", "weapon", "slot")
  VALUES (
    (SELECT player_ship.id FROM player_ship LEFT JOIN player ON player.id = player_ship.player LEFT JOIN ship ON player_ship.ship = ship.id WHERE player.name = 'toast' AND ship.name = 'Cylon Raider'),
    (SELECT player_weapon.id FROM player_weapon LEFT JOIN player ON player.id = player_weapon.player LEFT JOIN weapon ON player_weapon.weapon = weapon.id WHERE player.name = 'toast' AND weapon.name = 'Medium range cannon'),
    (SELECT ship_slot.id FROM ship_slot LEFT JOIN ship ON ship_slot.ship = ship.id WHERE ship.name = 'Viper Mark II' AND ship_slot.type = 'weapon' AND ship_slot.x_pos = 0.0)
  );
INSERT INTO public.ship_weapon ("ship", "weapon", "slot")
  VALUES (
    (SELECT id FROM player_ship WHERE player is NULL),
    (SELECT player_weapon.id FROM player_weapon LEFT JOIN weapon ON player_weapon.weapon = weapon.id WHERE player_weapon.player is NULL AND weapon.name = 'Medium range cannon'),
    (SELECT ship_slot.id FROM ship_slot LEFT JOIN ship ON ship_slot.ship = ship.id WHERE ship.name = 'Cylon Raider' AND ship_slot.type = 'weapon' AND ship_slot.x_pos = -0.5)
  );


INSERT INTO public.player_computer ("computer", "player", "level")
  VALUES (
    (SELECT id FROM computer WHERE name = 'Weapon buff'),
    (SELECT id FROM player WHERE name = 'colo'),
    1
  );
INSERT INTO public.player_computer ("computer", "player", "level")
  VALUES (
    (SELECT id FROM computer WHERE name = 'Scan'),
    (SELECT id FROM player WHERE name = 'colo'),
    5
  );
INSERT INTO public.player_computer ("computer", "player", "level")
  VALUES (
    (SELECT id FROM computer WHERE name = 'Weapon buff'),
    (SELECT id FROM player WHERE name = 'colo'),
    6
  );
INSERT INTO public.player_computer ("computer", "player", "level")
  VALUES (
    (SELECT id FROM computer WHERE name = 'Scan'),
    (SELECT id FROM player WHERE name = 'colo2'),
    5
  );

INSERT INTO public.ship_computer ("ship", "computer")
  VALUES (
    (SELECT player_ship.id FROM player_ship LEFT JOIN player ON player.id = player_ship.player LEFT JOIN ship ON player_ship.ship = ship.id WHERE player.name = 'colo' AND ship.name = 'Viper Mark II'),
    (SELECT player_computer.id FROM player_computer LEFT JOIN player ON player.id = player_computer.player LEFT JOIN computer ON player_computer.computer = computer.id WHERE player.name = 'colo' AND computer.name = 'Weapon buff' LIMIT 1)
  );
INSERT INTO public.ship_computer ("ship", "computer")
  VALUES (
    (SELECT player_ship.id FROM player_ship LEFT JOIN player ON player.id = player_ship.player LEFT JOIN ship ON player_ship.ship = ship.id WHERE player.name = 'colo' AND ship.name = 'Viper Mark II'),
    (SELECT player_computer.id FROM player_computer LEFT JOIN player ON player.id = player_computer.player LEFT JOIN computer ON player_computer.computer = computer.id WHERE player.name = 'colo' AND computer.name = 'Scan')
  );

INSERT INTO public.ship_computer ("ship", "computer")
  VALUES (
    (SELECT player_ship.id FROM player_ship LEFT JOIN player ON player.id = player_ship.player LEFT JOIN ship ON player_ship.ship = ship.id WHERE player.name = 'colo2' AND ship.name = 'Viper Mark VII'),
    (SELECT player_computer.id FROM player_computer LEFT JOIN player ON player.id = player_computer.player LEFT JOIN computer ON player_computer.computer = computer.id WHERE player.name = 'colo2' AND computer.name = 'Scan')
  );
