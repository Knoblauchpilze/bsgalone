
CREATE TABLE faction (
  name TEXT NOT NULL,
  PRIMARY KEY (name)
);

CREATE TABLE entity (
  name TEXT NOT NULL,
  PRIMARY KEY (name)
);

CREATE TABLE resource (
  id INTEGER GENERATED ALWAYS AS IDENTITY,
  name TEXT NOT NULL,
  PRIMARY KEY (id),
  UNIQUE (name)
);

CREATE TABLE slot (
  type TEXT NOT NULL,
  PRIMARY KEY (type)
);

INSERT INTO faction ("name") VALUES ('cylon');
INSERT INTO faction ("name") VALUES ('colonial');

INSERT INTO entity ("name") VALUES ('ship');
INSERT INTO entity ("name") VALUES ('asteroid');
INSERT INTO entity ("name") VALUES ('outpost');

INSERT INTO resource ("name") VALUES ('tylium');
INSERT INTO resource ("name") VALUES ('titane');

INSERT INTO slot ("type") VALUES ('weapon');
INSERT INTO slot ("type") VALUES ('computer');
