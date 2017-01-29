#define BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <cmath>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <libplayerc++/playerc++.h>
#include "Eigen/Dense"
#include "easyloggingpp-8.91/easylogging++.h"
_INITIALIZE_EASYLOGGINGPP

using namespace Eigen;
using namespace std;
using namespace PlayerCc;

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

PlayerClient * robot;
Position2dProxy * position2d;
ifstream * channel;

class PlanModule : public IModule<PlanModule>
{
  friend class IModule;
  friend class ActModule;

  public:
    void Run()
    {
        robot->Read();
        Navigator::GetInstance().Run();
        Pilot::GetInstance().Run();
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

    class Pilot : public IModule<Pilot>
    {
      friend class IModule;

      public:
        void Run()
        {
           if (IsDone())
           {
             forwardSpeed = 0.0;
             yawSpeed = 0.0;
             return;
           }
           Vector2d target = Navigator::GetInstance().GetNextPos()
            - GetCurrentPos();
           double forwardAngle = position2d->GetYaw();
           double targetAngle = atan2(target[1], target[0]);
           double deltaAngle = RTOD(NORMALIZE(targetAngle - forwardAngle));
           int sign = deltaAngle > 0 ? 1 : -1;
           if (abs(deltaAngle) > 10.0)
           {
             forwardSpeed = 0.1;
             yawSpeed = 2.0 * sign;
           }
           else
           {
             forwardSpeed = 1.0;
             yawSpeed = 0.1 * sign;
           }
        }
        double GetForwardSpeed()
        {
          return forwardSpeed;
        }
        double GetForwardYawSpeed()
        {
          return yawSpeed;
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
        Pilot() : IModule()
        {
          LINFO << "| Pilot Module initialized";
        }

        double forwardSpeed, yawSpeed;
        double GetNextYawSpeed(double deltaAngle, double turnRate)
        {
          double threshold = 1;
          if (deltaAngle < -threshold)
          {
            return -turnRate;
          }
          else if (deltaAngle > threshold)
          {
            return turnRate;
          }
        }
    };

    class Navigator : public IModule<Navigator>
    {
      friend class IModule;

      public:
        void Run()
        {
          if (Pilot::GetInstance().IsDone() && !IsDone())
          {
            string line;
            if (getline(*channel, line))
            {
              istringstream is(line);
              is >> next[0];
              is >> next[1];
              LINFO << "Move to " << next[0] << ", " << next[1];
            }
            else
            {
              channel->close();
            }
          }
        }
        Vector2d GetNextPos()
        {
          return next;
        }
        bool IsDone()
        {
          return !channel->is_open();
        }
      private:
        Navigator() : IModule()
        {
          robot->Read();
          next = Pilot::GetInstance().GetCurrentPos();
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
        position2d->SetSpeed(
          PlanModule::Pilot::GetInstance().GetForwardSpeed(),
          PlanModule::Pilot::GetInstance().GetForwardYawSpeed()
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
    printf("Usage: \"%s ./waypoints.txt\" # Please enter a waypoint datafile\n",
      argv[0]);
    return -1;
  }

  ifstream file(argv[1]);
  channel = &file;

  PlayerClient client("localhost");
  Position2dProxy p2dProxy(&client, 0);
  robot = &client;
  position2d = &p2dProxy;

  while (!PlanModule::GetInstance().IsDone())
  {
    PlanModule::GetInstance().Run();
    ActModule::GetInstance().Run();

    usleep(1); // wait 1 sec per Tick
  }

  LINFO << "Robot navigation completed";
  return 0;
}
