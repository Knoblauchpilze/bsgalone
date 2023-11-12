
-- https://www.postgresqltutorial.com/postgresql-tutorial/postgresql-identity-column/
CREATE TABLE player (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  name TEXT NOT NULL,
  password TEXT NOT NULL,
  PRIMARY KEY (id),
  UNIQUE (name)
);

CREATE TABLE player_resource (
  player INTEGER NOT NULL,
  resource TEXT NOT NULL,
  amount NUMERIC(12, 2) NOT NULL,
  PRIMARY KEY (resource),
  UNIQUE (player, resource),
  FOREIGN KEY (player) REFERENCES player(id),
  FOREIGN KEY (resource) REFERENCES resource(name)
);