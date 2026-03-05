
-- system: Munnin
INSERT INTO system_outpost ("outpost", "system", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM outpost WHERE faction = 'colonial'),
    (SELECT id FROM system WHERE name = 'Munnin'),
    12728.0,
    732.0,
    -6.0, 3.2, 0.0
  );
INSERT INTO system_outpost ("outpost", "system", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM outpost WHERE faction = 'cylon'),
    (SELECT id FROM system WHERE name = 'Munnin'),
    13400.0,
    152.0,
    6.0, -3.2, 0.0
  );

-- system: 169 aretis
INSERT INTO system_outpost ("outpost", "system", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM outpost WHERE faction = 'colonial'),
    (SELECT id FROM system WHERE name = '169 aretis'),
    5000.0,
    600.0,
    -5.0, 0.2, 0.0
  );
