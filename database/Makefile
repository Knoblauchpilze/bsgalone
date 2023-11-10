# Default variables
DB_NAME ?= bsgdb
DB_HOST ?= localhost
DB_PORT ?= 5432
DB_USER ?= bsgadmin
DB_PASSWORD ?= Ww76hQWbbt7zi2ItM6cNo4YYT
MIGRATION_STEPS ?= 1


# Target allowing to connect to the db in the image.
connect:
	psql -U ${DB_USER} -d ${DB_NAME} -h ${DB_HOST} -p ${DB_PORT}

# Target allowing to migrate the db to latest defined step.
.PHONY: migrate
migrate:
	migrate -path ${PWD}/migrations -database postgres://${DB_USER}:${DB_PASSWORD}@${DB_HOST}:${DB_PORT}/${DB_NAME}?sslmode=disable up
	# Target allowing to migrate the db to latest defined step.

# Target allowing to migrate the db one step further.
migrateO:
	migrate -path ${PWD}/migrations -database postgres://${DB_USER}:${DB_PASSWORD}@${DB_HOST}:${DB_PORT}/${DB_NAME}?sslmode=disable up ${MIGRATION_STEPS}

# Target allowing to migrate the db to initial state.
demigrate:
	migrate -path ${PWD}/migrations -database postgres://${DB_USER}:${DB_PASSWORD}@${DB_HOST}:${DB_PORT}/${DB_NAME}?sslmode=disable down

# Demigrate only the last step of the DB creation.
demigrateO:
	migrate -path ${PWD}/migrations -database postgres://${DB_USER}:${DB_PASSWORD}@${DB_HOST}:${DB_PORT}/${DB_NAME}?sslmode=disable down ${MIGRATION_STEPS}
