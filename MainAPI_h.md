# Introduction #

This is the file that load's the config files, changes stuff insed the game. this is in early 'PRE\_ALPHA' and will be more usefull later.


# Details #

This are the stuff that is working:

  * Load config file using .cfg using ConfigFile.h
  * Load plugins/mods from a lua file. -NOT STARTED

# Functions #
```C++

API(); //Constructor
~API(); //Destructor
void initAPI(int usingSD); //Inits the API.
void stopAPI(); //Stops the API (not needed to run at the moment)```

# Defines #

```C++

#define APIVERISON "02" //Type: String
#define APILANGUE "LUA" //Type: String```