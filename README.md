# Player/Stage Workspace

My workspace for a Robotics course

## Setup

> For best compatibility use Player 3.1 and Stage 4.1.1

### Install Ubuntu Desktop LTS

Get the latest [Ubuntu](https://www.ubuntu.com/) Desktop LTS. (Last tested under Ubuntu Desktop 16.04 LTS)

### Installing Player and Stage

The install .sh scripts are in /Bash_Files:

1. Download and copy the scripts to a new directory (e.g. /home/user/player_stage)
2. Execute install_dependency_packages.sh
3. Execute install_player_stage.sh

### Clone Git Repo

> This repository consists of symbolic linked files so most examples will be provided from the player and stage installations. Some example files are added in place their originals, so one can run .cfg files with a compiled Player driver.

## Drivers

### Compiling Examples

Use the following:

g++ -o proj.o \`pkg-config --cflags playerc++\` proj.cc \`pkg-config --libs playerc++\`

> The git repo ignores files with ".o" extension, therefore you should postfix output files with ".o".

### Development

Primarily drivers are written in C++ and use [libplayerc++](http://playerstage.sourceforge.net/doc/Player-svn/player/group__player__clientlib__cplusplus.html) libraries

> See this [link](http://player-stage-manual.readthedocs.io/) for the online guide to Player/Stage latest builds
