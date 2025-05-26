# Default variables
DB_PATH ?= bsgo
DB_HOST ?= localhost
DB_PORT ?= 5432
DB_NAME ?= db_bsgalone
DB_USER ?= bsgalone_admin
DB_PASSWORD ?= admin_password

MIGRATION_STEPS ?= 1

URL_ENCODED_PASSWORD=$(shell urlencode '${DB_PASSWORD}')

# https://stackoverflow.com/questions/6405127/how-do-i-specify-a-password-to-psql-non-interactively
connect:
	psql postgres://${DB_USER}:${URL_ENCODED_PASSWORD}@${DB_HOST}:${DB_PORT}/${DB_NAME}

.PHONY: migrate
migrate:
	migrate -path ${PWD}/${DB_PATH}/migrations -database postgres://${DB_USER}:${URL_ENCODED_PASSWORD}@${DB_HOST}:${DB_PORT}/${DB_NAME} up

migrateOne:
	migrate -path ${PWD}/${DB_PATH}/migrations -database postgres://${DB_USER}:${URL_ENCODED_PASSWORD}@${DB_HOST}:${DB_PORT}/${DB_NAME}?sslmode=disable up ${MIGRATION_STEPS}

demigrate:
	migrate -path ${PWD}/${DB_PATH}/migrations -database postgres://${DB_USER}:${URL_ENCODED_PASSWORD}@${DB_HOST}:${DB_PORT}/${DB_NAME}?sslmode=disable down

demigrateOne:
	migrate -path ${PWD}/${DB_PATH}/migrations -database postgres://${DB_USER}:${URL_ENCODED_PASSWORD}@${DB_HOST}:${DB_PORT}/${DB_NAME}?sslmode=disable down ${MIGRATION_STEPS}
