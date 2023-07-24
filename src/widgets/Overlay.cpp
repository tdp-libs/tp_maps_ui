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
  TP_REF_COUNT_OBJECTS("tp_maps_ui::Overlay::Private");
  TP_NONCOPYABLE(Private);
  Private() = default;

  //This represents the button state, normal, hover, presses, etc...
  VisualModifier currentVisualModifier{VisualModifier::Normal};
  float fade{1.0f};
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
void Overlay::setFade(float fade)
{
  d->fade = fade;
  update();
}

//##################################################################################################
float Overlay::fade() const
{
  return d->fade;
}

//##################################################################################################
void Overlay::fadeAnimation(float fadeTo, float speed)
{
  setVisible(true);
  setCurrentAnimation([&, lastTimeMS=-1.0, fadeTo, speed](double timeMS) mutable -> bool
  {
    auto delta = timeMS-lastTimeMS;
    if(lastTimeMS<0.0)
    {
      lastTimeMS = timeMS;
      return true;
    }

    if(delta<0.5)
      return false;

    auto s = speed * (float(delta)/1000.0f);

    if(s<0.000001f)
      return false;

    float f = fade();
    auto cont = calculateAnimationValue(fadeTo, f, s);
    setFade(f);
    setVisible(cont || (fadeTo>0.01f));
    return cont;
  });
}

//##################################################################################################
void Overlay::render(tp_maps::RenderInfo& renderInfo)
{
  TP_UNUSED(renderInfo);

  if(!drawHelper())
    return;

  auto m = matrix();

  drawHelper()->drawOverlay(m, width(), height(), d->fade);
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

  case tp_maps::MouseEventType::DragStart:
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
