

INSERT INTO ship_class ("name", "jump_time", "jump_time_threat")
  VALUES ('strike', 5.0, 60.0);
INSERT INTO ship_class ("name", "jump_time", "jump_time_threat")
  VALUES ('line', 25.0, 100.0);

-- Viper Mark II
INSERT INTO ship ("id", "name", "faction", "class", "starting_ship", "max_hull_points", "hull_points_regen", "max_power_points", "power_points_regen", "max_acceleration", "max_speed", "radius")
  VALUES ('e890bb9e-91f9-434f-a6c5-2228b00336b7', 'Viper Mark II', 'colonial', 'strike', true, 450.0, 3.0, 100.0, 2.0, 5.0, 4.0, 0.5);

INSERT INTO ship_price ("ship", "resource", "cost")
  VALUES (
    'e890bb9e-91f9-434f-a6c5-2228b00336b7',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    10000
  );

INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'aafb42c1-e43f-4eeb-bfda-9abf12befc0a',
    'e890bb9e-91f9-434f-a6c5-2228b00336b7',
    'weapon',
    -0.5, 0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'e5d1469c-acfe-48a9-9c50-657f0f6fbf6c',
    'e890bb9e-91f9-434f-a6c5-2228b00336b7',
    'weapon',
    0.0, 0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'dd9bc714-c4f2-46cd-9916-803896f4e5c0',
    'e890bb9e-91f9-434f-a6c5-2228b00336b7',
    'weapon',
    0.5, 0.5, 0.0
  );

INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '91df6798-402a-4e6f-9c9d-bc048321a09c',
    'e890bb9e-91f9-434f-a6c5-2228b00336b7',
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'e7ae9a5d-a404-4a82-b2ac-abdfc696f838',
    'e890bb9e-91f9-434f-a6c5-2228b00336b7',
    'computer',
    0.0, 0.0, 0.0
  );

-- Cylon Raider
INSERT INTO ship ("id", "name", "faction", "class", "starting_ship", "max_hull_points", "hull_points_regen", "max_power_points", "power_points_regen", "max_acceleration", "max_speed", "radius")
  VALUES ('2f0c6866-b39c-44d3-bf1c-050de7984778', 'Cylon Raider', 'cylon', 'strike', true, 450.0, 3.0, 100.0, 2.0, 5.0, 4.0, 0.5);

INSERT INTO ship_price ("ship", "resource", "cost")
  VALUES (
    '2f0c6866-b39c-44d3-bf1c-050de7984778',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    10000.0
  );

INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '85e6b193-1d46-42ef-a106-01f8ff65edb0',
    '2f0c6866-b39c-44d3-bf1c-050de7984778',
    'weapon',
    -0.5, 0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'c5b0bae9-7063-4662-8729-dabfceea4eca',
    '2f0c6866-b39c-44d3-bf1c-050de7984778',
    'weapon',
    0.0, 0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'e3f537db-4357-44ff-876d-54c6177de176',
    '2f0c6866-b39c-44d3-bf1c-050de7984778',
    'weapon',
    0.5, 0.5, 0.0
  );

INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'ec8af37d-c10a-44bc-93ef-4c8c7846d5b3',
    '2f0c6866-b39c-44d3-bf1c-050de7984778',
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '9f0d3f7b-c708-4332-b16d-6b56fc27c289',
    '2f0c6866-b39c-44d3-bf1c-050de7984778',
    'computer',
    0.0, 0.0, 0.0
  );

-- Viper Mark VII
INSERT INTO ship ("id", "name", "faction", "class", "starting_ship", "max_hull_points", "hull_points_regen", "max_power_points", "power_points_regen", "max_acceleration", "max_speed", "radius")
  VALUES ('1659a64b-6c2d-439e-b823-346502ca6cfe', 'Viper Mark VII', 'colonial', 'strike', false, 585.0, 4.0, 150.0, 5.0, 4.0, 4.0, 0.5);

INSERT INTO ship_price ("ship", "resource", "cost")
  VALUES (
    '1659a64b-6c2d-439e-b823-346502ca6cfe',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    45000
  );

INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '304bd61a-6464-451a-a808-bb44dfebbe58',
    '1659a64b-6c2d-439e-b823-346502ca6cfe',
    'weapon',
    -0.5, 0.3, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '695ee29e-7609-47fa-8de4-b54c40f031f8',
    '1659a64b-6c2d-439e-b823-346502ca6cfe',
    'weapon',
    -0.2, 0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '04cb9035-a0cd-4de8-a4b2-b63788162d0c',
    '1659a64b-6c2d-439e-b823-346502ca6cfe',
    'weapon',
    0.2, 0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '1f7e7015-1edf-4ab2-92a7-9450ff0bb0bc',
    '1659a64b-6c2d-439e-b823-346502ca6cfe',
    'weapon',
    0.5, 0.3, 0.0
  );

INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'ab8a8683-5e62-488c-9b63-7fa56624af2f',
    '1659a64b-6c2d-439e-b823-346502ca6cfe',
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '6df62792-70ff-4d01-9e81-e4095563180f',
    '1659a64b-6c2d-439e-b823-346502ca6cfe',
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'd6ecb080-3510-469d-91f0-0b0ec673f2da',
    '1659a64b-6c2d-439e-b823-346502ca6cfe',
    'computer',
    0.0, 0.0, 0.0
  );

-- -- Cylon War Raider
INSERT INTO ship ("id", "name", "faction", "class", "starting_ship", "max_hull_points", "hull_points_regen", "max_power_points", "power_points_regen", "max_acceleration", "max_speed", "radius")
  VALUES ('3bf5810a-140c-41f9-9c5a-f75ed5134ab6', 'Cylon War Raider', 'cylon', 'strike', false, 585.0, 4.0, 150.0, 5.0, 4.0, 4.0, 0.5);

INSERT INTO ship_price ("ship", "resource", "cost")
  VALUES (
    '3bf5810a-140c-41f9-9c5a-f75ed5134ab6',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    45000
  );

INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '6a3a0520-b1ae-4b84-b74f-bf18b8f1a620',
    '3bf5810a-140c-41f9-9c5a-f75ed5134ab6',
    'weapon',
    -0.5, 0.3, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '430652a1-600e-4996-ba6c-149c6ab64426',
    '3bf5810a-140c-41f9-9c5a-f75ed5134ab6',
    'weapon',
    -0.2, 0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '0a0e4b2b-409a-453a-8f92-4868ccc2d31d',
    '3bf5810a-140c-41f9-9c5a-f75ed5134ab6',
    'weapon',
    0.2, 0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '02396c33-f254-4cb4-bed0-1133368a34d8',
    '3bf5810a-140c-41f9-9c5a-f75ed5134ab6',
    'weapon',
    0.5, 0.3, 0.0
  );

INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '6de7524b-8432-4579-91bb-b493ba13692b',
    '3bf5810a-140c-41f9-9c5a-f75ed5134ab6',
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'ec730fb7-dbd8-43e9-91b9-86127e44ff14',
    '3bf5810a-140c-41f9-9c5a-f75ed5134ab6',
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '7bdab5e8-9c70-4529-a5c9-32b96fce41c0',
    '3bf5810a-140c-41f9-9c5a-f75ed5134ab6',
    'computer',
    0.0, 0.0, 0.0
  );

-- -- Jotunn
INSERT INTO ship ("id", "name", "faction", "class", "starting_ship", "max_hull_points", "hull_points_regen", "max_power_points", "power_points_regen", "max_acceleration", "max_speed", "radius")
  VALUES ('2f55af73-feb9-4da5-bb93-8656159c39cd', 'Jotunn', 'colonial', 'line', false, 3000.0, 50.0, 500.0, 5.0, 1.0, 2.0, 2.0);

INSERT INTO ship_price ("ship", "resource", "cost")
  VALUES (
    '2f55af73-feb9-4da5-bb93-8656159c39cd',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    200000
  );
INSERT INTO ship_price ("ship", "resource", "cost")
  VALUES (
    '2f55af73-feb9-4da5-bb93-8656159c39cd',
    'd38effc7-6463-4c20-9794-472806c80ca7',
    100
  );

INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '6fc2b684-e3ce-43b7-b845-0584b8bef860',
    '2f55af73-feb9-4da5-bb93-8656159c39cd',
    'weapon',
    -0.5, -0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'ad579245-e85d-4eb3-ae93-44ba187e0388',
    '2f55af73-feb9-4da5-bb93-8656159c39cd',
    'weapon',
    -0.5, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'f442f11d-2bb1-43e3-9e40-187e3535a7cd',
    '2f55af73-feb9-4da5-bb93-8656159c39cd',
    'weapon',
    -0.5, 0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '72edf5b2-2fcd-4727-8c2c-ecece771402f',
    '2f55af73-feb9-4da5-bb93-8656159c39cd',
    'weapon',
    0.5, 0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '416ddf40-f11a-4d09-8bfa-36ab2d3f0d63',
    '2f55af73-feb9-4da5-bb93-8656159c39cd',
    'weapon',
    0.5, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '66b64b0c-6e35-4423-9c26-c6603e5a08bb',
    '2f55af73-feb9-4da5-bb93-8656159c39cd',
    'weapon',
    0.5, -0.5, 0.0
  );

INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '56732e6d-65f2-41e0-99b1-c552a8c85998',
    '2f55af73-feb9-4da5-bb93-8656159c39cd',
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '88779a2a-f926-4c2c-ae70-ea9aae1adb06',
    '2f55af73-feb9-4da5-bb93-8656159c39cd',
    'computer',
    0.0, 0.0, 0.0
  );

-- -- Jormung
INSERT INTO ship ("id", "name", "faction", "class", "starting_ship", "max_hull_points", "hull_points_regen", "max_power_points", "power_points_regen", "max_acceleration", "max_speed", "radius")
  VALUES ('26a2a789-a4ff-4654-88f3-2f4ec8ca4e27', 'Jormung', 'cylon', 'line', false, 3000.0, 50.0, 500.0, 5.0, 1.0, 2.0, 2.0);

INSERT INTO ship_price ("ship", "resource", "cost")
  VALUES (
    '26a2a789-a4ff-4654-88f3-2f4ec8ca4e27',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    200000
  );
INSERT INTO ship_price ("ship", "resource", "cost")
  VALUES (
    '26a2a789-a4ff-4654-88f3-2f4ec8ca4e27',
    'd38effc7-6463-4c20-9794-472806c80ca7',
    100
  );

INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '415d0960-6dfe-4cc7-a12a-025fdd9aabfd',
    '26a2a789-a4ff-4654-88f3-2f4ec8ca4e27',
    'weapon',
    -0.5, -0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'd445b5e4-0eeb-48ee-92e3-f7a62a886565',
    '26a2a789-a4ff-4654-88f3-2f4ec8ca4e27',
    'weapon',
    -0.5, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'a8403f47-74d3-4b08-ac5d-a46b1ff495bb',
    '26a2a789-a4ff-4654-88f3-2f4ec8ca4e27',
    'weapon',
    -0.5, 0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '349af754-2669-49df-a1ea-97cc9c7125af',
    '26a2a789-a4ff-4654-88f3-2f4ec8ca4e27',
    'weapon',
    0.5, 0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'e3c56e31-7071-4b44-9b4b-0d3cf2f20ba6',
    '26a2a789-a4ff-4654-88f3-2f4ec8ca4e27',
    'weapon',
    0.5, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '62aa2319-6f82-42f1-a98c-08bd4a612461',
    '26a2a789-a4ff-4654-88f3-2f4ec8ca4e27',
    'weapon',
    0.5, -0.5, 0.0
  );

INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '25f6af99-bbdc-4efe-ad0e-7d435d1d9f31',
    '26a2a789-a4ff-4654-88f3-2f4ec8ca4e27',
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '03032468-5f39-4252-8915-3c85771a3239',
    '26a2a789-a4ff-4654-88f3-2f4ec8ca4e27',
    'computer',
    0.0, 0.0, 0.0
  );

-- -- Vanir
INSERT INTO ship ("id", "name", "faction", "class", "starting_ship", "max_hull_points", "hull_points_regen", "max_power_points", "power_points_regen", "max_acceleration", "max_speed", "radius")
  VALUES ('b7a7c3d3-94dd-464d-9319-77c2311f3e73', 'Vanir', 'colonial', 'line', false, 2500.0, 35.0, 600.0, 7.0, 1.0, 2.0, 2.0);

INSERT INTO ship_price ("ship", "resource", "cost")
  VALUES (
    'b7a7c3d3-94dd-464d-9319-77c2311f3e73',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    2000000
  );

INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'ea57e9f5-55c6-479d-9f5c-e2cfbb3a81b1',
    'b7a7c3d3-94dd-464d-9319-77c2311f3e73',
    'weapon',
    -0.5, -0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'a4bc4ed0-836c-4dd6-b37a-bbcf1125a763',
    'b7a7c3d3-94dd-464d-9319-77c2311f3e73',
    'weapon',
    -0.5, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'a720ca28-39db-4ebe-97cf-e4b0b512c84f',
    'b7a7c3d3-94dd-464d-9319-77c2311f3e73',
    'weapon',
    -0.5, 0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'ed178adf-4f4b-4821-a332-e661ee38dc6d',
    'b7a7c3d3-94dd-464d-9319-77c2311f3e73',
    'weapon',
    0.5, 0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'a746d9c8-2d97-4480-acfd-33d130daf43d',
    'b7a7c3d3-94dd-464d-9319-77c2311f3e73',
    'weapon',
    0.5, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'b96d0f7c-a96e-44d7-bff4-63c34845aaef',
    'b7a7c3d3-94dd-464d-9319-77c2311f3e73',
    'weapon',
    0.5, -0.5, 0.0
  );

INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'a7d8b16f-ab32-4755-9c54-e653c85cdc87',
    'b7a7c3d3-94dd-464d-9319-77c2311f3e73',
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '5acfc751-4048-46b1-a23a-25a01db97705',
    'b7a7c3d3-94dd-464d-9319-77c2311f3e73',
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'deed79fe-82cb-4c10-8b28-a0f51be16d61',
    'b7a7c3d3-94dd-464d-9319-77c2311f3e73',
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '4b041160-aff9-4a9b-9d79-66a6dbbaa673',
    'b7a7c3d3-94dd-464d-9319-77c2311f3e73',
    'computer',
    0.0, 0.0, 0.0
  );

-- -- Hel
INSERT INTO ship ("id", "name", "faction", "class", "starting_ship", "max_hull_points", "hull_points_regen", "max_power_points", "power_points_regen", "max_acceleration", "max_speed", "radius")
  VALUES ('a6136b40-e2da-492d-b20a-675026f8ac5b', 'Hel', 'cylon', 'line', false, 2500.0, 35.0, 600.0, 7.0, 1.0, 2.0, 2.0);

INSERT INTO ship_price ("ship", "resource", "cost")
  VALUES (
    'a6136b40-e2da-492d-b20a-675026f8ac5b',
    'f9f7636d-5a23-453e-b934-840b8b3ce74b',
    2000000
  );

INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '42e01d53-975e-4eed-a623-9db467979ce6',
    'a6136b40-e2da-492d-b20a-675026f8ac5b',
    'weapon',
    -0.5, -0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '86b1b605-7dfd-48a9-8e8a-fdf3a8f55710',
    'a6136b40-e2da-492d-b20a-675026f8ac5b',
    'weapon',
    -0.5, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '071be4a3-e51f-4e36-a601-c8f4eb1092f5',
    'a6136b40-e2da-492d-b20a-675026f8ac5b',
    'weapon',
    -0.5, 0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '0b070be5-f825-4f2b-bdd9-b37f2b7c8bd0',
    'a6136b40-e2da-492d-b20a-675026f8ac5b',
    'weapon',
    0.5, 0.5, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    'cf34c918-70d4-4673-ae7a-aa2e8bc23a33',
    'a6136b40-e2da-492d-b20a-675026f8ac5b',
    'weapon',
    0.5, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '9695c157-0637-449a-8188-2d075d0b917e',
    'a6136b40-e2da-492d-b20a-675026f8ac5b',
    'weapon',
    0.5, -0.5, 0.0
  );

INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '29eb3e3a-0b21-42c0-b4d1-15b7c59e73a4',
    'a6136b40-e2da-492d-b20a-675026f8ac5b',
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '75a72ed1-0f5a-4abb-a0d6-7304b273e3ff',
    'a6136b40-e2da-492d-b20a-675026f8ac5b',
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '71696edc-dca2-44c9-9247-35b44329c428',
    'a6136b40-e2da-492d-b20a-675026f8ac5b',
    'computer',
    0.0, 0.0, 0.0
  );
INSERT INTO ship_slot ("id", "ship", "type", "x_pos", "y_pos", "z_pos")
  VALUES (
    '8fd35162-4371-411c-832a-c59349603cf8',
    'a6136b40-e2da-492d-b20a-675026f8ac5b',
    'computer',
    0.0, 0.0, 0.0
  );
