#include "Libraries.h"
#include "PlanModule.h"
#include "ActModule.h"

PlanModule::Pilot::Pilot() : IModule()
{
  next = PlanModule::GetInstance().GetCurrentPos();
  LINFO << "| Pilot Module initialized";
}

void PlanModule::Pilot::Run() 
{
  if (ActModule::GetInstance().IsDone() && !channel.eof()) 
  {
    string line;
    if (getline(channel, line)) {
      istringstream is(line);
      is >> next[0];
      is >> next[1];
      LINFO << "| Moving to " << next[0] << ", " << next[1];
    }
  }
}

Vector2d PlanModule::Pilot::GetNextPos()
{
  return next;
}

bool PlanModule::Pilot::IsDone()
{
  return channel.eof() && ActModule::GetInstance().IsDone();
}