#include "tp_maps_ui/draw_helpers/DarkDrawHelper.h"

namespace tp_maps_ui
{

//##################################################################################################
DarkDrawHelper::DarkDrawHelper(UILayer* layer):
  ThemeDrawHelper(layer, params())
{

}

//##################################################################################################
ThemeParameters DarkDrawHelper::params()
{
  ThemeParameters params;

  //A panel
  {
    params.normalPanelFrame.l = {186,189,182,255};
    params.normalPanelFrame.r = {120,120,120,255};
    params.normalPanelFrame.t = {186,189,182,255};
    params.normalPanelFrame.b = {120,120,120,255};

    params.normalPanelFrame.c = {146,148,143,255};

    params.normalPanelFrame.tl = {192,192,192,255};
    params.normalPanelFrame.tr = {136,138,133,255};
    params.normalPanelFrame.bl = {136,138,133,255};
    params.normalPanelFrame.br = {120,120,120,255};
  }

  //A normal raised button
  {
    params.raisedButtonFrame.l = {186,189,182,255};
    params.raisedButtonFrame.r = {120,120,120,255};
    params.raisedButtonFrame.t = {186,189,182,255};
    params.raisedButtonFrame.b = {120,120,120,255};

    params.raisedButtonFrame.c = {146,148,143,255};

    params.raisedButtonFrame.tl = {192,192,192,255};
    params.raisedButtonFrame.tr = {136,138,133,255};
    params.raisedButtonFrame.bl = {136,138,133,255};
    params.raisedButtonFrame.br = {120,120,120,255};
  }

  //A pressed button
  {
    params.sunkenButtonFrame.l = {120,120,120,255};
    params.sunkenButtonFrame.r = {186,189,182,255};
    params.sunkenButtonFrame.t = {120,120,120,255};
    params.sunkenButtonFrame.b = {186,189,182,255};

    params.sunkenButtonFrame.c = {136,138,133,255};

    params.sunkenButtonFrame.tl = {120,120,120,255};
    params.sunkenButtonFrame.tr = {136,138,133,255};
    params.sunkenButtonFrame.bl = {136,138,133,255};
    params.sunkenButtonFrame.br = {192,192,192,255};
  }

  return params;
}

}
