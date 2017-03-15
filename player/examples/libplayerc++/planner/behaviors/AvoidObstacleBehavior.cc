#include "../Libraries.h"
#include "../ActModule.h"

Vector2d ActModule::AvoidObstacleBehavior::GetVector()
{
  bool avoid = false;
  Vector2d result(0.0, 0.0);
  for (int i = 0; i < ActModule::SONAR.laser->GetCount(); i++) {
    Vector2d v = GetRangeVector(i);
    result += v;
    if (v.norm() < 0.25) {
        avoid = true;
    }
  }
  if (!avoid) {
  Vector2d null(0.0, 0.0);
  return null;
  }
  return result;
}

bool ActModule::AvoidObstacleBehavior::IsDone() 
{
  return false;
}

Vector2d ActModule::AvoidObstacleBehavior::GetRangeVector(uint32_t scanIndex)
{
  player_point_2d_t point = ActModule::SONAR.laser->GetPoint(scanIndex);
  Vector2d v(point.px, point.py);
  Rotation2D<double> rot(ActModule::LOCALIZATION.position2d->GetYaw());
  return rot * v;
}