# Player/Stage Workspace

My workspace for a Robotics course

## Setup

### Install Ubuntu Desktop LTS

Get the latest [Ubuntu](https://www.ubuntu.com/) Desktop LTS. (Last tested under Ubuntu Desktop 16.04 LTS)

### Installing Player and Stage

The install .sh scripts are in /Bash_Files:

1. Copy the scripts to a new directory (e.g. /home/user/player_stage)
2. Execute install_dependency_packages.sh
3. Execute install_player_stage.sh (Downloads latest from version control)

### Clone Git Repo

> This repository consists of symbolic linked files so most examples should be provided from the installed versions. Some example files are added in place their originals, so one can run .cfg files with a compiled Player driver.

## Compiling Example Drivers

Use the following:

g++ -o proj.o \`pkg-config --cflags playerc++\` proj.cc \`pkg-config --libs playerc++\`

> The git repo ignores files with ".o" extension, therefore you should postfix output files with ".o".
