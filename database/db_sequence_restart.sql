
-- https://stackoverflow.com/questions/41102908/how-to-reset-all-sequences-to-1-before-database-migration-in-postgresql
SELECT  SETVAL(c.oid, 1)
  FROM pg_class c JOIN pg_namespace n
  ON n.oid = c.relnamespace
  WHERE c.relkind = 'S' AND n.nspname = 'public';