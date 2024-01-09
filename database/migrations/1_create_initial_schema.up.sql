
-- Common properties of the DB
SET client_encoding = 'UTF8';

SET search_path = public, pg_catalog;
SET default_tablespace = '';

-- https://www.postgresql.org/docs/current/sql-createtrigger.html
CREATE OR REPLACE FUNCTION update_updated_at() RETURNS TRIGGER AS $$
  BEGIN
    NEW.updated_at = now();
    RETURN NEW;
  END;
$$ language 'plpgsql';
