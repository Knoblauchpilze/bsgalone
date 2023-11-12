
INSERT INTO public.weapon ("name", "min_damage", "max_damage", "power_cost", "range", "reload_time_ms")
  VALUES ('Short range cannon', 1.0, 10.0, 0.5, 4.0, 400);
INSERT INTO public.weapon ("name", "min_damage", "max_damage", "power_cost", "range", "reload_time_ms")
  VALUES ('Medium range cannon', 1.0, 10.0, 0.5, 5.0, 500);
INSERT INTO public.weapon ("name", "min_damage", "max_damage", "power_cost", "range", "reload_time_ms")
  VALUES ('Long range cannon', 1.0, 10.0, 0.5, 6.0, 600);

INSERT INTO public.weapon_price ("weapon", "resource", "cost")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Short range cannon'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    10000.0
  );
INSERT INTO public.weapon_price ("weapon", "resource", "cost")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Short range cannon'),
    (SELECT id FROM resource WHERE name = 'titane'),
    1000.0
  );
INSERT INTO public.weapon_price ("weapon", "resource", "cost")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Medium range cannon'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    11000.0
  );
INSERT INTO public.weapon_price ("weapon", "resource", "cost")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Long range cannon'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    12000.0
  );
