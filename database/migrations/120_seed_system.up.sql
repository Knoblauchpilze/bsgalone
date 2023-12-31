
INSERT INTO public.system ("name", "x_pos", "y_pos", "z_pos")
  VALUES ('Munnin', -2.5, 3.0, 0.0);
INSERT INTO public.system ("name", "x_pos", "y_pos", "z_pos")
  VALUES ('169 aretis', 3.0, -4.7, 0.0);

INSERT INTO public.starting_system ("system", "faction")
  VALUES (
    (SELECT id FROM system WHERE name = 'Munnin'),
    'cylon'
  );
INSERT INTO public.starting_system ("system", "faction")
  VALUES (
    (SELECT id FROM system WHERE name = '169 aretis'),
    'colonial'
  );

INSERT INTO public.ship_system ("ship", "system", "docked")
  VALUES (
    (SELECT player_ship.id FROM player_ship LEFT JOIN player ON player_ship.player = player.id WHERE player.name = 'grouton' AND player_ship.active = true),
    (SELECT id FROM system WHERE name = 'Munnin'),
    false
  );
INSERT INTO public.ship_system ("ship", "system", "docked")
  VALUES (
    (SELECT player_ship.id FROM player_ship WHERE player_ship.player is NULL),
    (SELECT id FROM system WHERE name = 'Munnin'),
    false
  );

INSERT INTO public.asteroid ("system", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM system WHERE name = 'Munnin'),
    85.0,
    0.5,
    1.0,
    2.0,
    0.0
  );
INSERT INTO public.asteroid ("system", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM system WHERE name = 'Munnin'),
    222.0,
    1.0,
    4.0,
    2.0,
    0.0
  );
INSERT INTO public.asteroid ("system", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM system WHERE name = 'Munnin'),
    150.0,
    0.5,
    -2.0,
    -6.0,
    0.0
  );
INSERT INTO public.asteroid ("system", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM system WHERE name = 'Munnin'),
    3333.0,
    1.0,
    -3.0,
    -4.0,
    0.0
  );
INSERT INTO public.asteroid ("system", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM system WHERE name = 'Munnin'),
    2345.0,
    1.0,
    15.0,
    17.0,
    0.0
  );

INSERT INTO public.asteroid_loot ("asteroid", "resource", "amount")
  VALUES (
    (SELECT id FROM asteroid WHERE health = 85.0),
    (SELECT id FROM resource WHERE name = 'tylium'),
    10.0
  );
INSERT INTO public.asteroid_loot ("asteroid", "resource", "amount")
  VALUES (
    (SELECT id FROM asteroid WHERE health = 150.0),
    (SELECT id FROM resource WHERE name = 'titane'),
    2.0
  );

INSERT INTO outpost ("faction", "max_hull_points", "hull_points_regen", "max_power_points", "power_points_regen", "radius")
  VALUES ('colonial', 30000.0, 120.0, 4500.0, 100.0, 2.0);
INSERT INTO outpost ("faction", "max_hull_points", "hull_points_regen", "max_power_points", "power_points_regen", "radius")
  VALUES ('cylon', 30000.0, 120.0, 4500.0, 100.0, 2.0);

INSERT INTO public.system_outpost ("outpost", "system", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM outpost WHERE faction = 'colonial'),
    (SELECT id FROM system WHERE name = 'Munnin'),
    12728.0,
    732.0,
    -6.0,
    3.2,
    0.0
  );
INSERT INTO public.system_outpost ("outpost", "system", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM outpost WHERE faction = 'cylon'),
    (SELECT id FROM system WHERE name = 'Munnin'),
    13400.0,
    152.0,
    6.0,
    -3.2,
    0.0
  );
