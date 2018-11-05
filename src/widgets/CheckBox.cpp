#include "tp_maps_ui/widgets/CheckBox.h"
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
struct CheckBox::Private
{
  std::u16string text;
  std::unique_ptr<tp_maps::FontShader::PreparedString> preparedString;

  //This represents the button state, normal, hover, presses, etc...
  VisualModifier currentVisualModifier{VisualModifier::Normal};

  CheckState checkState{CheckState::Unchecked};

  bool tristate{false};
  bool regenerateText{true};

  //################################################################################################
  Private(const std::u16string& text_=std::u16string()):
    text(text_)
  {

  }

  //################################################################################################
  void updateUI()
  {

  }
};

//##################################################################################################
CheckBox::CheckBox(Widget* parent):
    Widget(parent),
    d(new Private())
{

}

//##################################################################################################
CheckBox::CheckBox(const std::u16string& text, Widget* parent):
  Widget(parent),
  d(new Private(text))
{

}

//##################################################################################################
CheckBox::~CheckBox()
{
  delete d;
}

//##################################################################################################
const std::u16string& CheckBox::text() const
{
  return d->text;
}

//##################################################################################################
void CheckBox::setText(const std::u16string& text)
{
  d->text = text;
  d->regenerateText = true;
  update();
}

//##################################################################################################
bool CheckBox::checked() const
{
  return (d->checkState==CheckState::Checked);
}

//##################################################################################################
void CheckBox::setChecked(bool checked)
{
  d->checkState = checked?CheckState::Checked:CheckState::Unchecked;
  d->updateUI();
}

//##################################################################################################
CheckState CheckBox::checkState() const
{
  return d->checkState;
}

//##################################################################################################
void CheckBox::setCheckstate(CheckState checkState)
{
  d->checkState = checkState;

  if(!d->tristate && d->checkState==CheckState::PartiallyChecked)
    d->checkState = CheckState::Unchecked;

  d->updateUI();
}

//##################################################################################################
bool CheckBox::tristate() const
{
  return d->tristate;
}

//##################################################################################################
void CheckBox::setTristate(bool tristate)
{
  d->tristate = tristate;
  d->updateUI();
}

//##################################################################################################
void CheckBox::render(tp_maps::RenderInfo& renderInfo)
{
  TP_UNUSED(renderInfo);

  auto m = matrix();

  //Draw the checkbox.
  if(d->checkState==CheckState::Checked && drawHelper())
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
void CheckBox::invalidateBuffers()
{
  Widget::invalidateBuffers();
}

//##################################################################################################
bool CheckBox::mouseEvent(const tp_maps::MouseEvent& event)
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

      d->checkState = (d->checkState==CheckState::Checked)?CheckState::Unchecked:CheckState::Checked;
      checkStateChanged(d->checkState);
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
void CheckBox::animate(double timestampMS)
{
  Widget::animate(timestampMS);
}

}
