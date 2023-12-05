
-- https://www.postgresqltutorial.com/postgresql-plpgsql/postgresql-create-procedure/
-- https://stackoverflow.com/questions/26406111/execute-postgresql-stored-procedure-as-one-transaction
CREATE PROCEDURE player_signup (
  player_name text,
  player_password text,
  player_faction text
)
LANGUAGE plpgsql
AS $$
BEGIN
    -- insert in player table
    INSERT INTO player("name", "password", "faction")
      VALUES (player_name, player_password, player_faction);

    -- register some resources for the new player
    INSERT INTO player_resource ("player", "resource", "amount")
      VALUES
      (
        (SELECT id FROM player WHERE name = player_name),
        (SELECT id FROM resource WHERE name = 'tylium'),
        10000.0
      ),
      (
        (SELECT id FROM player WHERE name = player_name),
        (SELECT id FROM resource WHERE name = 'titane'),
        10000.0
      );

    -- create the first player ship
    INSERT INTO player_ship ("ship", "player", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
      VALUES (
        (SELECT id FROM ship WHERE starting_ship = true AND faction = player_faction),
        (SELECT id FROM player WHERE name = player_name),
        true,
        (SELECT max_hull_points FROM ship WHERE starting_ship = true AND faction = player_faction),
        (SELECT max_power_points FROM ship WHERE starting_ship = true AND faction = player_faction),
        0.0,
        0.0,
        0.0
      );

    -- spawn the ship in the starting system
    INSERT INTO public.ship_system ("ship", "system")
      VALUES (
        (SELECT player_ship.id FROM player_ship LEFT JOIN player ON player_ship.player = player.id WHERE player.name = player_name),
        (SELECT system FROM starting_system WHERE faction = player_faction)
      );
END;$$
