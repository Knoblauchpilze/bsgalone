
INSERT INTO public.player ("name", "password", "faction")
  VALUES ('colo', 'aze', 'colonial');
INSERT INTO public.player ("name", "password", "faction")
  VALUES ('colo2', 'aze', 'colonial');
INSERT INTO public.player ("name", "password", "faction")
  VALUES ('toast', 'aze', 'cylon');

INSERT INTO public.player_resource ("player", "resource", "amount")
  VALUES (
    (SELECT id FROM player WHERE name = 'colo'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    100501.2
  );
INSERT INTO public.player_resource ("player", "resource", "amount")
  VALUES (
    (SELECT id FROM player WHERE name = 'colo'),
    (SELECT id FROM resource WHERE name = 'titane'),
    1017.2
  );

INSERT INTO public.player_resource ("player", "resource", "amount")
  VALUES (
    (SELECT id FROM player WHERE name = 'colo2'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    4567.0
  );

INSERT INTO public.player_resource ("player", "resource", "amount")
  VALUES (
    (SELECT id FROM player WHERE name = 'toast'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    1234.2
  );
INSERT INTO public.player_resource ("player", "resource", "amount")
  VALUES (
    (SELECT id FROM player WHERE name = 'toast'),
    (SELECT id FROM resource WHERE name = 'titane'),
    56789.2
  );
