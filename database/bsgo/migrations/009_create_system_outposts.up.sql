
CREATE TABLE system_outpost (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  outpost INTEGER NOT NULL,
  system INTEGER NOT NULL,
  hull_points NUMERIC(8, 2) NOT NULL,
  power_points NUMERIC(8, 2) NOT NULL,
  x_pos NUMERIC(12, 2) NOT NULL,
  y_pos NUMERIC(12, 2) NOT NULL,
  z_pos NUMERIC(12, 2) NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP WITH TIME ZONE,
  PRIMARY KEY (id),
  UNIQUE (outpost, system),
  FOREIGN KEY (outpost) REFERENCES outpost(id),
  FOREIGN KEY (system) REFERENCES system(id)
);

CREATE TRIGGER trigger_system_outpost_updated_at
  BEFORE UPDATE OR INSERT ON system_outpost
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();
