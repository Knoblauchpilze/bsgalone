#!/bin/sh

# https://stackoverflow.com/questions/8208181/create-postgres-database-using-batch-file-with-template-encoding-owner-and
psql -h 127.0.0.1 -U postgres postgres -f db_user_create.sql
