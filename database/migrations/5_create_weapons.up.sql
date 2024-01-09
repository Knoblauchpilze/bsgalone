
CREATE TABLE weapon (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  name TEXT NOT NULL,
  min_damage NUMERIC(8, 2) NOT NULL,
  max_damage NUMERIC(8, 2) NOT NULL,
  power_cost NUMERIC(8, 2) NOT NULL,
  range NUMERIC(8, 2) NOT NULL,
  reload_time_ms INTEGER NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP WITH TIME ZONE,
  PRIMARY KEY (id),
  UNIQUE (name)
);

CREATE TABLE weapon_price (
  weapon INTEGER NOT NULL,
  resource INTEGER NOT NULL,
  cost NUMERIC(8, 2) NOT NULL,
  PRIMARY KEY (weapon, resource),
  FOREIGN KEY (weapon) REFERENCES weapon(id),
  FOREIGN KEY (resource) REFERENCES resource(id)
);

CREATE TABLE player_weapon (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  weapon INTEGER NOT NULL,
  player INTEGER,
  level INTEGER NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP WITH TIME ZONE,
  PRIMARY KEY (id),
  FOREIGN KEY (weapon) REFERENCES weapon(id),
  FOREIGN KEY (player) REFERENCES player(id)
);

CREATE TABLE ship_weapon (
  ship INTEGER NOT NULL,
  weapon INTEGER NOT NULL,
  slot INTEGER NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP WITH TIME ZONE,
  PRIMARY KEY (ship, weapon),
  UNIQUE (ship, slot),
  FOREIGN KEY (ship) REFERENCES player_ship(id),
  FOREIGN KEY (weapon) REFERENCES player_weapon(id),
  FOREIGN KEY (slot) REFERENCES ship_slot(id)
);

CREATE TRIGGER trigger_weapon_updated_at
  BEFORE UPDATE OR INSERT ON weapon
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();

CREATE TRIGGER trigger_player_weapon_updated_at
  BEFORE UPDATE OR INSERT ON player_weapon
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();

CREATE TRIGGER trigger_ship_weapon_updated_at
  BEFORE UPDATE OR INSERT ON ship_weapon
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();
