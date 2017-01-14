#! /bin/bash
# make sure to run with super user (sudo) privileges

##### Install Dependencies #####

# Updating
sudo apt-get update
sudo apt-get -y upgrade

# Dependencies
sudo apt-get -y install autotools-dev
sudo apt-get -y install build-essential
sudo apt-get -y install cmake
sudo apt-get -y install cpp
sudo apt-get -y install libboost-thread1.55.0
sudo apt-get -y install libboost-thread-dev
sudo apt-get -y install libboost-signals1.55.0
sudo apt-get -y install libboost-signals-dev
sudo apt-get -y install libcv2.4
sudo apt-get -y install libcv-dev
sudo apt-get -y install libcvaux2.4
sudo apt-get -y install libcvaux-dev
sudo apt-get -y install libgnomecanvas2-dev
sudo apt-get -y install libgnomecanvasmm-2.6-1c2a
sudo apt-get -y install libgnomecanvasmm-2.6-dev
sudo apt-get -y install libgsl0-dev
sudo apt-get -y install libgsm1
sudo apt-get -y install libhighgui-dev
sudo apt-get -y install libjpeg8-dev
sudo apt-get -y install libraw1394-dev
sudo apt-get -y install libtool
sudo apt-get -y install libxmu-dev
sudo apt-get -y install robot-player
sudo apt-get -y install swig
sudo apt-get -y install python-dev

# stage
sudo apt-get -y install freeglut3
sudo apt-get -y install freeglut3-dev
sudo apt-get -y install libfltk1.3
sudo apt-get -y install libfltk1.3-dev
sudo apt-get -y install libgtk2.0-dev
sudo apt-get -y install libltdl-dev
sudo apt-get -y install libgeos-3.4.2
sudo apt-get -y install libgeos-c1
sudo apt-get -y install libgeos-dev
sudo apt-get -y install libgeos++-dev
sudo apt-get -y install libpng12-dev

