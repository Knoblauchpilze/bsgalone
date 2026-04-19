
-- Short range cannon
INSERT INTO weapon ("id", "name", "min_damage", "max_damage", "power_cost", "range", "reload_time")
  VALUES (
    'b91c2cc5-b8c4-42ed-bb8e-2bc10f982843',
    'Short range cannon',
    1.0, 10.0,
    0.5,
    4.0,
    0.4
  );

INSERT INTO weapon_price ("weapon", "resource", "cost")
  VALUES (
    'b91c2cc5-b8c4-42ed-bb8e-2bc10f982843',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    10000
  );
INSERT INTO weapon_price ("weapon", "resource", "cost")
  VALUES (
    'b91c2cc5-b8c4-42ed-bb8e-2bc10f982843',
    'd38effc7-6463-4c20-9794-472806c80ca7',
    1000
  );

-- Medium range cannon
INSERT INTO weapon ("id", "name", "min_damage", "max_damage", "power_cost", "range", "reload_time")
  VALUES (
    '01f4b4ab-3314-406f-aa44-a028a14773c6',
    'Medium range cannon',
    1.0, 10.0,
    0.5,
    5.0,
    0.5
  );

INSERT INTO weapon_price ("weapon", "resource", "cost")
  VALUES (
    '01f4b4ab-3314-406f-aa44-a028a14773c6',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    11000
  );

-- Long range cannon
INSERT INTO weapon ("id", "name", "min_damage", "max_damage", "power_cost", "range", "reload_time")
  VALUES (
    'c0fea72f-3ddb-4fe1-8674-0446a18c18e5',
    'Long range cannon',
    1.0, 10.0,
    0.5,
    6.0,
    0.6);

INSERT INTO weapon_price ("weapon", "resource", "cost")
  VALUES (
    'c0fea72f-3ddb-4fe1-8674-0446a18c18e5',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    12000
  );
