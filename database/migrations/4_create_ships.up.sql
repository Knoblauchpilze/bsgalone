
CREATE TABLE ship_class (
  name TEXT NOT NULL,
  jump_time_ms INTEGER NOT NULL,
  jump_time_threat_ms INTEGER NOT NULL,
  PRIMARY KEY (name)
);

-- https://www.postgresqltutorial.com/postgresql-tutorial/postgresql-numeric/
CREATE TABLE ship (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  name TEXT NOT NULL,
  faction TEXT NOT NULL,
  class TEXT NOT NULL,
  starting_ship BOOLEAN NOT NULL,
  max_hull_points NUMERIC(8, 2) NOT NULL,
  hull_points_regen NUMERIC(8, 2) NOT NULL,
  max_power_points NUMERIC(8, 2) NOT NULL,
  power_points_regen NUMERIC(8, 2) NOT NULL,
  max_acceleration NUMERIC(8, 2) NOT NULL,
  max_speed NUMERIC(8, 2) NOT NULL,
  radius NUMERIC(8, 2) NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP WITH TIME ZONE,
  PRIMARY KEY (id),
  UNIQUE (name),
  FOREIGN KEY (faction) REFERENCES faction(name),
  FOREIGN KEY (class) REFERENCES ship_class(name)
);

CREATE TABLE ship_price (
  ship INTEGER NOT NULL,
  resource INTEGER NOT NULL,
  cost NUMERIC(10, 2) NOT NULL,
  PRIMARY KEY (ship, resource),
  FOREIGN KEY (ship) REFERENCES ship(id),
  FOREIGN KEY (resource) REFERENCES resource(id)
);

CREATE TABLE ship_slot (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  ship INTEGER NOT NULL,
  type TEXT NOT NULL,
  x_pos NUMERIC(12, 2) DEFAULT NULL,
  y_pos NUMERIC(12, 2) DEFAULT NULL,
  z_pos NUMERIC(12, 2) DEFAULT NULL,
  PRIMARY KEY (id),
  FOREIGN KEY (ship) REFERENCES ship(id),
  FOREIGN KEY (type) REFERENCES slot(type)
);

CREATE TABLE player_ship (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  ship INTEGER NOT NULL,
  player INTEGER DEFAULT NULL,
  name TEXT NOT NULL,
  active BOOLEAN NOT NULL,
  hull_points NUMERIC(8, 2) NOT NULL,
  power_points NUMERIC(8, 2) NOT NULL,
  x_pos NUMERIC(12, 2) NOT NULL,
  y_pos NUMERIC(12, 2) NOT NULL,
  z_pos NUMERIC(12, 2) NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP WITH TIME ZONE,
  PRIMARY KEY (id),
  UNIQUE (ship, player),
  FOREIGN KEY (ship) REFERENCES ship(id),
  FOREIGN KEY (player) REFERENCES player(id)
);

CREATE TRIGGER trigger_ship_updated_at
  BEFORE UPDATE OR INSERT ON ship
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();

CREATE TRIGGER trigger_player_ship_updated_at
  BEFORE UPDATE OR INSERT ON player_ship
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();
