#include "../Libraries.h"
#include "../PlanModule.h"
#include "../ActModule.h"

Vector2d ActModule::GoToBehavior::GetVector() 
{
  return PlanModule::GetInstance().GetNextPos() - GetCurrentPos();
}

bool ActModule::GoToBehavior::IsDone()
{
  Vector2d v = PlanModule::GetInstance().GetNextPos();
  return 1.0 > (PlanModule::GetInstance().GetNextPos() - GetCurrentPos()).norm();
}

Vector2d ActModule::GoToBehavior::GetCurrentPos()
{
  Vector2d v(
    ActModule::LOCALIZATION.position2d->GetXPos(), 
    ActModule::LOCALIZATION.position2d->GetYPos());
  return v;
}
