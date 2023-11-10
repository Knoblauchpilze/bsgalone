# bsgalone

Yet another copy of a famous game which will be probably not as polished and not as fun as the original. This time we try out: BSGO.

# Installation

## Prerequisite

This projects uses:

- [google test](https://github.com/google/googletest): installation instructions [here](https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/), a simple `sudo apt-get install libgtest-dev` should be enough.
- [cmake](https://cmake.org/): installation instructions [here](https://askubuntu.com/questions/355565/how-do-i-install-the-latest-version-of-cmake-from-the-command-line), a simple `sudo apt-get cmake` should also be enough.
- [eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page): installation instructions [here](https://www.cyberithub.com/how-to-install-eigen3-on-ubuntu-20-04-lts-focal-fossa/) for Ubuntu 20.04, a simple `sudo apt install libeigen3-dev` should be enough.
- [libpqxx](https://github.com/jtv/libpqxx) for db connection. See installation instructions [here](https://github.com/jtv/libpqxx/blob/master/BUILDING-cmake.md#option-b-make-use-of-a-separately-installed-libpqxx) in the README of the github repository.
- [golang migrate](https://github.com/golang-migrate/migrate/blob/master/cmd/migrate/README.md): following the instructions there should be enough.

## Instructions

- Clone the repo: `git clone git@github.com:Knoblauchpilze/pge-app.git`.
- Clone dependencies:
  - [core_utils](https://github.com/Knoblauchpilze/core_utils)
  - [maths_utils](https://github.com/Knoblauchpilze/maths_utils)
- Go to the project's directory `cd ~/path/to/the/repo`.
- Compile: `make run`.

Don't forget to add `/usr/local/lib` to your `LD_LIBRARY_PATH` to be able to load shared libraries at runtime. This is handled automatically when using the `make run` target (which internally uses the [run.sh](data/run.sh) script).

### libpqxx

The README in the repo is not exactly working. You need to install `libpq-dev`: while not indicated it is clear from the build process.

Then the commands are not as described [here](https://github.com/jtv/libpqxx/blob/master/BUILDING-cmake.md#building-and-installing-libpqxx-yourself) but rather:
```bash
mkdir build
cd build
cmake ..
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=1 ..
sudo cmake --install .
```

Those were taking from [this](https://preshing.com/20170511/how-to-build-a-cmake-based-project/#running-cmake-from-the-command-line) tutorial and [this](https://stackoverflow.com/questions/67425557/how-do-i-build-a-cmake-project) StackOverflow post. By default libpqxx generates a static library which is not really suited for our purposes. The option `BUILD_SHARED_LIBS` forces to create them. This was indicated in the [README](https://github.com/jtv/libpqxx/blob/master/BUILDING-cmake.md#cheat-sheet).

Then we can attach the library as a dependency of the project as described in the rest of the install [guide](https://github.com/jtv/libpqxx/blob/master/BUILDING-cmake.md#option-b-make-use-of-a-separately-installed-libpqxx).

# Setting up the DB

## Install postgresql

To do this, install postgresql through the packages with:
```bash
sudo apt-get install postgresql-14
```

## How to send commands to the postgres server

In order to create the database and then perform the migration, we need to at least use the default `postgres` user to create a new user which can then perform all the rest.

On first inspection it seems like to do that we need to authenticate with the user. The first thing which was not working as easily as we thought is that running the default `psql` command is trying to connect using Unix sockets which are apparently configured to use peer authentication. This can be checked in the file `/etc/postgresql/14/main/pg_hba.conf`:

```
local   all             postgres                                peer
```

According to this [stackoverflow post](https://stackoverflow.com/questions/66915087/postgresql-psql-error-fatal-peer-authentication-failed-for-user-userrole) a way to change this behavior is either to change the `peer` value to something else or to use TCP for example by adding `-h 127.0.0.1` in the command line.

## Altering the postgres password

The second problem that arised was that once we were able to communicate with TCP, we were prompted the `postgres` user password. This is not known to us. This [stackoverflow post](https://stackoverflow.com/questions/27107557/what-is-the-default-password-for-postgres) provided useful guidance to do this.

## Creating the database

Once all of the above is done, we can successfully run the provided scripts to create a user (see [create_user.sh](database/create_user.sh)) first and then use this user to create the database with [create_database.sh](database/create_database.sh).

## Populating the database

We use `migrate` to manage the database and perform the data migrations. Once the previous step is complete (so the user and the database both exist) one can simply go to the [migrations](database/migrations) folder and run:
```bash
make migrate
```

This will **on a fresh database** trigger all the migrations in order. If the database is in an intermediate state then it will run the missing migrations.

## Managing the database

Once the database is created, one can add migrations in the folder and run again `make migrate` to apply them. If one needs to be reverted, `make demigrateO` can be used to undo the last migration while `make demigrate` will undo all the migrations and basically restore the database to its initial (empty) state.

## Drop the database

In case it is required to drop the database completely, a convenience script is available under [drop_database.sh](database/drop_database.sh). It is possible to recreate the database again by following the same procedure starting from the top [section](#creating-the-database).

## Creating migrations

According to this [stackoverflow](https://stackoverflow.com/questions/72144228/sequelize-migration-throws-error-function-uuid-generate-v4-does-not-exist) question it seems like we can directly use `gen_random_uuid` with versions of postgres more recent than 14. This is our case.

# What is the game about?

The goal is to have a 2d top view space shooter where the user can jump from systems to systems and fight enemies while mining resources and upgrading their ship.

# Useful links

About the entity component system, this [link](https://www.codingwiththomas.com/blog/an-entity-component-system-from-scratch) is useful. Also found [this](https://austinmorlan.com/posts/entity_component_system/) which seems more advanced so we might come back to it later.

The art for spaceships comes from [imgur](https://imgur.com/im8ukHF) and [deviantart](https://www.deviantart.com/ariochiv/art/Stars-in-Shadow-Phidi-Ships-378251756).

# Development process

Progressively clone bsgo

## Useful libraries

Postgresql library for cpp:
https://www.postgresql.org/docs/7.2/libpqplusplus.html

Json nlohmann:
https://github.com/nlohmann/json

ORM in cpp:
https://github.com/silverqx/TinyORM

Postgresql lib:
https://github.com/jtv/libpqxx

## Structure of the DB

### System table
Contains the system id, its name and position on the map and also maybe cap for people

### Asteroid table
Contains the position and helath points of the asteroid in a system. Foreign key on the system id

### Player table
Contains the id of the player and its name

### Ship table
Contains the definition of ships. They are just templates with the base statistics such as speed, acceleration, hull points, etc

### Player ship table
Contains the acual instantiation of the ships for each player. When a player buys a ship we create a new entry in this table from the template. It has a foreign key representing the player owning the ship

### Outpost table
Similar to the ship table in purpose but for outpost. Defines the base statistics for the outposts of each faction

### System outpost table
Similar to the player ship table in purpose. Contains a foreign key on the system and the actual instantiation of the outpost for a given system. When an outpost spawns in a system we duplicate an entry from the outpost table into this table

### Weapon table
Contains the definition of weapons in the game. Defines the statistics such as damage, reload time and power cost

### Player weapon table
Instantiation of the template of the weapons for each player. The level is added here and specific to each weapon. Each weapon has its own id. Not all weapons have an entry in the ship weapon table below

### Ship weapon table
Associates the weapon id from the table above with the player ship id. This links together the weapon and make it used by a ship. 

### Note for computers
Very similar system as for weapons: we have a template table, the instantiation per player and the association table for each ship

### Resource table
Define the list of resources available in the game with an id and a name

### Asteroid loot table
Define the list of asteroids having a loot as resource. It links an id of an asteroid (as defined in the asteroid table) with the id of a resource (taken from the resource table) and an amount. When the asteroid is destroyed we should clear this line in the DB

### Player resource table
Contains rows with the player id, a resource id and the amount. Unique on (playerId, resourceId). Each time a loot is taken, we update this value

### Locker
The locker is then just the collection of items from the player which are either resources (as resources can't be equiped) or weapons/computers/etc which have no entry in the corresponding ShipWeaponTable/ShipComputerTable/etc
