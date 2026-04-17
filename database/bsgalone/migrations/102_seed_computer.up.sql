
-- Weapon buff
INSERT INTO computer ("id", "name", "offensive", "power_cost", "reload_time", "range", "duration", "damage_modifier")
  VALUES (
    'dd623825-be31-44c9-be89-e83b8090b161',
    'Weapon buff',
    false,
    20.0,
    10.0,
    NULL,
    3.5,
    1.5
  );

INSERT INTO computer_price ("computer", "resource", "cost")
  VALUES (
    'dd623825-be31-44c9-be89-e83b8090b161',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    6500
  );

-- Scan
INSERT INTO computer ("id", "name", "offensive", "power_cost", "reload_time", "range", "duration", "damage_modifier")
  VALUES (
    'a6bc42fc-d57c-4f0b-8194-32841e4ac564',
    'Scan',
    true,
    5.0,
    0.5,
    6.0,
    NULL,
    NULL
  );

INSERT INTO computer_price ("computer", "resource", "cost")
  VALUES (
    'a6bc42fc-d57c-4f0b-8194-32841e4ac564',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    5000
  );

INSERT INTO computer_allowed_target ("computer", "entity")
  VALUES (
    'a6bc42fc-d57c-4f0b-8194-32841e4ac564',
    'asteroid'
  );
