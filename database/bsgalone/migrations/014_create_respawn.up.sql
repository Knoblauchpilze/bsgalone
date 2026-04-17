
CREATE TABLE asteroid_respawn (
  asteroid UUID NOT NULL,
  died_at INTEGER NOT NULL,
  respawn_at INTEGER NOT NULL,
  PRIMARY KEY (asteroid),
  FOREIGN KEY (asteroid) REFERENCES asteroid(id)
);

CREATE TABLE player_ship_respawn (
  ship UUID NOT NULL,
  died_at INTEGER NOT NULL,
  respawn_at INTEGER NOT NULL,
  PRIMARY KEY (ship),
  FOREIGN KEY (ship) REFERENCES player_ship(id)
);
