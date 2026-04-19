
-------------
-- PLAYERS --
-------------
-- player: colo
INSERT INTO account ("id", "name", "password")
  VALUES ('c744012a-7953-4eff-95c6-e1958e195942', 'colo', 'aze');

INSERT INTO player ("id", "account", "name", "faction")
  VALUES (
    'c39142bc-14d0-4436-9242-01aa791a07cb',
    'c744012a-7953-4eff-95c6-e1958e195942',
    'colo',
    'colonial'
  );

INSERT INTO player_role ("player", "role")
  VALUES ('c39142bc-14d0-4436-9242-01aa791a07cb', 'pilot');

INSERT INTO player_resource ("player", "resource", "amount")
  VALUES (
    'c39142bc-14d0-4436-9242-01aa791a07cb',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    100501
  );
INSERT INTO player_resource ("player", "resource", "amount")
  VALUES (
    'c39142bc-14d0-4436-9242-01aa791a07cb',
    'd38effc7-6463-4c20-9794-472806c80ca7',
    1017
  );

INSERT INTO player_weapon ("id", "weapon", "player", "level")
  VALUES (
    'a0a5100e-b814-487d-bc9c-e9093c92dc33',
    '01f4b4ab-3314-406f-aa44-a028a14773c6',
    'c39142bc-14d0-4436-9242-01aa791a07cb',
    2
  );
INSERT INTO player_weapon ("id", "weapon", "player", "level")
  VALUES (
    '37312265-e732-4480-abf6-4bb041c132ee',
    'b91c2cc5-b8c4-42ed-bb8e-2bc10f982843',
    'c39142bc-14d0-4436-9242-01aa791a07cb',
    1
  );
INSERT INTO player_weapon ("id", "weapon", "player", "level")
  VALUES (
    '2e288bd9-ddc3-4d3e-944c-8ef9dedcf813',
    'c0fea72f-3ddb-4fe1-8674-0446a18c18e5',
    'c39142bc-14d0-4436-9242-01aa791a07cb',
    6
  );
INSERT INTO player_weapon ("id", "weapon", "player", "level")
  VALUES (
    '1c68b53f-6f4a-46bd-86dc-fc713b32a920',
    'c0fea72f-3ddb-4fe1-8674-0446a18c18e5',
    'c39142bc-14d0-4436-9242-01aa791a07cb',
    7
  );

INSERT INTO player_computer ("id", "computer", "player", "level")
  VALUES (
    '5f07ea60-9731-4568-810e-e1a6a0ff4797',
    'dd623825-be31-44c9-be89-e83b8090b161',
    'c39142bc-14d0-4436-9242-01aa791a07cb',
    1
  );
INSERT INTO player_computer ("id", "computer", "player", "level")
  VALUES (
    'ce3fcc0b-f3b9-4f02-9cca-1d28ef88a498',
    'a6bc42fc-d57c-4f0b-8194-32841e4ac564',
    'c39142bc-14d0-4436-9242-01aa791a07cb',
    5
  );
INSERT INTO player_computer ("id", "computer", "player", "level")
  VALUES (
    'f62524a3-7ccf-460e-8484-5ef91acce298',
    'dd623825-be31-44c9-be89-e83b8090b161',
    'c39142bc-14d0-4436-9242-01aa791a07cb',
    6
  );

-- player: colo2
INSERT INTO account ("id", "name", "password")
  VALUES ('e207106d-a835-497b-a3df-ec3dce9effed', 'colo2', 'aze');

INSERT INTO player ("id", "account", "name", "faction")
  VALUES (
    'd227d520-e320-44ae-ae27-085d09beaaff',
    'e207106d-a835-497b-a3df-ec3dce9effed',
    'colo2',
    'colonial'
  );

INSERT INTO player_role ("player", "role")
  VALUES ('d227d520-e320-44ae-ae27-085d09beaaff', 'pilot');

INSERT INTO player_resource ("player", "resource", "amount")
  VALUES (
    'd227d520-e320-44ae-ae27-085d09beaaff',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    4567
  );

