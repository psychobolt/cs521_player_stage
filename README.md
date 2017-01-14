# Player/Stage Workspace

My workspace for a Robotics course

## Setup

### Install Ubuntu Desktop LTS

Get the latest [Ubuntu](https://www.ubuntu.com/) Desktop LTS. (Last tested under Ubuntu Desktop 16.04 LTS)

### Installing Player and Stage

Run the shell scripts:

1. install_dependency_packages.sh
2. install_player_stage.sh (Installs latest from version control)

### Clone Git Repo

> This repository consists of symbolic linked files so most examples should be provided from the installed versions. Some example files are added in place their originals, so one can run .cfg files with a compiled Player driver.

## Compiling Example Programs

g++ -o proj `pkg-config --cflags playerc++` proj.cc `pkg-config --libs playerc++`
