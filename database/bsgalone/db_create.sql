-- https://dba.stackexchange.com/questions/117109/how-to-manage-default-privileges-for-users-on-a-database-vs-schema/117661#117661
CREATE DATABASE db_bsgalone OWNER bsgalone_admin;
REVOKE ALL ON DATABASE db_bsgalone FROM public;

GRANT CONNECT ON DATABASE db_bsgalone TO bsgalone_user;

\connect db_bsgalone

CREATE SCHEMA bsgalone_schema AUTHORIZATION bsgalone_admin;

SET search_path = bsgalone_schema;

ALTER ROLE bsgalone_admin IN DATABASE db_bsgalone SET search_path = bsgalone_schema;
ALTER ROLE bsgalone_manager IN DATABASE db_bsgalone SET search_path = bsgalone_schema;
ALTER ROLE bsgalone_user IN DATABASE db_bsgalone SET search_path = bsgalone_schema;

GRANT USAGE  ON SCHEMA bsgalone_schema TO bsgalone_user;
GRANT CREATE ON SCHEMA bsgalone_schema TO bsgalone_admin;

ALTER DEFAULT PRIVILEGES FOR ROLE bsgalone_admin
GRANT SELECT ON TABLES TO bsgalone_user;

ALTER DEFAULT PRIVILEGES FOR ROLE bsgalone_admin
GRANT INSERT, UPDATE, DELETE ON TABLES TO bsgalone_manager;
