
CREATE TABLE player_ship (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  ship INTEGER NOT NULL,
  player INTEGER NOT NULL,
  name TEXT NOT NULL,
  active BOOLEAN NOT NULL,
  hull_points NUMERIC(8, 2) NOT NULL,
  power_points NUMERIC(8, 2) NOT NULL,
  x_pos NUMERIC(12, 2) NOT NULL,
  y_pos NUMERIC(12, 2) NOT NULL,
  z_pos NUMERIC(12, 2) NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP WITH TIME ZONE,
  PRIMARY KEY (id),
  UNIQUE (ship, player),
  FOREIGN KEY (ship) REFERENCES ship(id),
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

CREATE TABLE ship_computer (
  ship INTEGER NOT NULL,
  computer INTEGER NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP WITH TIME ZONE,
  PRIMARY KEY (ship, computer),
  FOREIGN KEY (ship) REFERENCES player_ship(id),
  FOREIGN KEY (computer) REFERENCES player_computer(id)
);

CREATE TABLE ship_system (
  ship INTEGER NOT NULL,
  system INTEGER NOT NULL,
  docked BOOLEAN NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP WITH TIME ZONE,
  PRIMARY KEY (ship),
  FOREIGN KEY (ship) REFERENCES player_ship(id),
  FOREIGN KEY (system) REFERENCES system(id)
);

CREATE TABLE ship_jump (
  ship INTEGER NOT NULL,
  system INTEGER NOT NULL,
  PRIMARY KEY (ship, system),
  FOREIGN KEY (ship) REFERENCES player_ship(id),
  FOREIGN KEY (system) REFERENCES system(id)
);

CREATE TABLE player_role (
  player INTEGER NOT NULL,
  role TEXT NOT NULL,
  target_ship INTEGER DEFAULT NULL,
  FOREIGN KEY (player) REFERENCES player(id),
  FOREIGN KEY (role) REFERENCES game_role(name),
  FOREIGN KEY (target_ship) REFERENCES player_ship,
  UNIQUE (player)
);

CREATE TRIGGER trigger_player_ship_updated_at
  BEFORE UPDATE OR INSERT ON player_ship
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();

CREATE TRIGGER trigger_ship_weapon_updated_at
  BEFORE UPDATE OR INSERT ON ship_weapon
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();

CREATE TRIGGER trigger_ship_computer_updated_at
  BEFORE UPDATE OR INSERT ON ship_computer
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();

CREATE TRIGGER trigger_ship_system_updated_at
  BEFORE UPDATE OR INSERT ON ship_system
  FOR EACH ROW
  EXECUTE FUNCTION update_updated_at();
