#pragma once

#include "../Libraries.h"

class IBehavior
{
  public:
    virtual ~IBehavior() {}
    virtual Vector2d GetVector() = 0;
    virtual bool IsDone() = 0;
};