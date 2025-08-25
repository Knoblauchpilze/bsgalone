
INSERT INTO faction ("name") VALUES ('cylon');
INSERT INTO faction ("name") VALUES ('colonial');

INSERT INTO entity ("name") VALUES ('ship');
INSERT INTO entity ("name") VALUES ('asteroid');
INSERT INTO entity ("name") VALUES ('outpost');

INSERT INTO resource ("name") VALUES ('tylium');
INSERT INTO resource ("name") VALUES ('titane');

INSERT INTO slot ("type") VALUES ('weapon');
INSERT INTO slot ("type") VALUES ('computer');

INSERT INTO ship_class ("name", "jump_time_ms", "jump_time_threat_ms")
  VALUES ('strike', 5000, 60000);
INSERT INTO ship_class ("name", "jump_time_ms", "jump_time_threat_ms")
  VALUES ('line', 25000, 100000);