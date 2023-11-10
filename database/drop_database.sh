#!/bin/sh

psql -h localhost -U bsgadmin postgres -f db_drop.sql
