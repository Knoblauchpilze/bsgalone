#!/bin/sh

psql -h localhost -U bsgadmin postgres -f db_sequence_restart.sql
