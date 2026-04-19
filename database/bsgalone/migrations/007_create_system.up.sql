
CREATE TABLE system (
  id UUID NOT NULL,
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
  system UUID NOT NULL,
  faction TEXT NOT NULL,
  PRIMARY KEY (system, faction),
  FOREIGN KEY (system) REFERENCES system(id),
  FOREIGN KEY (faction) REFERENCES faction(name)
);

CREATE TABLE asteroid (
  id UUID NOT NULL,
  system UUID NOT NULL,
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
  asteroid UUID NOT NULL,
  resource UUID NOT NULL,
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
