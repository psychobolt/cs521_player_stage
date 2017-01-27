#include <stdio.h>
#include <libplayerc++/playerc++.h>

class IModule
{
  public:
    virtual ~IModule() {}
    virtual void run() = 0;
};

class ActModule : public IModule
{
  public:
    void run() {

    }
};

class Navigator
{

};

class Pilot
{

};

class PlanModule : public IModule
{
    ActModule * act;
  public:
    PlanModule(ActModule *actModule) {
      act = actModule;
    }
    void run() {

    }
};

int main(int argc, char *argv[])
{
  using namespace PlayerCc;
  PlayerClient robot("localhost");
  Position2dProxy p2dProxy(&robot,0);

  IModule * act = new ActModule();
  //IModule * planner = new PlanModule(static_cast<ActModule*>(act));

  while (true) {
    //planner->run();
    act->run();
  }
  return 0;
}
