#define BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <libplayerc++/playerc++.h>
#include "Eigen/Dense"
#include "easyloggingpp-8.91/easylogging++.h"
_INITIALIZE_EASYLOGGINGPP

using namespace Eigen;
using namespace std;
using namespace PlayerCc;

struct BVector {
  double magnitude;
  double direction;
};

template <class T>
class IModule
{
  public:
    static T& GetInstance()
    {
      static T instance;
      return instance;
    }
    void operator=(T const&);
    virtual void Run() = 0;
  protected:
    IModule() {}
    IModule(IModule const&);
    virtual ~IModule() {}
};

class IBehavior
{
  public:
    virtual ~IBehavior() {}
    virtual Vector2d GetVector() = 0;
};

PlayerClient * robot;
Position2dProxy * position2d;
LaserProxy * laser;
istream * channel;


class PlanModule : public IModule<PlanModule>
{
  friend class IModule;
  friend class ActModule;

  public:
    void Run()
    {
        robot->Read();
        Navigator::GetInstance().Run();
        ReactivePilot::GetInstance().Run();
    }
    bool IsDone()
    {
      return Navigator::GetInstance().IsDone();
    }
  private:
    PlanModule() : IModule()
    {
      LINFO << "Plan Module initialized";
    }

    class ReactivePilot : public IModule<ReactivePilot>
    {
      friend class IModule;

      public:
        void Run()
        {
          bvector.magnitude = 0.0;
          bvector.direction = 0.0;
          if (IsDone())
          {
            return;
          }
          Vector2d target(0.0, 0.0);
          for (vector<IBehavior*>::iterator iter = behaviors.begin(); iter != behaviors.end(); iter++) {
            target += (*iter)->GetVector();
          }
          double forwardAngle = position2d->GetYaw();
          double targetAngle = atan2(target[1], target[0]);
          double deltaAngle = RTOD(NORMALIZE(targetAngle - forwardAngle));
          int sign = deltaAngle > 0 ? 1 : -1;
          if (abs(deltaAngle) > 10.0)
          {
            bvector.magnitude = 0.1;
            bvector.direction = 2.0 * sign;
          }
          else
          {
            bvector.magnitude = 1.0;
            bvector.direction = 0.1 * sign;
          }
        }
        BVector GetVector()
        {
          return bvector;
        }
        Vector2d GetCurrentPos()
        {
          Vector2d v(position2d->GetXPos(), position2d->GetYPos());
          return v;
        }
        bool IsDone()
        {
          return 0.05 > (Navigator::GetInstance().GetNextPos()
            - GetCurrentPos()).norm();
        }
        
      private:
        ReactivePilot() : IModule()
        {
          behaviors.push_back(new GoToBehavior());
          behaviors.push_back(new AvoidObstacleBehavior());
          LINFO << "| ReactivePilot Module initialized";
        }
        vector<IBehavior*> behaviors;
        BVector bvector;

        class GoToBehavior : public IBehavior
        {
          public:
            virtual Vector2d GetVector()
            {
              return Navigator::GetInstance().GetNextPos()
                - ReactivePilot::GetInstance().GetCurrentPos();
            }
        };
        class AvoidObstacleBehavior : public IBehavior
        {
          public:
            virtual Vector2d GetVector()
            {
              bool avoid = false;
              Vector2d result(0.0, 0.0);
              for (int i = 0; i < laser->GetCount(); i++) {
                Vector2d v = GetRangeVector(i);
                result += v;
                if (v.norm() < 0.5) {
                  avoid = true;
                }
              }
              if (!avoid) {
                Vector2d null(0.0, 0.0);
                return null;
              }
              return result;
            }
          private:
            Vector2d GetRangeVector(uint32_t scanIndex)
            {
              player_point_2d_t point = laser->GetPoint(scanIndex);
              Vector2d v(point.px, point.py);
              Rotation2D<double> rot(position2d->GetYaw());
              return rot * v;
            }
        };
    };

    class Navigator : public IModule<Navigator>
    {
      friend class IModule;

      public:
        void Run()
        {
          if (ReactivePilot::GetInstance().IsDone() && !IsDone())
          {
            if (channel->eof())
            {
              return;
            }
            string line;
            if (getline(*channel, line)) {
              istringstream is(line);
              is >> next[0];
              is >> next[1];
              LINFO << "Move to " << next[0] << ", " << next[1];
            }
          }
        }
        Vector2d GetNextPos()
        {
          return next;
        }
        bool IsDone()
        {
          return channel->eof();
        }
      private:
        Navigator() : IModule()
        {
          robot->Read();
          next = ReactivePilot::GetInstance().GetCurrentPos();
          LINFO << "| Navigator Module initialized";
        }
        Vector2d next;
    };
};

class ActModule : public IModule<ActModule>
{
  friend class IModule;

  public:
    void Run()
    {
      if (position2d != NULL)
      {
        BVector vector = PlanModule::ReactivePilot::GetInstance().GetVector();
        position2d->SetSpeed(
          vector.magnitude,
          vector.direction
        );
        // Vector2d target = PlanModule::Navigator::GetInstance().GetNextPos();
        // position2d->GoTo(target[0], target[1], 0.0);
      }
    }
  private:
    ActModule() : IModule()
    {
      LINFO << "Act Module initialized";
    }
};

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Usage: \"%s x y\" # Where x and y are signed double values\n",
      argv[0]);
    return -1;
  }

  string x = argv[1];
  string y = argv[2];
  istringstream is(x + " " + y + "\n\0");
  channel = &is;

  try {
    PlayerClient client("localhost");
    Position2dProxy p2dProxy(&client, 0);
    LaserProxy laserProxy(&client, 0);

    robot = &client;
    position2d = &p2dProxy;
    laser = &laserProxy;

    p2dProxy.SetMotorEnable(true);
    while (!PlanModule::GetInstance().IsDone())
    {
      PlanModule::GetInstance().Run();
      ActModule::GetInstance().Run();
    }
    
    LINFO << "Robot navigation completed";
  }
  catch (PlayerError &e)
  {
    LERROR << e;
    return -1;
  }
  
  return 0;
}
