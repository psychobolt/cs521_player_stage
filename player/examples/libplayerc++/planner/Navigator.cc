#include "Libraries.h"
#include "IModule.h"
#include "PlanModule.h"

PlanModule::Navigator::Navigator() : IModule()
{
  LINFO << "| Navigator Module initialized";
}

void PlanModule::Navigator::Run() 
{
  if (PlanModule::Pilot::GetInstance().IsDone() && !IsDone())
  {
    string line;
    if (getline(*(PlanModule::CLIENT.channel), line)) {
      istringstream is(line);
      Vector2d end;
      is >> end[0] >> end[1];
      PlanModule::Pilot::GetInstance().channel.clear();
      PlanModule::Pilot::GetInstance().channel.str("");
      pathFinder.FindPath(
        PlanModule::GetInstance().GetCurrentPos(), end, 
        &(PlanModule::Pilot::GetInstance().channel));
    }
  }
}

bool PlanModule::Navigator::IsDone() 
{
  return PlanModule::CLIENT.channel->eof();
}