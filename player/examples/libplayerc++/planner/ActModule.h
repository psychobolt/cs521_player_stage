#pragma once

#include "Libraries.h"
#include "Globals.h"
#include "IModule.h"
#include "behaviors/IBehavior.h"

class ActModule : public IModule<ActModule>
{
  friend class IModule;

  public:
    static Localization LOCALIZATION;
    static Sonar SONAR;
    void Run();
    bool IsDone();

  private:
    ActModule();
    bool isDone;
    vector<IBehavior*> behaviors;
    BVector bvector;
    BVector GetVector();

    class GoToBehavior : public IBehavior
    {
      public:
        Vector2d GetVector();
        bool IsDone();
    
      private:
        Vector2d GetCurrentPos();
    };

    class AvoidObstacleBehavior : public IBehavior
    {
      public:
        Vector2d GetVector();
        bool IsDone();
      
      private:
        Vector2d GetRangeVector(uint32_t scanIndex);
    };
};
