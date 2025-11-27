# Default variables
DB_PATH ?= bsgo
DB_HOST ?= localhost
DB_PORT ?= 5432
DB_NAME ?= db_bsgalone
DB_USER ?= bsgalone_admin
DB_PASSWORD ?= admin_password

MIGRATION_STEPS ?= 1

URL_ENCODED_PASSWORD=$(shell urlencode '${DB_PASSWORD}')

# This target can be used to print commands to copy to set the environment
# variables needed for passwords when creating users. This makes it faster
# than having to type them by manually.
printPasswords:
	@echo "export ADMIN_PASSWORD=admin_password"
	@echo "export MANAGER_PASSWORD=manager_password"
	@echo "export USER_PASSWORD=user_password"

# https://stackoverflow.com/questions/6405127/how-do-i-specify-a-password-to-psql-non-interactively
connect:
	psql postgres://${DB_USER}:${URL_ENCODED_PASSWORD}@${DB_HOST}:${DB_PORT}/${DB_NAME}

.PHONY: migrate
migrate:
	migrate -path ${PWD}/${DB_PATH}/migrations -database postgres://${DB_USER}:${URL_ENCODED_PASSWORD}@${DB_HOST}:${DB_PORT}/${DB_NAME}?sslmode=disable up

migrateOne:
	migrate -path ${PWD}/${DB_PATH}/migrations -database postgres://${DB_USER}:${URL_ENCODED_PASSWORD}@${DB_HOST}:${DB_PORT}/${DB_NAME}?sslmode=disable up ${MIGRATION_STEPS}

demigrate:
	migrate -path ${PWD}/${DB_PATH}/migrations -database postgres://${DB_USER}:${URL_ENCODED_PASSWORD}@${DB_HOST}:${DB_PORT}/${DB_NAME}?sslmode=disable down

demigrateOne:
	migrate -path ${PWD}/${DB_PATH}/migrations -database postgres://${DB_USER}:${URL_ENCODED_PASSWORD}@${DB_HOST}:${DB_PORT}/${DB_NAME}?sslmode=disable down ${MIGRATION_STEPS}
