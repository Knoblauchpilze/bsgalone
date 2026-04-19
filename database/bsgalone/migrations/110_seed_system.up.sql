
-- Munnin
INSERT INTO bsgalone_schema.system ("id", "name", "x_pos", "y_pos", "z_pos")
  VALUES ('52863732-d4a0-4835-a19a-c423d23f0e9d', 'Munnin', -2.5, 3.0, 0.0);

INSERT INTO tick_config ("system", "duration", "unit", "ticks")
  VALUES ('52863732-d4a0-4835-a19a-c423d23f0e9d', 1, 'seconds', 1);

INSERT INTO tick ("system", "current_tick")
  VALUES ('52863732-d4a0-4835-a19a-c423d23f0e9d', 0);

INSERT INTO starting_system ("system", "faction")
  VALUES ('52863732-d4a0-4835-a19a-c423d23f0e9d', 'cylon');

INSERT INTO asteroid ("id", "system", "max_health", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    '3355b2e1-7f9d-479c-81fd-b5f3c48f0ead',
    '52863732-d4a0-4835-a19a-c423d23f0e9d',
    85001.0,
    85.0,
    0.5,
    1.0, 2.0, 0.0
  );
INSERT INTO asteroid_loot ("asteroid", "resource", "amount")
  VALUES (
    '3355b2e1-7f9d-479c-81fd-b5f3c48f0ead',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    10
  );
INSERT INTO asteroid ("id", "system", "max_health", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    '257861a8-b795-4c40-ab00-d4f206879fe0',
    '52863732-d4a0-4835-a19a-c423d23f0e9d',
    222.0,
    222.0,
    1.0,
    4.0, 2.0, 0.0
  );
INSERT INTO asteroid ("id", "system", "max_health", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    'a338e7e1-cf8d-421c-a3f1-943395735557',
    '52863732-d4a0-4835-a19a-c423d23f0e9d',
    150.0,
    150.0,
    0.5,
    -2.0, -6.0, 0.0
  );
INSERT INTO asteroid_loot ("asteroid", "resource", "amount")
  VALUES (
    'a338e7e1-cf8d-421c-a3f1-943395735557',
    'd38effc7-6463-4c20-9794-472806c80ca7',
    2
  );
INSERT INTO asteroid ("id", "system", "max_health", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    'c69b9ab0-0f98-4297-9d4b-e91be4af0df3',
    '52863732-d4a0-4835-a19a-c423d23f0e9d',
    3333.0,
    3333.0,
    1.0,
    -3.0, -4.0, 0.0
  );
INSERT INTO asteroid ("id", "system", "max_health", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    '52aad905-d7fc-41e1-814a-5fbf4bdc6e06',
    '52863732-d4a0-4835-a19a-c423d23f0e9d',
    2345.0,
    2345.0,
    1.0,
    15.0, 17.0, 0.0
  );

-- 169 aretis
INSERT INTO bsgalone_schema.system ("id", "name", "x_pos", "y_pos", "z_pos")
  VALUES ('fb698253-68e7-49e9-bc06-ae799ce5694c', '169 aretis', 3.0, -4.7, 0.0);

INSERT INTO tick_config ("system", "duration", "unit", "ticks")
  VALUES ('fb698253-68e7-49e9-bc06-ae799ce5694c', 1, 'seconds', 1);

INSERT INTO tick ("system", "current_tick")
  VALUES ('fb698253-68e7-49e9-bc06-ae799ce5694c', 0);

INSERT INTO starting_system ("system", "faction")
  VALUES ('fb698253-68e7-49e9-bc06-ae799ce5694c', 'colonial');

INSERT INTO asteroid ("id", "system", "max_health", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    '0bf7ca5d-c224-4c62-bfd6-b27403ddc170',
    'fb698253-68e7-49e9-bc06-ae799ce5694c',
    40.0,
    40.0,
    1.0,
    -1.0, -2.0, 0.0
  );
INSERT INTO asteroid_loot ("asteroid", "resource", "amount")
  VALUES (
    '0bf7ca5d-c224-4c62-bfd6-b27403ddc170',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    1234
  );
INSERT INTO asteroid ("id", "system", "max_health", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    '26413de0-8bb6-4f16-bd9a-011f8424a931',
    'fb698253-68e7-49e9-bc06-ae799ce5694c',
    120.0,
    120.0,
    2.0,
    3.0, 5.0, 0.0
  );
INSERT INTO asteroid ("id", "system", "max_health", "health", "radius", "x_pos", "y_pos", "z_pos")
  VALUES (
    'c2a80b18-0026-4d05-a0c3-52649e3e1bb7',
    'fb698253-68e7-49e9-bc06-ae799ce5694c',
    6001.0,
    6001.0,
    1.5,
    4.0, -0.8, 0.0
  );
