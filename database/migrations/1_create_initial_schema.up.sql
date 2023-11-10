
-- Common properties of the DB
SET client_encoding = 'UTF8';

SET search_path = public, pg_catalog;
SET default_tablespace = '';

-- Register a function to automatically update the `created_at` field
-- of a new object to insert in the database.
CREATE OR REPLACE FUNCTION update_created_at() RETURNS TRIGGER AS $$
  BEGIN
    NEW.created_at = now();
    RETURN NEW;
  END;
$$ language 'plpgsql';
