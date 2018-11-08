#include "tp_maps_ui/Globals.h"

#include <cmath>

namespace tp_maps_ui
{

//##################################################################################################
bool calculateAnimationValue(float targetValue, float& currentValue, float speed)
{
  if(currentValue>targetValue)
  {
    currentValue -= speed;
    if(currentValue<targetValue)
      currentValue=targetValue;
  }
  else
  {
    currentValue += speed;
    if(currentValue>targetValue)
      currentValue=targetValue;
  }

  if(std::fabs(targetValue - currentValue)<0.000001f)
  {
    currentValue = targetValue;
    return false;
  }

  return true;
}

//##################################################################################################
bool calculateAnimationDim(const Dim& target, Dim& dim, float speedR, float speedP)
{
  bool cont=false;
  cont |= tp_maps_ui::calculateAnimationValue(target.sizeFraction, dim.sizeFraction, speedR);
  cont |= tp_maps_ui::calculateAnimationValue(target.sizePixels, dim.sizePixels, speedP);
  return cont;
}

}
