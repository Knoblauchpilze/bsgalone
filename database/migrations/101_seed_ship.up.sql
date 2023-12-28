
INSERT INTO public.ship_class ("name", "jump_time_ms", "jump_time_threat_ms")
  VALUES ('strike', 15000, 60000);
INSERT INTO public.ship_class ("name", "jump_time_ms", "jump_time_threat_ms")
  VALUES ('line', 25000, 100000);

INSERT INTO public.ship ("name", "faction", "class", "starting_ship", "max_hull_points", "hull_points_regen", "max_power_points", "power_points_regen", "max_acceleration", "max_speed", "radius")
  VALUES (
      'Viper Mark II', 'colonial', 'strike', true, 450.0, 3.0, 100.0, 2.0, 5.0, 4.0, 0.5
  );
INSERT INTO public.ship ("name", "faction", "class", "starting_ship", "max_hull_points", "hull_points_regen", "max_power_points", "power_points_regen", "max_acceleration", "max_speed", "radius")
  VALUES (
      'Cylon Raider', 'cylon', 'strike', true, 450.0, 3.0, 100.0, 2.0, 5.0, 4.0, 0.5
  );
INSERT INTO public.ship ("name", "faction", "class", "starting_ship", "max_hull_points", "hull_points_regen", "max_power_points", "power_points_regen", "max_acceleration", "max_speed", "radius")
  VALUES (
      'Jotunn', 'colonial', 'line', false, 3000.0, 50.0, 500.0, 5.0, 1.0, 2.0, 2.0
  );
INSERT INTO public.ship ("name", "faction", "class", "starting_ship", "max_hull_points", "hull_points_regen", "max_power_points", "power_points_regen", "max_acceleration", "max_speed", "radius")
  VALUES (
      'Jormung', 'cylon', 'line', false, 3000.0, 50.0, 500.0, 5.0, 1.0, 2.0, 2.0
  );

INSERT INTO public.ship_price ("ship", "resource", "cost")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Viper Mark II'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    10000.0
  );
INSERT INTO public.ship_price ("ship", "resource", "cost")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Cylon Raider'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    10000.0
  );
INSERT INTO public.ship_price ("ship", "resource", "cost")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jotunn'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    200000.0
  );
INSERT INTO public.ship_price ("ship", "resource", "cost")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jotunn'),
    (SELECT id FROM resource WHERE name = 'titane'),
    100.0
  );
INSERT INTO public.ship_price ("ship", "resource", "cost")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jormung'),
    (SELECT id FROM resource WHERE name = 'titane'),
    100.0
  );

-- Viper Mark II
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Viper Mark II'),
    'weapon',
    -0.5, 0.5, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Viper Mark II'),
    'weapon',
    0.0, 0.5, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Viper Mark II'),
    'weapon',
    0.5, 0.5, 0.0
  );

INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Viper Mark II'),
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Viper Mark II'),
    'computer',
    0.0, 0.0, 0.0
  );

-- Cylon Raider
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Cylon Raider'),
    'weapon',
    -0.5, 0.5, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Cylon Raider'),
    'weapon',
    0.0, 0.5, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Cylon Raider'),
    'weapon',
    0.5, 0.5, 0.0
  );

INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Cylon Raider'),
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Cylon Raider'),
    'computer',
    0.0, 0.0, 0.0
  );

-- Jotunn
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jotunn'),
    'weapon',
    -0.5, -0.5, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jotunn'),
    'weapon',
    -0.5, 0.0, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jotunn'),
    'weapon',
    -0.5, 0.5, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jotunn'),
    'weapon',
    0.5, 0.5, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jotunn'),
    'weapon',
    0.5, 0.0, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jotunn'),
    'weapon',
    0.5, -0.5, 0.0
  );

INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jotunn'),
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jotunn'),
    'computer',
    0.0, 0.0, 0.0
  );

-- Jormung
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jormung'),
    'weapon',
    -0.5, -0.5, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jormung'),
    'weapon',
    -0.5, 0.0, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jormung'),
    'weapon',
    -0.5, 0.5, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jormung'),
    'weapon',
    0.5, 0.5, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jormung'),
    'weapon',
    0.5, 0.0, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jormung'),
    'weapon',
    0.5, -0.5, 0.0
  );

INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jormung'),
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO public.ship_slot ("ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM ship WHERE name = 'Jormung'),
    'computer',
    0.0, 0.0, 0.0
  );
