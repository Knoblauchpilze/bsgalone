
-- system: Munnin, ship: Cylon Raider
INSERT INTO player_ship ("ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
  VALUES (
      (SELECT id FROM ship WHERE name = 'Cylon Raider'),
      NULL,
      'AI Cylon Raider',
      true,
      280.0,
      23.2,
      0.0,
      0.0,
      0.0
  );

INSERT INTO player_weapon ("weapon", "player", "level")
  VALUES (
    (SELECT id FROM weapon WHERE name = 'Medium range cannon'),
    NULL,
    20
  );

INSERT INTO ship_weapon ("ship", "weapon", "slot")
  VALUES (
    (SELECT id FROM player_ship WHERE player is NULL),
    (SELECT player_weapon.id FROM player_weapon LEFT JOIN weapon ON player_weapon.weapon = weapon.id WHERE player_weapon.player is NULL AND weapon.name = 'Medium range cannon'),
    (SELECT ship_slot.id FROM ship_slot LEFT JOIN ship ON ship_slot.ship = ship.id WHERE ship.name = 'Cylon Raider' AND ship_slot.type = 'weapon' AND ship_slot.x_pos = -0.5)
  );

INSERT INTO ship_system ("ship", "system", "docked")
  VALUES (
    (SELECT player_ship.id FROM player_ship WHERE player_ship.player is NULL),
    (SELECT id FROM system WHERE name = 'Munnin'),
    false
  );
