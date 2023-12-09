
CREATE PROCEDURE player_buy_weapon (
  player_id INTEGER,
  weapon_id INTEGER
)
LANGUAGE plpgsql
AS $$
BEGIN
  -- insert in player weapon table
  INSERT INTO player_weapon ("weapon", "player", "level")
    VALUES (weapon_id, player_id, 1);

  -- decrease the amount of resources
  UPDATE player_resource SET
    amount = player_resource.amount - weapon_price.cost
  FROM
    weapon_price
  WHERE
    player_resource.resource = weapon_price.resource
    AND player_resource.player = player_id
    AND weapon_price.weapon = weapon_id;
END
$$;

CREATE PROCEDURE player_buy_computer (
  player_id INTEGER,
  computer_id INTEGER
)
LANGUAGE plpgsql
AS $$
BEGIN
  -- insert in player computer table
  INSERT INTO player_computer ("computer", "player", "level")
    VALUES (computer_id, player_id, 1);

  -- decrease the amount of resources
  UPDATE player_resource SET
    amount = player_resource.amount - computer_price.cost
  FROM
    computer_price
  WHERE
    player_resource.resource = computer_price.resource
    AND player_resource.player = player_id
    AND computer_price.computer = computer_id;
END
$$;
