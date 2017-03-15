#include "planner/Planner.h"

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Usage: \"%s x1,y1 x2,y2 # Please enter a waypoint set\n",
      argv[0]);
    return -1;
  }

  stringstream iostring;
  for (int i = 1; i < argc; i++) {
    string v = argv[i];
    if (v.compare("debug") == 0) 
    {
      PlanModule::CLIENT.DEBUG = true;
    } 
    else 
    {
      replace(v.begin(), v.end(), ',', ' ');
      iostring << v << '\n';
    }
  }

  try {
    PlayerClient client("localhost");
    Position2dProxy p2dProxy(&client, 0);
    LaserProxy laserProxy(&client, 0);

    PlanModule::CLIENT.channel = &iostring;
    PlanModule::CLIENT.robot = &client;
    ActModule::LOCALIZATION.position2d = &p2dProxy;
    ActModule::SONAR.laser = &laserProxy;

    p2dProxy.SetMotorEnable(true);
    while (!PlanModule::GetInstance().IsDone())
    {
      PlanModule::GetInstance().Run();
      ActModule::GetInstance().Run();
    }
    
    LINFO << "Robot navigation completed";
  }
  catch (exception& e)
  {
    cerr << e.what();
    return -1;
  }
  
  return 0;
}
