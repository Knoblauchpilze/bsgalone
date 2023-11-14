
INSERT INTO public.player_ship ("ship", "player", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
      (SELECT id FROM ship WHERE name = 'Viper Mark II'),
      (SELECT id FROM player WHERE name = 'grouton'),
      true,
      369.2,
      11.9,
      0.0,
      0.0,
      0.0
  );
INSERT INTO public.player_ship ("ship", "player", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
      (SELECT id FROM ship WHERE name = 'Cylon Raider'),
      NULL,
      true,
      280.0,
      23.2,
      0.0,
      0.0,
      0.0
  );
INSERT INTO public.player_ship ("ship", "player", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
      (SELECT id FROM ship WHERE name = 'Jotunn'),
      (SELECT id FROM player WHERE name = 'grouton'),
      false,
      3000.0,
      500.0,
      0.0,
      0.0,
      0.0
  );

INSERT INTO public.player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Medium range cannon'),
    (SELECT id FROM player WHERE name = 'grouton'),
    2
  );
INSERT INTO public.player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Short range cannon'),
    (SELECT id FROM player WHERE name = 'grouton'),
    1
  );
INSERT INTO public.player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Long range cannon'),
    (SELECT id FROM player WHERE name = 'grouton'),
    6
  );
INSERT INTO public.player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Long range cannon'),
    (SELECT id FROM player WHERE name = 'grouton'),
    7
  );
INSERT INTO public.player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Medium range cannon'),
    NULL,
    20
  );

INSERT INTO public.ship_weapon ("ship", "weapon")
  VALUES (
    (SELECT player_ship.id FROM player_ship LEFT JOIN player ON player.id = player_ship.player LEFT JOIN ship ON player_ship.ship = ship.id WHERE player.name = 'grouton' AND ship.name = 'Viper Mark II'),
    (SELECT player_weapon.id FROM player_weapon LEFT JOIN player ON player.id = player_weapon.player LEFT JOIN weapon ON player_weapon.weapon = weapon.id WHERE player.name = 'grouton' AND weapon.name = 'Medium range cannon')
  );
INSERT INTO public.ship_weapon ("ship", "weapon")
  VALUES (
    (SELECT player_ship.id FROM player_ship LEFT JOIN player ON player.id = player_ship.player LEFT JOIN ship ON player_ship.ship = ship.id WHERE player.name = 'grouton' AND ship.name = 'Viper Mark II'),
    (SELECT player_weapon.id FROM player_weapon LEFT JOIN player ON player.id = player_weapon.player LEFT JOIN weapon ON player_weapon.weapon = weapon.id WHERE player.name = 'grouton' AND weapon.name = 'Short range cannon')
  );
INSERT INTO public.ship_weapon ("ship", "weapon")
  VALUES (
    (SELECT id FROM player_ship WHERE player is NULL),
    (SELECT player_weapon.id FROM player_weapon LEFT JOIN player ON player.id = player_weapon.player LEFT JOIN weapon ON player_weapon.weapon = weapon.id WHERE player.name = 'grouton' AND weapon.name = 'Medium range cannon')
  );


INSERT INTO public.player_computer ("computer", "player", "level")
  VALUES (
    (SELECT id FROM computer WHERE name = 'Weapon buff'),
    (SELECT id FROM player WHERE name = 'grouton'),
    1
  );
INSERT INTO public.player_computer ("computer", "player", "level")
  VALUES (
    (SELECT id FROM computer WHERE name = 'Scan'),
    (SELECT id FROM player WHERE name = 'grouton'),
    5
  );
INSERT INTO public.player_computer ("computer", "player", "level")
  VALUES (
    (SELECT id FROM computer WHERE name = 'Weapon buff'),
    (SELECT id FROM player WHERE name = 'grouton'),
    6
  );

INSERT INTO public.ship_computer ("ship", "computer")
  VALUES (
    (SELECT player_ship.id FROM player_ship LEFT JOIN player ON player.id = player_ship.player LEFT JOIN ship ON player_ship.ship = ship.id WHERE player.name = 'grouton' AND ship.name = 'Viper Mark II'),
    (SELECT player_computer.id FROM player_computer LEFT JOIN player ON player.id = player_computer.player LEFT JOIN computer ON player_computer.computer = computer.id WHERE player.name = 'grouton' AND computer.name = 'Weapon buff' LIMIT 1)
  );
INSERT INTO public.ship_computer ("ship", "computer")
  VALUES (
    (SELECT player_ship.id FROM player_ship LEFT JOIN player ON player.id = player_ship.player LEFT JOIN ship ON player_ship.ship = ship.id WHERE player.name = 'grouton' AND ship.name = 'Viper Mark II'),
    (SELECT player_computer.id FROM player_computer LEFT JOIN player ON player.id = player_computer.player LEFT JOIN computer ON player_computer.computer = computer.id WHERE player.name = 'grouton' AND computer.name = 'Scan')
  );
