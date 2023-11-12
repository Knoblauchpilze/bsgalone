
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
  PRIMARY KEY (name)
);

INSERT INTO public.faction ("name") VALUES('cylon');
INSERT INTO public.faction ("name") VALUES('colonial');

INSERT INTO public.entity ("name") VALUES('ship');
INSERT INTO public.entity ("name") VALUES('asteroid');
INSERT INTO public.entity ("name") VALUES('outpost');

INSERT INTO public.resource ("name") VALUES('tylium');
INSERT INTO public.resource ("name") VALUES('titane');
