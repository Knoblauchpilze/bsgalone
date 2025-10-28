
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

CREATE TRIGGER trigger_ai_targets_updated_at
  BEFORE UPDATE OR INSERT ON ai_targets
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();

