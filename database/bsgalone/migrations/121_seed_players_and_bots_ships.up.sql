
-- player: colo, ship: Viper Mark II, system: Munnin
INSERT INTO player_ship ("id", "ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
      '83949aba-7bbd-406f-ace5-64d40bf77512',
      'e890bb9e-91f9-434f-a6c5-2228b00336b7',
      'c39142bc-14d0-4436-9242-01aa791a07cb',
      'my-awesome-ship',
      true,
      369.2,
      11.9,
      0.0, 0.0, 0.0
  );
INSERT INTO ship_system ("ship", "system", "docked")
  VALUES (
    '83949aba-7bbd-406f-ace5-64d40bf77512',
    '52863732-d4a0-4835-a19a-c423d23f0e9d',
    true
  );

INSERT INTO ship_weapon ("ship", "weapon", "slot")
  VALUES (
    '83949aba-7bbd-406f-ace5-64d40bf77512',
    'a0a5100e-b814-487d-bc9c-e9093c92dc33',
    'aafb42c1-e43f-4eeb-bfda-9abf12befc0a'
  );
INSERT INTO ship_weapon ("ship", "weapon", "slot")
  VALUES (
    '83949aba-7bbd-406f-ace5-64d40bf77512',
    '37312265-e732-4480-abf6-4bb041c132ee',
    'e5d1469c-acfe-48a9-9c50-657f0f6fbf6c'
  );

INSERT INTO ship_computer ("ship", "computer")
  VALUES (
    '83949aba-7bbd-406f-ace5-64d40bf77512',
    '5f07ea60-9731-4568-810e-e1a6a0ff4797'
  );
INSERT INTO ship_computer ("ship", "computer")
  VALUES (
    '83949aba-7bbd-406f-ace5-64d40bf77512',
    'ce3fcc0b-f3b9-4f02-9cca-1d28ef88a498'
  );

-- player: colo, ship: Jotunn
INSERT INTO player_ship ("id", "ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
      'cf39df57-842a-4ccb-bbd3-9cde5b5ea5ae',
      '2f55af73-feb9-4da5-bb93-8656159c39cd',
      'c39142bc-14d0-4436-9242-01aa791a07cb',
      'The Big Jotunn',
      false,
      3000.0,
      500.0,
      0.0, 0.0, 0.0
  );

-- player: colo2, ship: Viper Mark VII, system: 169 aretis
INSERT INTO player_ship ("id", "ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
      'fcc11591-e186-4293-aac8-0bf1f1976855',
      '1659a64b-6c2d-439e-b823-346502ca6cfe',
      'd227d520-e320-44ae-ae27-085d09beaaff',
      'another-ship',
      true,
      10.2,
      120.9,
      0.0, 0.0, 0.0
  );
INSERT INTO ship_system ("ship", "system", "docked")
  VALUES (
    'fcc11591-e186-4293-aac8-0bf1f1976855',
    'fb698253-68e7-49e9-bc06-ae799ce5694c',
    true
  );

INSERT INTO ship_weapon ("ship", "weapon", "slot")
  VALUES (
    'fcc11591-e186-4293-aac8-0bf1f1976855',
    'f357950f-f106-461f-81eb-974ae9c72fff',
    '695ee29e-7609-47fa-8de4-b54c40f031f8'
  );

INSERT INTO ship_computer ("ship", "computer")
  VALUES (
    'fcc11591-e186-4293-aac8-0bf1f1976855',
    '97cf751b-3210-4787-b4c1-00350ef4b8bb'
  );

-- player: toast, ship: Cylon Raider, system: Munnin
INSERT INTO player_ship ("id", "ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
      '3d6f444c-f787-4e6f-9f06-a353664d0f88',
      '2f0c6866-b39c-44d3-bf1c-050de7984778',
      '5560aacc-1121-4513-9f4f-6f64814871ad',
      'my-annoying-ship',
      true,
      369.2,
      99.9,
      0.0, 0.0, 0.0
  );
INSERT INTO ship_system ("ship", "system", "docked")
  VALUES (
    '3d6f444c-f787-4e6f-9f06-a353664d0f88',
    '52863732-d4a0-4835-a19a-c423d23f0e9d',
    true
  );

INSERT INTO ship_weapon ("ship", "weapon", "slot")
  VALUES (
    '3d6f444c-f787-4e6f-9f06-a353664d0f88',
    'bd0b66ac-f758-43c1-8121-d6d977fb5c46',
    'c5b0bae9-7063-4662-8729-dabfceea4eca'
  );

-- player: cylon bot, ship: Cylon Raider, system: Munnin
INSERT INTO player_ship ("id", "ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
      '701a7c52-59b4-421b-ad46-b32fa5c551f5',
      '2f0c6866-b39c-44d3-bf1c-050de7984778',
      'b40bce21-594b-4178-8ccb-e313174e4be1',
      'Cylon Raider',
      true,
      280.0,
      23.2,
      0.0, 0.0, 0.0
  );
