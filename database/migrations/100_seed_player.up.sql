
INSERT INTO public.player ("name", "password")
  VALUES ('grouton', '123456');
INSERT INTO public.player ("name", "password")
  VALUES ('knoppgrunt', '654321');

INSERT INTO public.player_resource ("player", "resource", "amount")
  VALUES (
    (SELECT id FROM player WHERE name = 'grouton'),
    (SELECT id FROM resource WHERE name = 'tylium'),
    10501.2
  );
INSERT INTO public.player_resource ("player", "resource", "amount")
  VALUES (
    (SELECT id FROM player WHERE name = 'grouton'),
    (SELECT id FROM resource WHERE name = 'titane'),
    1017.2
  );
