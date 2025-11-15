
-- https://www.postgresqltutorial.com/postgresql-tutorial/postgresql-identity-column/
-- https://www.postgresql.org/docs/current/functions-datetime.html#FUNCTIONS-DATETIME-CURRENT
CREATE TABLE player (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  account INTEGER DEFAULT NULL,
  name TEXT NOT NULL,
  faction TEXT NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP WITH TIME ZONE,
  PRIMARY KEY (id),
  UNIQUE (name),
  FOREIGN KEY (account) REFERENCES account(id),
  FOREIGN KEY (faction) REFERENCES faction(name)
);

CREATE TABLE player_resource (
  player INTEGER NOT NULL,
  resource INTEGER NOT NULL,
  amount INTEGER NOT NULL,
  PRIMARY KEY (player, resource),
  FOREIGN KEY (player) REFERENCES player(id),
  FOREIGN KEY (resource) REFERENCES resource(id)
);
CREATE TABLE player_weapon (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  weapon INTEGER NOT NULL,
  player INTEGER NOT NULL,
  level INTEGER NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP WITH TIME ZONE,
  PRIMARY KEY (id),
  FOREIGN KEY (weapon) REFERENCES weapon(id),
  FOREIGN KEY (player) REFERENCES player(id)
);

CREATE TABLE player_computer (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  computer INTEGER NOT NULL,
  player INTEGER NOT NULL,
  level INTEGER NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP WITH TIME ZONE,
  PRIMARY KEY (id),
  FOREIGN KEY (computer) REFERENCES computer(id),
  FOREIGN KEY (player) REFERENCES player(id)
);

-- https://www.reddit.com/r/PostgreSQL/comments/10shxej/how_to_update_a_timestamp_automatically/
-- https://dba.stackexchange.com/questions/149267/assignment-in-trigger-function-does-not-work
CREATE TRIGGER trigger_player_updated_at
  BEFORE UPDATE OR INSERT ON player
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();

CREATE TRIGGER trigger_player_weapon_updated_at
  BEFORE UPDATE OR INSERT ON player_weapon
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();

CREATE TRIGGER trigger_player_computer_updated_at
  BEFORE UPDATE OR INSERT ON player_computer
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();
