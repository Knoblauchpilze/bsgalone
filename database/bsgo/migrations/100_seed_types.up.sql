
INSERT INTO faction ("name") VALUES ('cylon');
INSERT INTO faction ("name") VALUES ('colonial');

INSERT INTO entity ("name") VALUES ('ship');
INSERT INTO entity ("name") VALUES ('asteroid');
INSERT INTO entity ("name") VALUES ('outpost');

INSERT INTO resource ("name") VALUES ('tylium');
INSERT INTO resource ("name") VALUES ('titane');

INSERT INTO slot ("type") VALUES ('weapon');
INSERT INTO slot ("type") VALUES ('computer');

INSERT INTO ship_class ("name", "jump_time", "jump_time_threat")
  VALUES ('strike', 5.0, 60.0);
INSERT INTO ship_class ("name", "jump_time", "jump_time_threat")
  VALUES ('line', 25.0, 100.0);
