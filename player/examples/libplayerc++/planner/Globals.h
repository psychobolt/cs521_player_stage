#pragma once

#include "Libraries.h"

struct BVector {
  double magnitude;
  double direction;
};

struct Client {
    PlayerClient * robot;
    istream * channel;
    bool DEBUG;
};

struct Localization {
    Position2dProxy * position2d;
};

struct Sonar {
    LaserProxy * laser;
};