
-- https://www.postgresqltutorial.com/postgresql-tutorial/postgresql-numeric/
CREATE TABLE ship (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  name TEXT NOT NULL,
  faction TEXT NOT NULL,
  max_hull_points NUMERIC(8, 2) NOT NULL,
  hull_points_regen NUMERIC(8, 2) NOT NULL,
  max_power_points NUMERIC(8, 2) NOT NULL,
  power_points_regen NUMERIC(8, 2) NOT NULL,
  max_acceleration NUMERIC(8, 2) NOT NULL,
  max_speed NUMERIC(8, 2) NOT NULL,
  radius NUMERIC(8, 2) NOT NULL,
  PRIMARY KEY (id),
  UNIQUE (name),
  FOREIGN KEY (faction) REFERENCES faction(name)
);

CREATE TABLE player_ship (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  ship INTEGER NOT NULL,
  player INTEGER NOT NULL,
  name TEXT NOT NULL,
  hull_points NUMERIC(8, 2) NOT NULL,
  power_points NUMERIC(8, 2) NOT NULL,
  PRIMARY KEY (id),
  UNIQUE (ship, player),
  FOREIGN KEY (ship) REFERENCES ship(id),
  FOREIGN KEY (player) REFERENCES player(id)
);
