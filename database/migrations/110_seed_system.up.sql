
INSERT INTO public.system ("name") VALUES ('Munnin');

INSERT INTO public.asteroid ("system", "health", "radius")
  VALUES (
    (SELECT id FROM system WHERE name = 'Munnin'),
    85.0,
    0.5
  );
INSERT INTO public.asteroid ("system", "health", "radius")
  VALUES (
    (SELECT id FROM system WHERE name = 'Munnin'),
    222.0,
    1.0
  );
INSERT INTO public.asteroid ("system", "health", "radius")
  VALUES (
    (SELECT id FROM system WHERE name = 'Munnin'),
    150.0,
    0.5
  );
INSERT INTO public.asteroid ("system", "health", "radius")
  VALUES (
    (SELECT id FROM system WHERE name = 'Munnin'),
    3333.0,
    1.0
  );
INSERT INTO public.asteroid ("system", "health", "radius")
  VALUES (
    (SELECT id FROM system WHERE name = 'Munnin'),
    2345.0,
    1.0
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
  VALUES ('cylon', 19000.0, 80.0, 600.0, 12.0, 3.0);

INSERT INTO public.system_outpost ("outpost", "system", "hull_points", "power_points")
  VALUES (
    (SELECT id FROM outpost WHERE faction = 'colonial'),
    (SELECT id FROM system WHERE name = 'Munnin'),
    12728.0,
    732.0
  );
INSERT INTO public.system_outpost ("outpost", "system", "hull_points", "power_points")
  VALUES (
    (SELECT id FROM outpost WHERE faction = 'cylon'),
    (SELECT id FROM system WHERE name = 'Munnin'),
    13400.0,
    152.0
  );