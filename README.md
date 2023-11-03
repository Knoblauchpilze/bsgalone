# bsgalone

Yet another copy of a famous game which will be probably not as polished and not as fun as the original. This time we try out: BSGO.

# Installation

## Prerequisite

This projects uses:

- [google test](https://github.com/google/googletest): installation instructions [here](https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/), a simple `sudo apt-get install libgtest-dev` should be enough.
- [cmake](https://cmake.org/): installation instructions [here](https://askubuntu.com/questions/355565/how-do-i-install-the-latest-version-of-cmake-from-the-command-line), a simple `apt-get` should also be enough.
- [eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page): installation instructions [here](https://www.cyberithub.com/how-to-install-eigen3-on-ubuntu-20-04-lts-focal-fossa/) for Ubuntu 20.04, a simple `sudo apt install libeigen3-dev` should be enough.

## Instructions

- Clone the repo: `git clone git@github.com:Knoblauchpilze/pge-app.git`.
- Clone dependencies:
  - [core_utils](https://github.com/Knoblauchpilze/core_utils)
  - [maths_utils](https://github.com/Knoblauchpilze/maths_utils)
- Go to the project's directory `cd ~/path/to/the/repo`.
- Compile: `make run`.

Don't forget to add `/usr/local/lib` to your `LD_LIBRARY_PATH` to be able to load shared libraries at runtime. This is handled automatically when using the `make run` target (which internally uses the [run.sh](data/run.sh) script).

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

## Steps

✅ Extract the coordinator from the view and remove the update method from the IView interface.  
✅ Call the update method of the coordinator in the step method of the game.

✅ Répertoire pour la ui de l'outpost  
✅ Modifier le texture pack pour gérer mieux la mémoire pour les olc::Decal  
✅ Créer une méthode pour avoir uniquement un Decal et pas forcément un Pack  
✅ Une image de fond genre docking bay  
✅ Un menu général avec shop/locker/hangar  
✅ Le menu Locker présente une vue de la liste des items dans la partie gauche et une liste des Slots a droite  
✅ Le Locker est une liste de slot avec un type et un id qui indique dans quel repo aller chercher les information

✅ Introduire la notion de faction: créer un répertoire enums dans le folder bsgo avec ça  
✅ Chaque ship et outpost ont une faction

✅ Introduire la notion de slot type, similaire à la notion de faction  
❌ Chaque ship a un count de slot de chaque type

✅ Rename findOneById dans le ship repository  
❌ Ajouter un findSlotByIdAndType dedans qui retourne une list d'id

✅ Séparer les différents aspects du loading dans le DataSource  
✅ Refine le Velocity component pour inclure l'accélération maximale  
✅ Utiliser l'accélération maximale du vaisseau dans le component

✅ Le PlayerShip definit un vector pour les id des slots de chaque type  
✅ Ajouter un WeaponRepository qui permet d'accéder aux weapons dont l'id a été retourné dans le ShipRepository  
✅ Il retourne une weapon avec dedans un niveau de dégâts et un reload time et un coût énergétique et une portée

✅ Ajouter un IComponent interface  
✅ Ajouter un RegenerativeComponent  
✅ Faire en sorte que Power et Health derive de celui ci

✅ Le bouton de dock ne doit être accessible qu'aux ships de la bonne faction

✅ Faire en sorte que la couleur de BG de la weapon UI change de couleur en fonction du statut de l'arme : vert si elle peut tirer, orange si pas assez d'énergie, jaune si reload et rouge si hors portée  
✅ Afficher la portée et les dégâsses sur le menu

✅ Ajouter un attribut heading dans le transform component  
✅ Orienter le vaisseau basé là dessus et pas sur la vitesse  
✅ La vitesse modifie le heading immédiatement

✅ Ajouter un attribut on/off sur les WeaponSlot  
✅ Attacher la touche G pour modifier toutes mes armes dans le GameScreenInputHandler

✅ Créer un TargetComponent et remplacer ce que de droit  
✅ Dans le DataSource on crée le target component pour les entités de type ship et outpost  
✅ Dans le GameScreenInputHandler on n'update plus la TargetView mais le component de l'entity  
✅ Dans le GameScreenUi on ne prend plus la target view mais le component de l'entity  
✅ On peut probablement supprimer la TargetView

❌ Ajouter un onEntityAdded sur le ISystem?  
❌ Ajouter un onComponentAdded sur le ISystem?

✅ Créer un WeaponSystem  
✅ D'après https://gamedev.stackexchange.com/questions/71711/ecs-how-to-access-multiple-components-not-the-same-one-in-a-system ce système itere sur les entités au lieu des components et accède uniquement a celle qui ont une weapon (puisque tout le monde a une transform)  
✅ Si l'arme est on et a portée on essaie de l'utiliser sur la target si elle est valide en decrementant le power component

✅ Créer un AbstractSystem pour itérer sur les entités. Il prend en paramètre une fonction pour filtrer les entités  
✅ Les autres systèmes héritent de cette classe

✅ On ne peut tirer que sur les entités qui ne sont pas de notre faction  
✅ Créer un FactionComponent

❌ Dans Coordinator remplacer la map m_entities par une map avec une struct comme valeur. Cette struct possède le kind et un flag active  
❌ Ajouter une méthode deleteEntity qui met ce flag a false  
✅ Supprimer le use du RegenerativeComponent et le mettre dans les Power et Health components  
✅ Ajouter une methode isAlive dans le Health  
❌ Les autres méthodes utilisent ce flag pour récupérer les entités -> pas besoin puisque chaque entite est supprimée dans la même frame qu'elle est détruite  
✅ Ajouter une méthode cleanUpEntities qui itere sur la liste m_entities et qui supprime celles qui sont désactivées  
✅ Appeler cette méthode dans le update  
✅ Le WeaponSystem enleve la target d'une entite si elle est morte  
✅ Dans le WeaponSystem on vérifie si la vie est trop basse auquel cas on set le flag active de l'entité

❌ Comment gérer la soute ?  
✅ Ajouter un LockerComponent ? -> Oui

✅ Ajouter un optional d'une ressource dans un astéroïde (juste un float)  
✅ Ajouter un LootComponent dans le cas où l'astéroïde a une resource : ce composant a besoin d'un endroit où stocker le loot

✅ Computer sont une autre classe qui hérite de SlotComponent : elle a des méthodes similaires  
✅ Chaque ordinateur dans la DB sera définit avec in enum type qui dit qu'elle caractéristique est amelioré mais est utilisé dans le ECS comme ayant l'ensemble des stats améliorées (certaines étant 0)  
✅ On peut l'ajouter dans le PlayerShip et aussi dans le DataSource en tant que SlotComponent  
✅ L'entité a une liste de Computer comme elle a une liste de weapons  
✅ Le WeaponSystem parcourt la liste des ordis et applique le résultat sur chaque arme

✅ Plutot que de partir avec les slot d'ordi, on peut créer un EffectComponent qui a une durée et un effet  
✅ Le WeaponChangeComponent peut soit buf/debuff les armes : on pourra ajouter d'autres effets plus tard  
✅ Lorsqu'on clique sur un ordi, on ajoute un component qui correspond à l'effet de l'ordi  
✅ Un systeme se charge d'update les effets et de supprimer les components quand ils sont terminés  
✅ Ajouter une methode removeXY pour un EffectComponent chaque component dans le coordinator

✅ Comment gérer le player ? Est ce une entité ? -> Oui

✅ Créer un target system qui clear les target si elles sont dead  
✅ Ease of use for the Entity struct

✅ Changer ISystem pour que update soit const  
✅ Changer AbstractSystem pour que updateEntity soit const  
✅ Appliquer le damage modifier dans le WeaponSystem  
✅ Faire en sorte qu'un ordi ait un temps de recharge  
✅ Afficher le temps de recharge de maniere visible  
✅ Faire en sorte que les ordis consomment de l'energie  
✅ Afficher les info de l'ordi dans la UI

✅ Afficher le reload time dans la UI  
✅ Vérifier que les armes ne tirent pas toutes en même temps s'il n'y a que de l'énergie pour une seule  
✅ Ajouter un parametere pour ignorer les enfants dans le menu pour le processUserInput  
✅ Les armes peuvent etre des/activees en cliquant dessus

✅ Le scan est un ordi offensif sans modifier  
✅ Chaque astéro a un scanned component  
✅ Le renderer prend en compte le scanned et affiche le loot

✅ Trouver un moyen de faire en sorte que le clic sur un ordi n'active pas mais fasse une requete de clic?  
✅ Rajouter une list de kind pour les ordis qui definit sur lesquels il s'appliquent  
✅ Modifier le computer system pour qu'il regarde pas hasTargetDifferentFaction mais plutot hasTargetRightKind  
✅ Tout comme on a un addWeaponEffect dans le WeaponSystem on peut detecter que le computer s'applique  sur les astero et declencher le scanned component

✅ Ne pas exposer la methode getEntity publiquement sur la IView : on doit passer par les systemes

✅ Le LootComponent est piloté par un LootSystem  
✅ Dans le WeaponSystem, quand on damage une entité on cherche si elle a un composant loot et si oui on s'enregistre dans la liste des gens qui ont fait du dégâts  
✅ Le LootSystem s'occupe des entités qui sont mortes et distribue le loot a ceux qui se sont enregistré dans les dégâsses  
✅ Créer un PlayerComponent  
✅ Ajouter dans le PlayerShip un id qui correspond au player  
✅ Dans le DataSource on ajoute le PlayerComponent si le ship a un player id  
✅ Le LootSystem prend en compte le PlayerComponent pour savoir s'il y a du loot à distribuer

❌ Comment stocker ce loot ? Dans le repository ?  
✅ Dans une autre entité qui serait le player et qui aurait un LockerComponent ? Une entite qui ne serait pas visible ? -> On a une entité player qui contient tout le locker en tant que components (y compris les resources)

✅ Creer une nouvelle entite dans le DataSource qui correspond au player  
✅ Dans le DataSource on n'enregiste l'id non pas du player mais de cette entité dans le PlayerComponent  
✅ Le LootSystem recupère cette entité et ajoute le loot  
✅ Créer un LockerComponent  
✅ Créer un AsteroidRepository qui permet de recuperer le loot d'un asteroid  
❌ Créer un Loot qui a un type et un DbId qui correspond à un id dans l'un des repository (weapon, computer)  
❌ Le type d'un item est une enum class comme faction ou entity kind  
✅ Le LootComponent devrait avoir acces a l'item venant du AsteroidLootRepository  
✅ Modifier le Asteroid pour que la resource soit un id qui correspond au loot  
✅ Adapter le DataSource pour ajouter ca au LootComponent  
✅ Le LootComponent en plus du db id prend un type de loot en argument. Cela permet au LootSystem d'adapter correctement la distribution  
✅ Le LootComponent permet de retrouver le type et l'id  
❌ Le LootSystem ajoute le loot dans le LockerComponent du player en transferant l'id et le type au locker  
❌ Le LockerComponent détient une liste d'id avec leur type  
✅ Le LootSystem ajoute une entree dans le repository correspondant au type de loot  
✅ Créer un PlayerResourceRepository qui retourne une struct resource avec un nom et une amount

❌ A-t-on besoin du LockerComponent ? -> A priori non

✅ On pourrait attacher des composants resource a l'entité player, un par resource  
✅ Le LootSystem pour les resources update ce composant au lieu d'update la db  
La UI peut aussi utiliser ce composant pour savoir combien de resource afficher

✅ Supprimer les repositories du Coordinator  
✅ Rendre les repositories accessibles depuis le DataSource

✅ Bullets  
✅ Changer la friction pour qu'elle multiplie le elapsedSeconds et pas la vitesse  
✅ Rendre le VelocityComponent capable de prendre une vitesse en paramètre de constructor: dans ce cas l'accélération est faire de telle sorte qu'elle match la friction  
✅ Rajouter une entrée Bullet dans le EntityKind  
✅ Le WeaponSystem si l'arme peut tirer créer une entité Bullet qui a une Velocity et une Transform et une valeur de dommage  
✅ Faire un système de rendu pour les bullets avec un sprite  
✅ Créer un DamageComponent qui contient la valeur de damage d'une bullet  
✅ Créer une BulletSystem qui prend en compte les bullets et quand elles arrivent trop près de leur cibles on les fait exploser : le code est le même que ce qu'il y a dans le WeaponSystem  
✅ Le BulletSystem marque la bullet pour destruction quand elle fait du dégât  
✅ Le BulletSystem change l'accélération de la Bullet pour toujours être dirigée vers la target  
✅ Quand la target meurt, la Bullet meurt aussi  
✅ Renommer le PlayerComponent en OwnerComponent  
✅ S'assurer que le BulletSystem s'enregistre dans le LootComponent avec le bon id (celui du ship, pas celui de la bullet)

✅ Le VelocityComponent ne marche pas comme il devrait. La friction et la vitesse sont pas terrible -> solved par un article et un Fixed mode pour la vitesse

✅ Locker UI  
✅ Le plus simple est sûrement d'avoir accès aux repositories depuis la UI -> fait via les views  
✅ Créer un PlayerWeaponRepository, un ShipWeaponRepository  
✅ Modifier le DataSource pour que le ship retourné par PlayerShipRepository n'ait pas de weapon : elle sont retournées par une autre requête a ce ShipWeaponRepository puis au PlayerWeaponRepository  
✅ Faire pareil pour les ordinateurs  
✅ Eclater la UI de l'outpost en plus petits morceaux  
✅ La UI interroge le PlayerResourceRepository pour avoir la liste des ressources. On récupère le nom et la quantité

✅ La UI interroge le PlayerWeaponRepository pour récupérer la liste des armes. Elle croise ça avec le data de ShipWeaponRepository pour avoir la liste dans le Locker  
✅ On peut extraire cela dans la PlayerView et peut être la ShipView  
✅ Les menus sont juste composé d'un agrégat avec l'id de l'item pour l'instant  
✅ Ajouter un nom au computer et aux weapons  
✅ Afficher ce nom dans la UI

✅ Shop UI  
✅ La UI interroge le WeaponRepository et le Computer repository pour récupérer la liste des templates de chaque type  
✅ On peut extraire ca dans la ShopView  
✅ Créer un ResourceRepository : il retourne une struct avec le nom de la resource et c'est tout  
✅ Créer un WeaponPriceRepository pour représenter le prix d'une weapon. Ce prix fait référence a des id de ressources (dans le ResourceRepository) et des quantités  
✅ Pareil pour les ordis  
✅ La UI affiche juste le prix en resource avec le nom et une couleur pour chaque type d'item  
✅ Ajouter un ordi et une weapon a acheter

✅ UI pour le outpost

Améliorer le systeme dans lequel le joueur evolue  
Autre vaisseaux dans le jeu  
DB pour les données