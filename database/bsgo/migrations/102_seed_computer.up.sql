
-- Weapon buff
INSERT INTO computer ("name", "offensive", "power_cost", "reload_time", "range", "duration", "damage_modifier")
  VALUES (
    'Weapon buff', false, 20.0, 100.0, NULL, 35.0, 1.5
  );

INSERT INTO computer_price ("computer", "resource", "cost")
  VALUES (
    (SELECT id FROM computer WHERE name = 'Weapon buff'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    6500
  );

-- Scan
INSERT INTO computer ("name", "offensive", "power_cost", "reload_time", "range", "duration", "damage_modifier")
  VALUES (
    'Scan', true, 5.0, 5.0, 6.0, NULL, NULL
  );

INSERT INTO computer_price ("computer", "resource", "cost")
  VALUES (
    (SELECT id FROM computer WHERE name = 'Scan'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    5000
  );

INSERT INTO computer_allowed_target ("computer", "entity")
  VALUES (
    (SELECT id FROM computer WHERE name = 'Scan'),
    'asteroid'
  );
