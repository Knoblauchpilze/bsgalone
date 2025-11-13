
CREATE TABLE weapon (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  name TEXT NOT NULL,
  min_damage NUMERIC(8, 2) NOT NULL,
  max_damage NUMERIC(8, 2) NOT NULL,
  power_cost NUMERIC(8, 2) NOT NULL,
  range NUMERIC(8, 2) NOT NULL,
  reload_time NUMERIC(8, 2) NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP WITH TIME ZONE,
  PRIMARY KEY (id),
  UNIQUE (name)
);

CREATE TABLE weapon_price (
  weapon INTEGER NOT NULL,
  resource INTEGER NOT NULL,
  cost INTEGER NOT NULL,
  PRIMARY KEY (weapon, resource),
  FOREIGN KEY (weapon) REFERENCES weapon(id),
  FOREIGN KEY (resource) REFERENCES resource(id)
);

CREATE TRIGGER trigger_weapon_updated_at
  BEFORE UPDATE OR INSERT ON weapon
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();
