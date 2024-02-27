# bsgalone

Yet another copy of a famous game which will be probably not as polished and not as fun as the original. This time we try out: BSGO.

# What is the game about?

The goal is to have a 2d top view space shooter where the user can travel from systems to systems and fight enemies while mining resources and upgrading their ship.

# Installation

## Prerequisite

This projects uses:

- [google test](https://github.com/google/googletest): installation instructions [here](https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/), a simple `sudo apt-get install libgtest-dev` should be enough.
- [cmake](https://cmake.org/): installation instructions [here](https://askubuntu.com/questions/355565/how-do-i-install-the-latest-version-of-cmake-from-the-command-line), a simple `sudo apt-get cmake` should also be enough.
- [eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page): installation instructions [here](https://www.cyberithub.com/how-to-install-eigen3-on-ubuntu-20-04-lts-focal-fossa/) for Ubuntu 20.04, a simple `sudo apt install libeigen3-dev` should be enough.
- [libpqxx](https://github.com/jtv/libpqxx) for db connection. See installation instructions in the [dedicated section](#libpqxx) of this README.
- [golang migrate](https://github.com/golang-migrate/migrate/blob/master/cmd/migrate/README.md): following the instructions there should be enough.
- postgresql which can be taken from the packages with `sudo apt-get install postgresql-14` for example.
- [asio](https://think-async.com/Asio/) for networking. See installation instructions in the [dedicated section](#asio) of this README.

## Instructions

- Clone the repo: `git clone git@github.com:Knoblauchpilze/pge-app.git`.
- Clone dependencies:
  - [core_utils](https://github.com/Knoblauchpilze/core_utils)
  - [maths_utils](https://github.com/Knoblauchpilze/maths_utils)
- Go to the project's directory `cd ~/path/to/the/repo`.
- Compile: `make run`.

Don't forget to add `/usr/local/lib` to your `LD_LIBRARY_PATH` to be able to load shared libraries at runtime. This is handled automatically when using the `make run` target (which internally uses the [run.sh](data/run.sh) script).

### libpqxx

The [README](https://github.com/jtv/libpqxx/blob/master/BUILDING-cmake.md) in the repo is not exactly working. You need to install `libpq-dev`: while not indicated it is clear from the build process.

Then the commands are not as described [here](https://github.com/jtv/libpqxx/blob/master/BUILDING-cmake.md#building-and-installing-libpqxx-yourself) but rather:
```bash
mkdir build
cd build
cmake -DCMAKE_CXX_STANDARD=20 -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=1 ..
make -j 8
sudo cmake --install .
```

Those were taken from [this](https://preshing.com/20170511/how-to-build-a-cmake-based-project/#running-cmake-from-the-command-line) tutorial and [this](https://stackoverflow.com/questions/67425557/how-do-i-build-a-cmake-project) Stack Overflow post. By default libpqxx generates a static library which is not really suited for our purposes. The option `BUILD_SHARED_LIBS` forces to create them. This was indicated in the [README](https://github.com/jtv/libpqxx/blob/master/BUILDING-cmake.md#cheat-sheet). Also the c++20 standard comes from [this issue](https://github.com/jtv/libpqxx/issues/732) as we encountered the same problem when building with the debug version.

Then we can attach the library as a dependency of the project as described in the rest of the install [guide](https://github.com/jtv/libpqxx/blob/master/BUILDING-cmake.md#option-b-make-use-of-a-separately-installed-libpqxx).

### asio

#### Install on the system

This [Stack Overflow topic](https://stackoverflow.com/questions/70848366/unable-to-move-asiosslstream-to-handler) seems to indicate that there's a package for it under:
```bash
apt-get install libasio-dev
```

However this seems to be a relatively old version (1.18, from August 2020). Instead, we decided to download the newest revision from the [official website](https://think-async.com/Asio/). Click on the download link from the [download page](https://think-async.com/Asio/Download.html).

The download process gives a `tar.bz2` file which can be extracted with (change the version as needed):
```bash
tar -xvjf asio-1.28.0.tar.bz2
```

**Note:** we used version `1.28` for the development.

Asio is a header only library so we then just need to copy this to `usr/local/include`:
```bash
sudo mkdir /usr/local/include/asio
sudo cp -r asio-1.28.0/include/* /usr/local/include/asio
```

You can remove the `Makefile.am` and `Makefile.in` files from this directory to not pollute the includes.

#### Use the library in the project

After this we need to instruct cmake to find the library. This shouldn't be an issue as it is now installed on the system directories. However we faced another problem. When looking at `asio.hpp` the includes look fine:

![Asio include ok](resources/asio_include_ok.png)

But when going into for example `any_completion_executor.hpp` we see this:

![Asio include ko](resources/asio_include_ko.png)

This is a problem as relatively to the `any_completion_executor.hpp` the config file is in the same directory. So it seems the library expects to have the `/usr/local/include` directory as a `-I` directive to correctly be set up.

Looking at a few topics on the internet it seems like this is usually handled by putting the library directly in the project's folder, or by adding some include pathes in the CMake files (see [1](https://stackoverflow.com/questions/60592615/how-would-i-include-asio-library-using-cmake) or [2](https://stackoverflow.com/questions/53945148/how-to-use-the-c-standalone-asio-library)). Another way is to write manually a `FindAsio.cmake` file to find the corresponding package and set the include path automatically.

We also used this approach in the [CMakeLists.txt](src/net/CMakeLists.txt) of the net library for example. This is sufficient to allow development, we might revisit this later on.

### PixelGameEngine

The application is built around the [PixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine). We found it is a reliable engine to create 2D applications and display some graphics. However, there are a couple of compilation problems when we add the typical options we use (such as `-Wall` and `-Werror`) due to the `#ifdef` directives to support multiple platforms.

We isolated the `PixelGameEngine` into its own library in [src/pge](src/pge) and abstracted the use of the graphic resources and rendering elements into it. This is not 100% enough as we still get some errors when compiling the library. We allowed unknown pragmas and fixed the unused parameters manually in [fa73574](https://github.com/Knoblauchpilze/bsgalone/commit/fa73574d0cd47cea663305058f257abad7e1ce7e).

Additionally the application does not define a very nice icon nor tooltip. This was changed directly in the [PixelGameEngine](src/pge/olc/olcPixelGameEngine.h) file in [28590d1](https://github.com/Knoblauchpilze/bsgalone/commit/28590d182a35e3977f3221c7991a6be6b2169241).

In case of a future update we can port those changes or adapt them.

# Setting up the DB

## Install postgresql

To do this, install postgresql through the packages with:
```bash
sudo apt-get install postgresql-14
```

## How to send commands to the postgres server

In order to create the database and then perform the migration, we need to use at least once the default `postgres` user to create a new one. This new user will then be used to perform all the rest.

On first inspection it seems like to do that we need to authenticate with the `postgres` user. The first thing which was not working as easily as we thought is that running the default `psql` command is trying to connect using Unix sockets which are apparently configured to use peer authentication. This can be checked in the file `/etc/postgresql/14/main/pg_hba.conf`:

```
local   all             postgres                                peer
```

According to this [Stack Overflow post](https://stackoverflow.com/questions/66915087/postgresql-psql-error-fatal-peer-authentication-failed-for-user-userrole) a way to change this behavior is either to change the `peer` value to something else or to use TCP for example by adding `-h 127.0.0.1` or `-h localhost` as an option of the command line.

## Altering the postgres password

The second problem that arised was that once we were able to communicate with TCP, we were prompted the `postgres` user password. This is not known to us. This [Stack Overflow post](https://stackoverflow.com/questions/27107557/what-is-the-default-password-for-postgres) provided useful guidance to change it to something we know.

## Creating the database

Once all of the above is done, we can successfully run the provided scripts to create a user (see [create_user.sh](database/create_user.sh)) first and then use this user to create the database with [create_database.sh](database/create_database.sh).

## Populating the database

We use `migrate` to manage the database and perform the data migrations. Once the previous step is complete (so the user and the database both exist) one can simply go to the [migrations](database/migrations) folder and run:
```bash
make migrate
```

**On a fresh database**, this will trigger all the migrations in order. If the database is in an intermediate state then it will only run the missing migrations.

## Managing the database

Once the database is created, it is easy to add migration files in the folder and run again `make migrate` to apply them. If one of them needs to be reverted, `make demigrateO` can be used to undo the last migration while `make demigrate` will undo all the migrations and basically restore the database to its initial (empty) state.

## Drop the database

In case it is required to drop the database completely, a convenience script is available under [drop_database.sh](database/drop_database.sh). It is possible to recreate the database again by following the same procedure starting from the top [section](#creating-the-database).

## Creating migrations and generating uuids

According to this [Stack Overflow](https://stackoverflow.com/questions/72144228/sequelize-migration-throws-error-function-uuid-generate-v4-does-not-exist) question it seems like we can directly use `gen_random_uuid` with versions of postgres more recent than 14. This is our case.

Another approach as we need to deserialize the data from the database is to use plain integers for identifiers. This is probably enough for now. To this end it seems like the way to go about it is to use `generated always as identity` (taken from this [Stack Overflow](https://stackoverflow.com/questions/55300370/postgresql-serial-vs-identity) post).

## Allowing clients to connect to the database

As of now each client will connect to the database and try to get information from it. This is mainly the case when processing messages and initializing the UI.

By default postgres does not allow incoming connections from remote host to come through. In order to allow the host machine to accept those we need to modify the configuration of the postgres a bit.

Once again we found a nice [Stack Overflow](https://stackoverflow.com/questions/17838613/open-port-in-ubuntu) post explaining what needs to be done. The steps are presented in the following sections.

### Edit: postgresql.conf

Edit the file:
```bash
sudo nano /etc/postgresql/<version>/main/postgresql.conf
```

And enable or add:
```
listen_addresses = '*'
```

### Edit pg_hba.conf

Edit the file:
```bash
sudo nano /etc/postgresql/<version>/main/pg_hba.conf
```

Register the network mask you want to allow connections from. A `0` stands for a wildcard and we chose to restrict to local network for now by adding the following:
```
host    all             all             192.168.1.0/0          scram-sha-256
```

The connection method was changed from `md5` to `scram-sha-256` as recommended by this [DBA stackexchange](https://dba.stackexchange.com/questions/83984/connect-to-postgresql-server-fatal-no-pg-hba-conf-entry-for-host) post: it is supposed to be more secured than the old `md5` approach.

With this we were able to connect from a remote computer to the database hosted on another machine.

# Development log and findings

## Goal

The goal of the game is to try to mimic some aspects of [Battlestar Galactica Online](https://www.mmorpg.com/battlestar-galactica-online) until we decide to actually move away from it.

It is similar in having players fight with each other in space ships. It is different in the sense that it is a 2d game.

We aim at progressively transition from a single computer, local game to a multiplayer online game while learning in the meantime how to do it.

## AI of bots

It seems there are two main ways to handle AI for NPCs:
* behavior trees
* state machines

### Behavior trees

While searching for some resources, we gathered the following collection of articles:
* one on [gamedeveloper](https://www.gamedeveloper.com/programming/behavior-trees-for-ai-how-they-work).
* one on the root website of the approach (as it seems :D) at [behaviortree.dev](https://www.behaviortree.dev/).
* and of course on [wikipedia](https://en.wikipedia.org/wiki/Behavior_tree_(artificial_intelligence,_robotics_and_control)).

### State machines

State machines seem pretty linked to Unity which probably has an implementation of them. The collection of articles below present the concept:
* one explaining how it works [in Unity](https://pavcreations.com/finite-state-machine-for-ai-enemy-controller-in-2d/2/#Activity-class).
* this one without Unity over at [tutsplus](https://code.tutsplus.com/finite-state-machines-theory-and-implementation--gamedev-11867t).
* this [one](https://www.gamedeveloper.com/programming/designing-a-simple-game-ai-using-finite-state-machines) is similar to the previous one but simpler.

### Decision

For our purposes it seems like both things could work:
* state machines might be easier if it's easy to come up with all the states beforehand: then it's clear what are the transitions and what can happen in any state.
* decision trees might require a more careful definition of what can happen in which scenario. The fallback mechanisms are also not as clear as with the state machine where it's just transitioning to another state: here we would have to go up the hierarchy and see what to do.

After pondering a bit, we decided to go for behavior tree. The state machine seems similar to how we already approached AI in the past so maybe it's a good idea to try something new.

### Current state of the AI

The idea behind the behavior tree is to have a list of nodes arranged in a graph. They are executed at each step which allows to easily react to new situations. Contraty to state machines, we don't have to think about switching from any state to any other state: at each frame we just start from the root again and see which state we end up with when the execution of the tree is finished.

The following image shows an example of how the very simplistic AI works at the moment:

![Behavior tree](resources/behavior_tree_example.png)

In this example the root node is a repeater: this is usually advised as then we have an infinite behavior. We then have two main mode: the attack mode and the idle mode. Both of them are composed of a succession of actions. These are composed together with a fallback node, meaning that if the first strategy fail we will go on with the second one.

#### Attack mode

In the attack mode, the first thing is to pick a target: to that purpose the AI will scan its surroundings for a valid target. Two main results: success or failure.

In case it succeeds, we continue to the next: the AI will try to follow the target. This action just takes into consideration the target and tries to come closer to it: it succeeds immediately but also work iteratively. After this the Fire mode will be triggered which will check if the AI is close enough to the target and try to shoot at it.

In case it fails, then the parent sequence node will also fail as one of its child couldn't succeed and the repreater will then go on to the next element.

#### Idle mode

At any point the attack mode can fail: for example if the target goes too far from the AI, or if there's no target in the first place. In this case we count on the fallback node to start the idle mode.

The idle mode is composed of a succession of node to go to a target. This defines a patrol for the AI to loop through while waiting for something to happen.

This mode can't fail: the AI will just loop indefinitely until something else happens.

#### Orchestration

At each loop of the game we just iterate over the whole tree again. Usually it is advised to keep the processing time of nodes small so that it's not an issue to iterate over them very often.

Due to the dynamic nature of the tree and the fact that we iterate over it all the time, we can very easily react to a change:
* we're in idle mode but a target arrives? The attack mode will trigger itself on its own because the `PickTarget` node will suddenly return a valid target.
* we're shooting at the enemy but it dies? The next iteration will fail to find a target and we go back to idle mode.

This is much easier than having the AI in a certain state and then having at the very beginning of each state to do something like:
```cpp
if (determineState() != m_currentState) {
  return otherStateMethod();
}
```

It keeps the reactions of the AI dynamic by codifying them into the structure of the tree.

## Entity Component System

In order to represent the various elements of the game and their properties, we decided to implement an [entity component system](https://en.wikipedia.org/wiki/Entity_component_system).

The goal of this mechanism is to avoid the deeply nested class hierarchy common when developing a game. We already tried this approach when dealing with the agents (see the [agents](https://github.com/KnoblauchPilze/agents) repository).

While researching how to implement it we found this blog [codingwiththomas](https://www.codingwiththomas.com/blog/an-entity-component-system-from-scratch) which presents the implementation of such a system from scratch. It is well documented and explains the concept in quite some details.

In comparison with the `agents` project we decide to follow the paradigm a bit better and actually created some systems and a way to iterate through the entities.

### Entity

The [Entity](src/bsgo/entities/Entity.hh) is the base class and is basically just an identifier with a list of components. We just added some convenience methods to assess the existence of some component and access them in a secure way.

### Component

A [Component](src/bsgo/components/IComponent.hh) is the base class for all components. It just defines one basic method to update the component with some elapsed time. This might or might not be useful to the inheriting components but allows to have some quantity varying over time.

The goal of the implementation is to keep away the processing from the components in favour of putting all of them in the systems.

### System

A [System](src/bsgo/systems/ISystem.hh) is iterating over the entities and only processing the ones that are interesting. Our implementation defines an `AbstractSystem` which aims at iterating over the entities.

The constructor expects the inheriting classes to pass a callback which will be used to filter the entities and keep only the ones that are interesting for the system. This typically involves checking if an entity has some components which are handled by the system.

Typically if an entity defines a [HealthComponent](src/bsgo/components/HealthComponent.hh) we expect it to be processed by the [HealthSystem](src/bsgo/systems/HealthSystem.hh).

## Database interaction

In the [clone of ogame](https://github.com/KnoblauchPilze/sogserver) we put most of the logic to interact with the database in stored procedures. The problem was that it was quite cumbersome to make some validation on the input data and also to get back any error that might happen during the insertion.

For this project we wanted to revisit this hypothesis and see if it was relatively widely used or not. The findings can be summarized in these two articles:
* [Stored Procedures a bad practice at one of worlds largest IT software consulting firms?](https://softwareengineering.stackexchange.com/questions/65742/stored-procedures-a-bad-practice-at-one-of-worlds-largest-it-software-consulting)
* [Business Layer in Database logic system](https://softwareengineering.stackexchange.com/questions/325488/business-layer-in-database-logic-system)

It seems like this can be justified to sometimes have a bit of logic in the database but should (based on these two examples) be avoided.

In this context, we implemented a business logic layer which aims at separating the concerns to update the database and how to do it. Typically each action that the user can take usually involves multiple operations in the database. For example signing up means:
* registering the player
* adding some starting resources/gear
* register at least one ship
* set the current system of the ship to the starting system

This is probably why it's not popular to put all of this logic in a stored procedure: it is quite complex and can get messy really fast when writing it in the language of the procedure. It is also not easy to debug or validate parameters within the procedures. On the other hand we can very easily do this if we keep a business logic layer in the application.

We chose to use a concept of services, all inheriting from a base [IService](src/bsgo/services/IService.hh) class: there's a service for each kind of action (for example signing up, purchasing an item, etc.). Each of them define the conditions to meet for an action to be allowed, and the logic to actually perform the action.

Later on, it can also be relatively easily be extended to not do all the validations locally but rather by contacting a remote server.

**Note:** for now we don't use a single transaction to perform a complete action. This probably needs to be changed in the future to not leave the database in an inconsistent state.

## Interaction between the database and the ECS

### The problem statement

When the user is playing, there are some interactions which need to be persisted to the database. This will ultimately mean sending these commands through the network to the server which can then make the necessary verifications before allowing them.

A typical example is the jump. When the user selects a destination and initiates a jump, we register this in one of the component of the ship and decrement the countdown until it reaches zero. We have to persist the information that the ship is jumping somehow to the database but this information is within a component of the ECS.

Another example is when a player mines an asteroid and is supposed to gain some resources. This also relates to dealing damage in the first place and also killing an entity. These changes of state should be communicated to the database (and to the server) from within the ECS.

### Research

A straightforward solution is to make the systems aware of the database and just let them save whatever data they assume is meaningful to it. This has the benefit that it is simple to implement.

However thinking a bit ahead, what is supposed to happen when we will split the application into client/server? It seems like essentially every system would have to handle the networking aspect of getting data from the server and potentially patching whathever discrepencies might be detected between local and remote data.

We could find a bunch of relevant links on how to deal with this on the internet.

In [How to sync entities in an ecs game](https://gamedev.stackexchange.com/questions/178469/how-to-sync-entities-in-an-ecs-game) and [Networking with entity component system](https://www.reddit.com/r/gamedev/comments/5oib5v/networking_with_entity_component_system/) the recommendation is to use a `NetworkSynComponent` or something similar. The idea in each case is to have some dedicated `NetworkSystem` which takes care of receiving data and updated the entities which have a network component. The network component can presumably be either very stupid (just flagging the entity as having some form of networking aspect) or a bit smarter and define for example the properties of other components needing to be synced.

The topic in [How to network this entity system](https://gamedev.stackexchange.com/questions/21032/how-to-network-this-entity-system) does not recommend to go for a 'smart' network component as this is essentially mirroring what already exists in other components. Instead it seems like the approach would then be to have the `NetworkSystem` be able to deal with each component in a way similar to:
* check if the entity has a network aspect
* if yes iterates over all registered component
* for each component call an internal method that indicates which properties need to be updated and how

One drawback of this is that the `NetworkSystem` will grow quite a bit due to handling all the components we have in the game. The plus is that there's a single place which should deal with network communication. We could possibly also include the various reconciliation mechanisms here.

Finally the [Documentation of Space Station 14](https://docs.spacestation14.com/en/robust-toolbox/ecs.html) indicates some sort of `dirty` property to attach to component which would make the job of the `NetworkSystem` easier: by checking this we know if it should send data to the network or not. This is quite interesting as it allows to not keep track internally in the system of the previous states of all components to detect modifications.

### Possible solution

To come back to our jump/mining examples, here's how we could do it:
* create a network component which contains a `dirty` property.
* this property is updated by the systems when an action requires it (typically a jump is started or the health is changed or the loot is distributed, etc.).
* create a `NetworkSystem` which loops over entities having a network component.
* this system loops over the components of the entities and perform the syncing (for now to the database) for each of them.

For additional flexibility we could also create an enum which defines the type of component and register a list of enum values in the network component. This would allow to only sync some aspects of an entity rather than automatically syncing all its components.

## Messaging

The application also needs to make some changes in the UI based on what is happening in the ECS. For example when an asteroid is scanned, we want to display a message indicating the result of the analysis. Or when an entity is killed or an asteroid destroyed, we might also want to display some message to represent it.

Similarly, when the user clicks on interactive parts of the UI, we might need to make some changes to the entities: for example if the `DOCK` button is clicked, or an item is purchased, we need to reflect this in the ECS if it is relevant.

Later on when the application will be split into a client/server architecture, some of the interactions we presented before will be either received from the server (an enemy died) or sent to the server for validation (request to purchase an item).

These considerations made us realize that we needed some decoupling between what happens in the UI (respectively the ECS) and how is it applied to the ECS (respectively the UI).

In order to achieve this, we created a messaging system. This allows communicating from the systems operating on the entities to the UI in a decoupled and asynchronous way.

### Message queue

The [IMessageQueue](src/bsgo/communication/IMessageQueue.hh) allows to keep track of all the messages needing to be processed. These messages can be registred through the interface method `pushMessage`.

The `Game` class holds an attribute of type message queue and takes care of calling the `processMessages` once per frame. This could be refined in the future: for example when these events will be generated on the server side, we might want to have a processing thread sending the messages back to the clients.

### Listeners

The [IMessageListener](src/bsgo/queues/IMessageListener.hh) allows anyone to register to the message queue and get notified of the messages. We expect such objects to provide indications about which messages they are interested about and to feed them a message. It is guaranteed that the messages receieved through the `onMessageReceived` method will be of the right type for the listener.

### Messages

The meat of the messaging process is the [IMessage](src/bsgo/messages/IMessage.hh) class. This interface defines a type of message and we can add more type as we see fit. Most of the time a message corresponds to a flow in the game, such as jumping or scanning a resource.

You can find other examples of messages in the same source folder.

An important part of our approach is to provide de/serialization methods for the messages: this allows to easily send them through the network to the server and receive the response back. We added some [unit tests](tests/unit/bsgo/messages) for this behavior to make sure it does not break.

### Communication between the UI and the ECS

In the application we gave the [UiHandlers](src/client/lib/ui) the possibility to register themselves (or some other components) to the message queue of the application.

A common pattern is to make the handler implement the message listener interface and handle some relevant messages.

On the other hand, the systems are pushing messages to the queue not knowing (and not caring) if some component will read and process them or not. This seems quite interesting when considering the future client/server architecture. For example when the `DOCK` button is clicked, the UI will send a message indicating that such an event happened. If the event is successfully processed and considered valid, we consider that some other part of the application will most likely change the active screen.

### Consumers

We followed the [Publish-Subscribe](https://en.wikipedia.org/wiki/Publish%E2%80%93subscribe_pattern) pattern when handling messages. For each type of message we have a corresponding consumer. For example for dock messages we have a [DockMessageConsumer](src/server/lib/consumers/system/DockMessageConsumer.hh).

This component registers itself to the message queue and indicates its interest in receiving the dock messages. When one is received, we can intercept the message and perform the necessary verification before processing it: for example we might check that the ship concerned by the message is not already docked, or that it is close enough to the outpost or that it does not try to dock to an outpost in a different system etc.

Most of the time, the consumers are just calling the corresponding service (for example the `DockMessageConsumer` relies on the [ShipService](src/bsgo/services/ShipService.hh)).

This approach is flexible as it allows to easily keep messages which would fail to be processed for later analysis and also probably make it easy to send those messages to the server from each client.

### Updating the Game

With the previous example of the dock button being clicked, we still have to explain how the UI is finally notified about the success or the failure of the operation.

We implemented a [GameMessageModule](src/client/lib/game/GameMessageModule.hh) class. This class is a message listener and registers itself for certain messages which impact the UI. Typically the `DockMessage` will be receieved by this class and analyzed: if it succeeds we can notify the `Game` class by a direct method call to update the active screen.

The strength of this system is that if the processing fails for whatever reason we will never receive the message indicating that the dock succeeded (because it did not) and so the UI will not make any changes that would have to be reverted.

# Client/Server architecture

## Generalities

The aim of the project is to provide a client server architecture where multiple players can connect to the game and play together. In order to achieve this we decided to go for an authoritative server and separate client.

From the code perspective there are elements and structures that will be used both by the client and the server.

It is not so clear cut whether writing the server and the client in the same language bring huge benefits. This [article](https://softwareengineering.stackexchange.com/questions/171343/how-important-is-using-the-same-language-for-client-and-server) summarizes the research we did on this topic. As a summary: it depends™.

## Discussion on our approach

For now the client is written in C++ thanks to the `PixelGameEngine` renderer. We also developed the rest of the game (the ECS and the interaction with the database) in C++ for this reason.

As we only started to have a look at the server a bit later on, we had a choice to make between continuing the implementation of the server also in C++ or using a different language.

Our initial idea was to use `Go` as the server's language: we are more familiar with it and it handles the threading and networking quite well.

However the problem with it would be that we would have to essentially make some wrappers around the core game classes (the ECS system) as we don't want to rewrite it. Also the network part would probably involve some conversions between the data structure we receive in Go and the binding to communicate to the server in C++. This is a similar situation as we havd in the past for other projects and it was usually a bit of a pain to handle. Moreover, the networking shouldn't be such an issue that we would consider writing the server in another language: as we anyway will have to handle some level of networking on the client, it seems a bit counterproductive to do it a second time in the server.

## Structure of this repository

We divided the [source](src) folder into several directories to make it easy to segregate code that belong to one application from the rest.

### bsgo

Thie [bsgo](src/bsgo) folder contains the core library defining the Entity Component System and the interaction with the database. This will most likely be used both in the server and the client side.

### pge

The [pge](src/pge) folder contains the code to interact with the `PixelGameEngine`. By having it in its own folder we are able to minimize the surface we would have to replace if we want to use a different engine. This also allows to easily update it without having to change too much code: the rest of the modules (mainly the client) do not know that the underlying drawing routines are using the `PixelGameEngine`.

### client

The [client](src/client) folder regroups all the code that is used exclusively by the client. It links against the core library and enriches it to present a compelling application to the player.

### server

The [server](src/server) folder defines all the server specific code. It will most likely be a smart wrapper around the core library of the project to expose some networking capabilities.

### Add elements to the project

When developing new features, we will now ask ourselves the question whether it can potentially be used both by the client and the server or only one of them. Depending on the answer to this question we will put the code in the adequate folder.

It can also be that later on we realize that the `bsgo` library is too big, or that some separate features (for example networking or Data Transfer Objects) can be shared: this could be achieved by adding more top level folders in the [src](src) directory.

# Networking

## Research
The content of this [video series](https://youtu.be/2hNdkYInj4g?si=Q-NOTJ__p-5a2jS8) of Javidx9 was very informative. The resulting code can be found on [github](https://github.com/OneLoneCoder/Javidx9/tree/master/PixelGameEngine/BiggerProjects/Networking) and it inspired the [Connection](src/net/connection/Connection.hh) class we created.

## Generalities

We use the [asio](#asio) library without boost to handle network communication in the project. We extracted all the logic to perform the calls, connect to the server and to the client in a dedicated [net](src/net) folder: similarly to what happens for the `PixelGameEngine` wrapping we want to be able to swap libraries or update relatively easily.

⚠️ For now this is not entirely the case: even though none of the modules are using raw `asio` objects in their code, there are still some asio headers included in the `net` library. It is not too much of a problem for now but we could see to improve this later on.

## Context

The networking revolves around the idea of an `io_context`. This is wrapped by our own [Context](src/net/connection/Context.hh) class which is instantiated both on the client and the server. The idea is that this context runs from the whole lifetime of the application and is used in the background by `asio` to perform the network calls.

## Connection

A connection is the central object allowing to communicate. It has two modes: `SERVER` and `CLIENT`. This defines which operations it will perform. The difference is mainly on the order with which operations are performed as both connections will (during the game) be receiving and sending data.

For the `CLIENT` mode we expect to try to connect to the server. Conversely in `SERVER` mode we get ready to accept connections.

Sending data is made easy through the `send` method which takes a `IMessage` as input, serializes it and transmit it through the network.

Receiving data is potentially more complex: depending on the type of data we might want to perform certain processes and relatively different depending whether we're on the client or the server. To this avail we provide a `setDataHandler` which allows to pass a callback which will receive the raw data from the network.

## TcpServer

A specific component of the server is the [TcpServer](src/net/server/TcpServer.hh) class. In order to make it easy to extend it and possibly reuse it and also to hide the dependency to the `asio` library we mainly work with callbacks.

The [ServerConfig](src/net/server/ServerConfig.hh) defines several to handle a new connection (approval or denial), a disconnection from a client and some data received. This last part is to make it easy to automatically assign the same data handler (for now a method of the server class) to all incoming connections.

# Server design

## General idea

Ideally the server should process the events generated by the clients and return the answer to them. If we designed the data structures right, we should be able to essentially instantiate the same game loop as for the clients and plug in the messages coming from the network into the internal game loop. Those messages will be picked up by the ECS and generate some more messages. In turn, the output messages should be broadcast to the clients that are interested in them.

## Messages

In order to correctly receive messages from various sources, we added a new implementation of the [IMessageQueue](src/bsgo/communication/IMessageQueue.hh): the [SynchronizedMesageQueue](src/server/lib/messages/SynchronizedMessageQueue.hh).

Its role is to gather the messages incoming from potentially multiple sources and to safely register them in the system. We use a mutex to protect concurrent access to the actual queue of messages.

In addition to this, it would be relatively impractical for the server to also run say at fps and process the messages in this timeframe. It might be that there are times where no messages are emitted, or times where processing a message as fast as possible is critical.

To achieve this we created a [AsyncMessageQueue](src/server/lib/messages/AsyncMessageQueue.hh) which spawns a thread to process the messages. It uses the `SynchronizedMessageQueue` and reacts whenever a new message is posted. We then wake up the processing thread with a condition variable and process the messages.

When testing this approach we are able to process (⚠️ without consumers) 100x more messages than the client is producing. It seems robust enough for now. In the future we could also separate messages based on their type, their system or any other heuristic. This would allow to scale in a safer way if the amount of messages becomes a problem.

## Game loop

The server is responsible to simulate all the systems of the game and make the ships and elements there look lively. In the client, we ever simulate a single system: the one where the player currently is.

We picked an approach where we group the elements needed to simulate a single system and put them in a [processor class](src/server/lib/game/SystemProcessor.hh). This regroups:
* A [Coordinator](src/bsgo/Coordinator.hh)
* A [DataSource](src/bsgo/data/DataSource.hh)
* Some [services](src/bsgo/services/Services.hh)

The output of this simulation is a bunch of messages that need to be dispatched to the various clients interested in them.

# Future work

## Useful links

The art for spaceships comes from [imgur](https://imgur.com/im8ukHF) and [deviantart](https://www.deviantart.com/ariochiv/art/Stars-in-Shadow-Phidi-Ships-378251756).

We found [this link](https://austinmorlan.com/posts/entity_component_system/) as part of the ECS research which seems more advanced so we might come back to it later.

## Useful libraries

Json nlohmann:
https://github.com/nlohmann/json

ORM in cpp:
https://github.com/silverqx/TinyORM

# Current progress of the game

![state of the game 1](resources/state_of_the_game_1.png)

![state of the game 2](resources/state_of_the_game_2.png)

![state of the game 3](resources/state_of_the_game_3.png)

# DB cheat sheet

```sql
select ss.ship, p.id as player, ss.docked, ss.system, sj.system, ps.hull_points, p.name from ship_system ss left join ship_jump sj on sj.ship = ss.ship left join player_ship ps on ss.ship = ps.id left join player p on ps.player = p.id where p.name in ('colo', 'colo2');
```

```sql
update player_ship set hull_points = 370 where ship = 1;
```

```sql
update ship_system set docked=true where ship in ('1', '5', '4');
```