INSERT INTO ship_system ("ship", "system", "docked")
  VALUES (
    '701a7c52-59b4-421b-ad46-b32fa5c551f5',
    '52863732-d4a0-4835-a19a-c423d23f0e9d',
    false
  );

INSERT INTO ship_weapon ("ship", "weapon", "slot")
  VALUES (
    '701a7c52-59b4-421b-ad46-b32fa5c551f5',
    'ff3b5288-c06a-4650-9537-ce3b1a136ec2',
    '85e6b193-1d46-42ef-a106-01f8ff65edb0'
  );

INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    '701a7c52-59b4-421b-ad46-b32fa5c551f5',
    0,
    3.0, 10.0, 0.0
  );
INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    '701a7c52-59b4-421b-ad46-b32fa5c551f5',
    1,
    4.0, -8.0, 0.0
  );
INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    '701a7c52-59b4-421b-ad46-b32fa5c551f5',
    2,
    -7.0, -2.0, 0.0
  );
INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    '701a7c52-59b4-421b-ad46-b32fa5c551f5',
    3,
    -5.0, 6.0, 0.0
  );

INSERT INTO ai_behaviors ("ship", "index")
  VALUES ('701a7c52-59b4-421b-ad46-b32fa5c551f5', 0);

-- player: colonial bot, ship: Jotunn, system: Munnin
INSERT INTO player_ship ("id", "ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
      '63c31dda-142c-416d-8596-ef47015381b1',
      '2f55af73-feb9-4da5-bb93-8656159c39cd',
      '363d4fce-73e3-4b9b-9c17-c7700faf7c38',
      'Jotunn',
      true,
      1234.0,
      98.7,
      0.0, 0.0, 0.0
  );
INSERT INTO ship_system ("ship", "system", "docked")
  VALUES (
    '63c31dda-142c-416d-8596-ef47015381b1',
    '52863732-d4a0-4835-a19a-c423d23f0e9d',
    false
  );

INSERT INTO ship_weapon ("ship", "weapon", "slot")
  VALUES (
    '63c31dda-142c-416d-8596-ef47015381b1',
    '991160e2-211e-435b-80ba-87db3aa9aa0e',
    '6fc2b684-e3ce-43b7-b845-0584b8bef860'
  );

INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    '63c31dda-142c-416d-8596-ef47015381b1',
    0,
    -7.0, -6.0, 0.0
  );
INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    '63c31dda-142c-416d-8596-ef47015381b1',
    1,
    4.0, 8.0, 0.0
  );

INSERT INTO ai_behaviors ("ship", "index")
  VALUES ('63c31dda-142c-416d-8596-ef47015381b1', 0);


-- player: colonial bot, ship: Viper Mark VII, system: 169 aretis
INSERT INTO player_ship ("id", "ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
      '8ec04faa-173f-48bc-ac2b-ac8f1436eef3',
      '1659a64b-6c2d-439e-b823-346502ca6cfe',
      '24aa9a66-d2a1-4131-9222-6d5440088ebf',
      'Viper Mark VII',
      true,
      48.0,
      98.7,
      0.0, 0.0, 0.0
  );
INSERT INTO ship_system ("ship", "system", "docked")
  VALUES (
    '8ec04faa-173f-48bc-ac2b-ac8f1436eef3',
    'fb698253-68e7-49e9-bc06-ae799ce5694c',
    false
  );

INSERT INTO ship_weapon ("ship", "weapon", "slot")
  VALUES (
    '8ec04faa-173f-48bc-ac2b-ac8f1436eef3',
    '590b37dc-fb7d-409d-b511-2d2ff6fcaaa0',
    '304bd61a-6464-451a-a808-bb44dfebbe58'
  );

INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    '8ec04faa-173f-48bc-ac2b-ac8f1436eef3',
    0,
    -10.0, -11.0, 0.0
  );
INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    '8ec04faa-173f-48bc-ac2b-ac8f1436eef3',
    1,
    -5.0, 0.0, 0.0
  );
INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    '8ec04faa-173f-48bc-ac2b-ac8f1436eef3',
    2,
    -11.0, 10.0, 0.0
  );
INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    '8ec04faa-173f-48bc-ac2b-ac8f1436eef3',
    3,
    0.0, 5.0, 0.0
  );
INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    '8ec04faa-173f-48bc-ac2b-ac8f1436eef3',
    4,
    11.0, 10.0, 0.0
  );
INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    '8ec04faa-173f-48bc-ac2b-ac8f1436eef3',
    5,
    5.0, 0.0, 0.0
  );
INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    '8ec04faa-173f-48bc-ac2b-ac8f1436eef3',
    6,
    11.0, -10.0, 0.0
  );
INSERT INTO ai_targets ("ship", "index", "x_pos", "y_pos", "z_pos")
  VALUES (
    '8ec04faa-173f-48bc-ac2b-ac8f1436eef3',
    7,
    0.0, -5.0, 0.0
  );

INSERT INTO ai_behaviors ("ship", "index")
  VALUES ('8ec04faa-173f-48bc-ac2b-ac8f1436eef3', 0);
