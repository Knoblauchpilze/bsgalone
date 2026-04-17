
CREATE TABLE outpost (
  id UUID NOT NULL,
  faction TEXT NOT NULL,
  max_hull_points NUMERIC(8, 2) NOT NULL,
  hull_points_regen NUMERIC(8, 2) NOT NULL,
  max_power_points NUMERIC(8, 2) NOT NULL,
  power_points_regen NUMERIC(8, 2) NOT NULL,
  radius NUMERIC(8, 2) NOT NULL,
  PRIMARY KEY (id),
  UNIQUE (faction),
  FOREIGN KEY (faction) REFERENCES faction(name)
);
