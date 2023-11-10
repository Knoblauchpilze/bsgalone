#!/bin/sh

# https://stackoverflow.com/questions/8208181/create-postgres-database-using-batch-file-with-template-encoding-owner-and
psql -h localhost -U bsgadmin postgres -f db_create.sql
