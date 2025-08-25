
-- Munnin
INSERT INTO bsgalone_schema.system ("name", "x_pos", "y_pos", "z_pos")
  VALUES ('Munnin', -2.5, 3.0, 0.0);

INSERT INTO bsgalone_schema.starting_system ("system", "faction")
  VALUES (
    (SELECT id FROM system WHERE name = 'Munnin'),
    'cylon'
  );

INSERT INTO asteroid ("system", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM system WHERE name = 'Munnin'),
    85.0,
    0.5,
    1.0, 2.0, 0.0
  );
INSERT INTO asteroid_loot ("asteroid", "resource", "amount")
  VALUES (
    (SELECT id FROM asteroid WHERE health = 85.0),
    (SELECT id FROM resource WHERE name = 'tylium'),
    10.0
  );
INSERT INTO asteroid ("system", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM system WHERE name = 'Munnin'),
    222.0,
    1.0,
    4.0, 2.0, 0.0
  );
INSERT INTO asteroid ("system", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM system WHERE name = 'Munnin'),
    150.0,
    0.5,
    -2.0, -6.0, 0.0
  );
INSERT INTO asteroid_loot ("asteroid", "resource", "amount")
  VALUES (
    (SELECT id FROM asteroid WHERE health = 150.0),
    (SELECT id FROM resource WHERE name = 'titane'),
    2.0
  );
INSERT INTO asteroid ("system", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM system WHERE name = 'Munnin'),
    3333.0,
    1.0,
    -3.0, -4.0, 0.0
  );
INSERT INTO asteroid ("system", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM system WHERE name = 'Munnin'),
    2345.0,
    1.0,
    15.0, 17.0, 0.0
  );

-- 169 Aretis
INSERT INTO bsgalone_schema.system ("name", "x_pos", "y_pos", "z_pos")
  VALUES ('169 aretis', 3.0, -4.7, 0.0);

INSERT INTO bsgalone_schema.starting_system ("system", "faction")
  VALUES (
    (SELECT id FROM system WHERE name = '169 aretis'),
    'colonial'
  );

INSERT INTO asteroid ("system", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM system WHERE name = '169 aretis'),
    40.0,
    1.0,
    -1.0, -2.0, 0.0
  );
INSERT INTO asteroid_loot ("asteroid", "resource", "amount")
  VALUES (
    (SELECT id FROM asteroid WHERE health = 40.0),
    (SELECT id FROM resource WHERE name = 'tylium'),
    1234.0
  );
INSERT INTO asteroid ("system", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM system WHERE name = '169 aretis'),
    120.0,
    2.0,
    3.0, 5.0, 0.0
  );
INSERT INTO asteroid ("system", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    (SELECT id FROM system WHERE name = '169 aretis'),
    6001.0,
    1.5,
    4.0, -0.8, 0.0
  );
