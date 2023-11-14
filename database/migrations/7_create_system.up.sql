
CREATE TABLE system (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  name TEXT NOT NULL,
  PRIMARY KEY (id),
  UNIQUE (name)
);

CREATE TABLE ship_system (
  ship INTEGER NOT NULL,
  system INTEGER NOT NULL,
  PRIMARY KEY (ship, system),
  FOREIGN KEY (ship) REFERENCES player_ship(id),
  FOREIGN KEY (system) REFERENCES system(id)
);

CREATE TABLE asteroid (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  system INTEGER NOT NULL,
  health NUMERIC(12, 2) NOT NULL,
  radius NUMERIC(12, 2) NOT NULL,
  x_pos NUMERIC(12, 2) NOT NULL,
  y_pos NUMERIC(12, 2) NOT NULL,
  z_pos NUMERIC(12, 2) NOT NULL,
  PRIMARY KEY (id),
  FOREIGN KEY (system) REFERENCES system(id)
);

CREATE TABLE asteroid_loot (
  asteroid INTEGER NOT NULL,
  resource INTEGER NOT NULL,
  amount NUMERIC(12, 2) NOT NULL,
  PRIMARY KEY (asteroid),
  FOREIGN KEY (asteroid) REFERENCES asteroid(id),
  FOREIGN KEY (resource) REFERENCES resource(id)
);

CREATE TABLE outpost (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  faction TEXT NOT NULL,
  max_hull_points NUMERIC(8, 2) NOT NULL,
  hull_points_regen NUMERIC(8, 2) NOT NULL,
  max_power_points NUMERIC(8, 2) NOT NULL,
  power_points_regen NUMERIC(8, 2) NOT NULL,
  radius NUMERIC(8, 2) NOT NULL,
  PRIMARY KEY (id),
  UNIQUE (faction),
  FOREIGN KEY (faction) REFERENCES faction(name)
);

CREATE TABLE system_outpost (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  outpost INTEGER NOT NULL,
  system INTEGER NOT NULL,
  hull_points NUMERIC(8, 2) NOT NULL,
  power_points NUMERIC(8, 2) NOT NULL,
  x_pos NUMERIC(12, 2) NOT NULL,
  y_pos NUMERIC(12, 2) NOT NULL,
  z_pos NUMERIC(12, 2) NOT NULL,
  PRIMARY KEY (id),
  UNIQUE (outpost, system),
  FOREIGN KEY (outpost) REFERENCES outpost(id),
  FOREIGN KEY (system) REFERENCES system(id)
);
