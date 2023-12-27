
INSERT INTO public.computer ("name", "offensive", "power_cost", "reload_time_ms", "range", "duration_ms", "damage_modifier")
  VALUES (
    'Weapon buff', false, 20.0, 10000.0, NULL, 3500.0, 1.5
  );
INSERT INTO public.computer ("name", "offensive", "power_cost", "reload_time_ms", "range", "duration_ms", "damage_modifier")
  VALUES (
    'Scan', true, 5.0, 500.0, 6.0, NULL, NULL
  );

INSERT INTO public.computer_price ("computer", "resource", "cost")
  VALUES (
    (SELECT id FROM computer WHERE name = 'Weapon buff'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    6500.0
  );
INSERT INTO public.computer_price ("computer", "resource", "cost")
  VALUES (
    (SELECT id FROM computer WHERE name = 'Scan'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    5000.0
  );

INSERT INTO public.computer_allowed_target ("computer", "entity")
  VALUES (
    (SELECT id FROM computer WHERE name = 'Scan'),
    'asteroid'
  );
