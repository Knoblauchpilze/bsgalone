
CREATE TABLE asteroid_respawn (
  asteroid INTEGER NOT NULL,
  died_at INTEGER NOT NULL,
  respawn_at INTEGER NOT NULL,
  PRIMARY KEY (asteroid),
  FOREIGN KEY (asteroid) REFERENCES asteroid(id)
);

CREATE TABLE system_outpost_respawn (
  outpost INTEGER NOT NULL,
  died_at INTEGER NOT NULL,
  respawn_at INTEGER NOT NULL,
  PRIMARY KEY (outpost),
  FOREIGN KEY (outpost) REFERENCES system_outpost(id)
);
