#include "Libraries.h"
#include "PlanModule.h"
#include "ActModule.h"

Client PlanModule::CLIENT;

PlanModule::PlanModule() : IModule()
{
  LINFO << "Plan Module initialized";
}

void PlanModule::Run() 
{
  PlanModule::CLIENT.robot->Read();
  Navigator::GetInstance().Run();
  Pilot::GetInstance().Run();
}

bool PlanModule::IsDone() 
{
  return Navigator::GetInstance().IsDone() && Pilot::GetInstance().IsDone();
}

Vector2d PlanModule::GetNextPos()
{
  return Pilot::GetInstance().GetNextPos();
}

Vector2d PlanModule::GetCurrentPos()
{
  Vector2d v(
    ActModule::LOCALIZATION.position2d->GetXPos(), 
    ActModule::LOCALIZATION.position2d->GetYPos());
  return v;
}