#!/bin/bash
# make sure to run with super user (sudo) privileges
# restart terminal after running this script

##### Install Player-Stage #####

# install svn and git
sudo apt-get -y install subversion
sudo apt-get -y install git

# download player and stage files
svn checkout svn://svn.code.sf.net/p/playerstage/svn/code/player/trunk player
git clone http://github.com/rtv/Stage.git stage

# build player
cd player/
mkdir build
cd build/

sudo cmake ../
sudo make
sudo make install


# build stage
cd ../../stage
mkdir build
cd build/

sudo cmake ../
sudo make
sudo make install

cd ../..

# add additional path to environment variable to be able to run "player /usr/local/share/stage/worlds/simple.cfg"
arch=$(uname -m)
if [ "$arch" == "i686" ]
then 
	sudo sed -i '$ a\export LD_LIBRARY_PATH+=/usr/local/lib' /etc/bash.bashrc
	sudo sed -i '$ a\export STAGEPATH+=/usr/local/lib' /etc/bash.bashrc
	sudo sed -i '$ a\export PLAYERPATH+=/usr/local/lib' /etc/bash.bashrc
fi
if [ "$arch" == "x86_64" ]
then
	sudo sed -i '$ a\export LD_LIBRARY_PATH+=/usr/local/lib64' /etc/bash.bashrc
        sudo sed -i '$ a\export STAGEPATH+=/usr/local/lib64' /etc/bash.bashrc
        sudo sed -i '$ a\export PLAYERPATH+=/usr/local/lib64' /etc/bash.bashrc
fi

