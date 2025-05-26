
-- https://stackoverflow.com/questions/72985242/securely-create-role-in-postgres-using-sql-script-and-environment-variables
CREATE USER bsgalone_admin WITH CREATEDB PASSWORD :'admin_password';
CREATE USER bsgalone_manager WITH PASSWORD :'manager_password';
CREATE USER bsgalone_user WITH PASSWORD :'user_password';

GRANT bsgalone_user TO bsgalone_manager;
GRANT bsgalone_manager TO bsgalone_admin;
