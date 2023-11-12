
CREATE TABLE computer (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  name TEXT NOT NULL,
  offensive BOOLEAN NOT NULL,
  power_cost NUMERIC(8, 2) NOT NULL,
  reload_time_ms INTEGER NOT NULL,
  range NUMERIC(8, 2) DEFAULT NULL,
  duration_ms INTEGER DEFAULT NULL,
  damage_modifier NUMERIC(8, 2) DEFAULT NULL,
  PRIMARY KEY (id),
  UNIQUE (name)
);

CREATE TABLE computer_price (
  computer INTEGER NOT NULL,
  resource INTEGER NOT NULL,
  cost NUMERIC(8, 2) NOT NULL,
  PRIMARY KEY (computer, resource),
  FOREIGN KEY (computer) REFERENCES computer(id),
  FOREIGN KEY (resource) REFERENCES resource(id)
);

CREATE TABLE computer_allowed_target (
  computer INTEGER NOT NULL,
  entity TEXT NOT NULL,
  PRIMARY KEY (computer, entity),
  FOREIGN KEY (computer) REFERENCES computer(id),
  FOREIGN KEY (entity) REFERENCES entity(name)
);

CREATE TABLE player_computer (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  computer INTEGER NOT NULL,
  player INTEGER NOT NULL,
  level INTEGER NOT NULL,
  PRIMARY KEY (id),
  FOREIGN KEY (computer) REFERENCES computer(id),
  FOREIGN KEY (player) REFERENCES player(id)
);

CREATE TABLE ship_computer (
  ship INTEGER NOT NULL,
  computer INTEGER NOT NULL,
  PRIMARY KEY (ship, computer),
  FOREIGN KEY (ship) REFERENCES player_ship(id),
  FOREIGN KEY (computer) REFERENCES player_computer(id)
);