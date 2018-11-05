#include "tp_maps_ui/widgets/PushButton.h"
#include "tp_maps_ui/layers/UILayer.h"
#include "tp_maps_ui/DrawHelper.h"

#include "tp_maps/Map.h"
#include "tp_maps/MouseEvent.h"
#include "tp_maps/shaders/FontShader.h"

#include "glm/gtx/transform.hpp"

#include <memory>

namespace tp_maps_ui
{

//##################################################################################################
struct PushButton::Private
{
  std::u16string text;
  std::unique_ptr<tp_maps::FontShader::PreparedString> preparedString;

  //This represents the button state, normal, hover, presses, etc...
  VisualModifier currentVisualModifier{VisualModifier::Normal};

  bool regenerateText{true};
};

//##################################################################################################
PushButton::PushButton(Widget* parent):
  Widget(parent),
  d(new Private())
{

}

//##################################################################################################
PushButton::PushButton(const std::u16string& text, Widget* parent):
  Widget(parent),
  d(new Private())
{
  setText(text);
}

//##################################################################################################
PushButton::~PushButton()
{
  delete d;
}

//##################################################################################################
const std::u16string& PushButton::text() const
{
  return d->text;
}

//##################################################################################################
void PushButton::setText(const std::u16string& text)
{
  d->text = text;
  d->regenerateText = true;
  update();
}

//##################################################################################################
void PushButton::render(tp_maps::RenderInfo& renderInfo)
{
  TP_UNUSED(renderInfo);

  auto m = matrix();

  //Draw the button background.
  if(drawHelper())
  {
    drawHelper()->drawBox(m, width(), height(), BoxType::Raised, FillType::Button, d->currentVisualModifier);
  }

  //Draw the text.
  if(font())
  {
    auto shader = layer()->map()->getShader<tp_maps::FontShader>();
    if(shader->error())
      return;

    if(d->regenerateText)
    {
      tp_maps::PreparedStringConfig config;
      config.topDown = true;
      d->preparedString.reset(new tp_maps::FontShader::PreparedString(shader, font(), d->text, config));
    }

    if(!d->preparedString)
      return;

    shader->use();
    shader->setMatrix(glm::translate(m, {width()/2.0f, height()/1.55f, 0.0f}));

    shader->drawPreparedString(*d->preparedString.get());
  }
}

//##################################################################################################
void PushButton::invalidateBuffers()
{
  Widget::invalidateBuffers();
}

//##################################################################################################
bool PushButton::mouseEvent(const tp_maps::MouseEvent& event)
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
void PushButton::animate(double timestampMS)
{
  Widget::animate(timestampMS);
}

}
