#include "Libraries.h"
#include "ActModule.h"

Localization ActModule::LOCALIZATION;
Sonar ActModule::SONAR;

void ActModule::Run() {
  if (ActModule::LOCALIZATION.position2d != NULL)
  {
    BVector vector;
    Vector2d target(0.0, 0.0);
    for (std::vector<IBehavior*>::iterator iter = behaviors.begin(); iter != behaviors.end(); iter++) 
    {
      target += (*iter)->GetVector();
    }
    double forwardAngle = ActModule::LOCALIZATION.position2d->GetYaw();
    double targetAngle = atan2(target[1], target[0]);
    double deltaAngle = RTOD(NORMALIZE(targetAngle - forwardAngle));
    int sign = deltaAngle > 0 ? 1 : -1;
    if (abs(deltaAngle) > 10.0)
    {
      vector.magnitude = 0.1;
      vector.direction = 2.0 * sign;
    }
    else
    {
      vector.magnitude = 1.0;
      vector.direction = 0.1 * sign;
    }
    ActModule::LOCALIZATION.position2d->SetSpeed(
      vector.magnitude,
      vector.direction
    );
  }
}

bool ActModule::IsDone() 
{
  for (std::vector<IBehavior*>::iterator iter = behaviors.begin(); iter != behaviors.end(); iter++) {
    if ((*iter)->IsDone()) {
      return true;
    }
  }
  return false;
}

ActModule::ActModule() : IModule()
{
  behaviors.push_back(new GoToBehavior());
  behaviors.push_back(new AvoidObstacleBehavior());
  LINFO << "Act Module initialized";
}

BVector ActModule::GetVector()
{
  return bvector;
}
