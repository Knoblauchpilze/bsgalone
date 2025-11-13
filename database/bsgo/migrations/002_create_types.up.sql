
CREATE TABLE faction (
  name TEXT NOT NULL,
  PRIMARY KEY (name)
);

CREATE TABLE entity (
  name TEXT NOT NULL,
  PRIMARY KEY (name)
);

CREATE TABLE resource (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  name TEXT NOT NULL,
  PRIMARY KEY (id),
  UNIQUE (name)
);

CREATE TABLE slot (
  type TEXT NOT NULL,
  PRIMARY KEY (type)
);
