
INSERT INTO public.player ("name", "password", "faction")
  VALUES ('grouton', '123456', 'cylon');
INSERT INTO public.player ("name", "password", "faction")
  VALUES ('knoppgrunt', '654321', 'colonial');

INSERT INTO public.player_resource ("player", "resource", "amount")
  VALUES (
    (SELECT id FROM player WHERE name = 'grouton'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    100501.2
  );
INSERT INTO public.player_resource ("player", "resource", "amount")
  VALUES (
    (SELECT id FROM player WHERE name = 'grouton'),
    (SELECT id FROM resource WHERE name = 'titane'),
    1017.2
  );
