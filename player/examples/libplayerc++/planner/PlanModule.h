#pragma once

#include "Libraries.h"
#include "Globals.h"
#include "IModule.h"
#include "PathFinder.h"

class PlanModule : public IModule<PlanModule>
{
  friend class IModule;
  friend class ActModule;

  public:
    static Client CLIENT;
    void Run();
    bool IsDone();
    Vector2d GetNextPos();
    Vector2d GetCurrentPos();
    
  private:
    PlanModule();

    class Pilot : public IModule<Pilot>
    {
      friend class IModule;

      public:
        stringstream channel;
        void Run();
        Vector2d GetNextPos();
        bool IsDone();

      private:
        Pilot();
        Vector2d next;
    };
    
    class Navigator : public IModule<Navigator>
    {
      friend class IModule;

      public:
        void Run();
        bool IsDone();

      private:
        Navigator();
        PathFinder pathFinder;
    };
};
