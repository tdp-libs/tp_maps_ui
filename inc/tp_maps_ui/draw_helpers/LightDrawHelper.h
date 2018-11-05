#ifndef tp_maps_ui_LightDrawHelper_h
#define tp_maps_ui_LightDrawHelper_h

#include "tp_maps_ui/draw_helpers/ThemeDrawHelper.h"

#include "glm/glm.hpp"

namespace tp_maps_ui
{

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT LightDrawHelper : public ThemeDrawHelper
{
public:
  //################################################################################################
  LightDrawHelper(UILayer* layer);

  //################################################################################################
  static ThemeParameters params();
};

}

#endif

