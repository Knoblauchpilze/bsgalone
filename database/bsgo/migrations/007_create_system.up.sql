
CREATE TABLE system (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  name TEXT NOT NULL,
  x_pos NUMERIC(6, 2) NOT NULL,
  y_pos NUMERIC(6, 2) NOT NULL,
  z_pos NUMERIC(6, 2) NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP WITH TIME ZONE,
  PRIMARY KEY (id),
  UNIQUE (name)
);

CREATE TABLE starting_system (
  system INTEGER NOT NULL,
  faction TEXT NOT NULL,
  PRIMARY KEY (system, faction),
  FOREIGN KEY (system) REFERENCES system(id),
  FOREIGN KEY (faction) REFERENCES faction(name)
);

CREATE TABLE asteroid (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  system INTEGER NOT NULL,
  max_health NUMERIC(8, 2) NOT NULL,
  health NUMERIC(12, 2) NOT NULL,
  radius NUMERIC(12, 2) NOT NULL,
  x_pos NUMERIC(12, 2) NOT NULL,
  y_pos NUMERIC(12, 2) NOT NULL,
  z_pos NUMERIC(12, 2) NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP WITH TIME ZONE,
  PRIMARY KEY (id),
  FOREIGN KEY (system) REFERENCES system(id)
);

CREATE TABLE asteroid_loot (
  asteroid INTEGER NOT NULL,
  resource INTEGER NOT NULL,
  amount INTEGER NOT NULL,
  PRIMARY KEY (asteroid),
  FOREIGN KEY (asteroid) REFERENCES asteroid(id),
  FOREIGN KEY (resource) REFERENCES resource(id)
);

CREATE TRIGGER trigger_system_updated_at
  BEFORE UPDATE OR INSERT ON system
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();

CREATE TRIGGER trigger_asteroid_updated_at
  BEFORE UPDATE OR INSERT ON asteroid
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();
