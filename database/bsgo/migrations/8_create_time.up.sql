
CREATE TABLE tick_config (
  system INTEGER NOT NULL,
  unit TEXT NOT NULL,
  ticks INTEGER NOT NULL,
  updated_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (system),
  FOREIGN KEY (system) REFERENCES system(id)
);

CREATE TABLE tick (
  system INTEGER NOT NULL,
  current_tick INTEGER NOT NULL,
  updated_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (system),
  FOREIGN KEY (system) REFERENCES system(id)
);

CREATE TRIGGER trigger_tick_config_updated_at
  BEFORE UPDATE OR INSERT ON tick_config
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();

CREATE TRIGGER trigger_tick_updated_at
  BEFORE UPDATE OR INSERT ON tick
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();
