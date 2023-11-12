
-- https://www.postgresqltutorial.com/postgresql-tutorial/postgresql-identity-column/
CREATE TABLE player (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  name TEXT NOT NULL,
  password TEXT NOT NULL,
  PRIMARY KEY (id),
  UNIQUE (name)
);
