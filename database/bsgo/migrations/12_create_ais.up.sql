
CREATE TABLE ai_targets (
  ship INTEGER NOT NULL,
  index INTEGER NOT NULL,
  x_pos NUMERIC(12, 2) NOT NULL,
  y_pos NUMERIC(12, 2) NOT NULL,
  z_pos NUMERIC(12, 2) NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP WITH TIME ZONE,
  PRIMARY KEY (ship, index),
  FOREIGN KEY (ship) REFERENCES player_ship(id)
);

CREATE TABLE ai_behaviors (
  ship INTEGER NOT NULL,
  index INTEGER NOT NULL,
  updated_at TIMESTAMP WITH TIME ZONE,
  PRIMARY KEY (ship, index),
  FOREIGN KEY (ship, index) REFERENCES ai_targets(ship, index)
);

CREATE TRIGGER trigger_ai_targets_updated_at
  BEFORE UPDATE OR INSERT ON ai_targets
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();

CREATE TRIGGER trigger_ai_behaviors_updated_at
  BEFORE UPDATE OR INSERT ON ai_behaviors
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();

