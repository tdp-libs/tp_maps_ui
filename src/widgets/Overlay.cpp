#include "tp_maps_ui/widgets/Overlay.h"
#include "tp_maps_ui/layers/UILayer.h"
#include "tp_maps_ui/DrawHelper.h"

#include "tp_maps/Map.h"
#include "tp_maps/MouseEvent.h"
#include "tp_maps/shaders/FontShader.h"

#include "tp_utils/DebugUtils.h"

#include "glm/gtx/transform.hpp"

#include <memory>

namespace tp_maps_ui
{

//##################################################################################################
struct Overlay::Private
{
  //This represents the button state, normal, hover, presses, etc...
  VisualModifier currentVisualModifier{VisualModifier::Normal};
};

//##################################################################################################
Overlay::Overlay(Widget* parent):
  Widget(parent),
  d(new Private())
{

}

//##################################################################################################
Overlay::~Overlay()
{
  delete d;
}

//##################################################################################################
void Overlay::render(tp_maps::RenderInfo& renderInfo)
{
  TP_UNUSED(renderInfo);

  if(!drawHelper())
    return;

  auto m = matrix();

  drawHelper()->drawOverlay(m, width(), height(), 1.0f);
}

//##################################################################################################
void Overlay::invalidateBuffers()
{
  Widget::invalidateBuffers();
}

//##################################################################################################
bool Overlay::mouseEvent(const tp_maps::MouseEvent& event)
{
  switch(event.type)
  {
  case tp_maps::MouseEventType::Press:
  {
    d->currentVisualModifier = VisualModifier::Pressed;
    update();
    return true;
  }

  case tp_maps::MouseEventType::Move:
  {
    if(d->currentVisualModifier == VisualModifier::Pressed)
      return true;
    break;
  }

  case tp_maps::MouseEventType::Release:
  {
    if(d->currentVisualModifier == VisualModifier::Pressed)
    {
      d->currentVisualModifier = VisualModifier::Normal;
      clicked();
      update();
      return true;
    }
    break;
  }

  case tp_maps::MouseEventType::Wheel:
  {
    break;
  }

  case tp_maps::MouseEventType::DoubleClick:
  {
    break;
  }

  case tp_maps::MouseEventType::Click:
  {
    break;
  }
  }

  return Widget::mouseEvent(event);
}

//##################################################################################################
void Overlay::animate(double timestampMS)
{
  Widget::animate(timestampMS);
}

}