INSERT INTO player_weapon ("id", "weapon", "player", "level")
  VALUES (
    'f357950f-f106-461f-81eb-974ae9c72fff',
    'c0fea72f-3ddb-4fe1-8674-0446a18c18e5',
    'd227d520-e320-44ae-ae27-085d09beaaff',
    8
  );

INSERT INTO player_computer ("id", "computer", "player", "level")
  VALUES (
    '97cf751b-3210-4787-b4c1-00350ef4b8bb',
    'a6bc42fc-d57c-4f0b-8194-32841e4ac564',
    'd227d520-e320-44ae-ae27-085d09beaaff',
    5
  );

-- player: toast
INSERT INTO account ("id", "name", "password")
  VALUES ('6b185072-33dc-4cde-a98d-2933f47ae314', 'toast', 'aze');

INSERT INTO player ("id", "account", "name", "faction")
  VALUES (
    '5560aacc-1121-4513-9f4f-6f64814871ad',
    '6b185072-33dc-4cde-a98d-2933f47ae314',
    'toast',
    'cylon'
  );

INSERT INTO player_role ("player", "role")
  VALUES ('5560aacc-1121-4513-9f4f-6f64814871ad', 'pilot');

INSERT INTO player_resource ("player", "resource", "amount")
  VALUES (
    '5560aacc-1121-4513-9f4f-6f64814871ad',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    1234
  );
INSERT INTO player_resource ("player", "resource", "amount")
  VALUES (
    '5560aacc-1121-4513-9f4f-6f64814871ad',
    'd38effc7-6463-4c20-9794-472806c80ca7',
    56789
  );

INSERT INTO player_weapon ("id", "weapon", "player", "level")
  VALUES (
    'bd0b66ac-f758-43c1-8121-d6d977fb5c46',
    '01f4b4ab-3314-406f-aa44-a028a14773c6',
    '5560aacc-1121-4513-9f4f-6f64814871ad',
    3
  );

-------------
--  BOTS   --
-------------
-- system: Munnin, ship: Cylon Raider
INSERT INTO player ("id", "account", "name", "faction")
  VALUES ('b40bce21-594b-4178-8ccb-e313174e4be1', NULL, 'AI Cylon Raider', 'cylon');

INSERT INTO player_role ("player", "role")
  VALUES ('b40bce21-594b-4178-8ccb-e313174e4be1', 'pilot');

INSERT INTO player_weapon ("id", "weapon", "player", "level")
  VALUES (
    'ff3b5288-c06a-4650-9537-ce3b1a136ec2',
    '01f4b4ab-3314-406f-aa44-a028a14773c6',
    'b40bce21-594b-4178-8ccb-e313174e4be1',
    20
  );

-- system: Munnin, ship: Jotunn
INSERT INTO player ("id", "account", "name", "faction")
  VALUES ('363d4fce-73e3-4b9b-9c17-c7700faf7c38', NULL, 'AI Jotunn', 'colonial');

INSERT INTO player_role ("player", "role")
  VALUES ('363d4fce-73e3-4b9b-9c17-c7700faf7c38', 'pilot');

INSERT INTO player_weapon ("id", "weapon", "player", "level")
  VALUES (
    '991160e2-211e-435b-80ba-87db3aa9aa0e',
    'b91c2cc5-b8c4-42ed-bb8e-2bc10f982843',
    '363d4fce-73e3-4b9b-9c17-c7700faf7c38',
    20
  );

-- system: 169 aretis, ship: Viper Mark VII
INSERT INTO player ("id", "account", "name", "faction")
  VALUES ('24aa9a66-d2a1-4131-9222-6d5440088ebf', NULL, 'AI Viper Mark VII', 'colonial');

INSERT INTO player_role ("player", "role")
  VALUES ('24aa9a66-d2a1-4131-9222-6d5440088ebf', 'pilot');

INSERT INTO player_weapon ("id", "weapon", "player", "level")
  VALUES (
    '590b37dc-fb7d-409d-b511-2d2ff6fcaaa0',
    '01f4b4ab-3314-406f-aa44-a028a14773c6',
    '24aa9a66-d2a1-4131-9222-6d5440088ebf',
    5
  );