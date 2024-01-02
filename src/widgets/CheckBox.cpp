#include "tp_maps_ui/widgets/CheckBox.h"
#include "tp_maps_ui/layers/UILayer.h"
#include "tp_maps_ui/DrawHelper.h"

#include "tp_maps/Map.h"
#include "tp_maps/MouseEvent.h"
#include "tp_maps/Font.h"
#include "tp_maps/FontRenderer.h"
#include "tp_maps/shaders/FontShader.h"

#include "tp_utils/TimeUtils.h"

#include "glm/gtx/transform.hpp" // IWYU pragma: keep

#include <memory>

namespace tp_maps_ui
{

//##################################################################################################
struct CheckBox::Private
{
  TP_REF_COUNT_OBJECTS("tp_maps_ui::CheckBox::Private");
  TP_NONCOPYABLE(Private);

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
  update();
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

  update();
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
  update();
}

//################################################################################################
std::pair<Dim, Dim> CheckBox::sizeHint() const
{
  auto f = font()?font()->font():nullptr;

  if(!f)
    return Widget::sizeHint();

  return {{0.0f, 1.0f}, {f->lineHeight()+8.0f, 0.0f}};
}

//##################################################################################################
void CheckBox::render(tp_maps::RenderInfo& renderInfo)
{
  TP_FUNCTION_TIME("CheckBox::render");

  TP_UNUSED(renderInfo);

  if(!drawHelper())
    return;

  auto m = matrix();

  //Draw the checkbox.
  auto visualModifier = (d->checkState==CheckState::Checked)?VisualModifier::Checked:VisualModifier::Unchecked;
  drawHelper()->drawBox(m, width(), height(), BoxType::Raised, FillType::CheckBox, visualModifier);

  //Draw the text.
  if(font())
  {
    auto shader = layer()->map()->getShader<tp_maps::FontShader>();
    if(shader->error())
      return;

    if(d->regenerateText)
    {
      d->regenerateText = false;
      tp_maps::PreparedStringConfig config;
      config.topDown = true;
      d->preparedString.reset(new tp_maps::FontShader::PreparedString(font(), d->text, config));
    }

    if(!d->preparedString)
      return;

    auto color = drawHelper()->textColor(BoxType::Raised, FillType::CheckBox, visualModifier);

    shader->use(renderInfo.shaderType());
    shader->setMatrix(glm::translate(m, glm::floor(glm::vec3(width()/2.0f, height()/1.55f, 0.0f))));
    shader->setColor(color);
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

  case tp_maps::MouseEventType::DragStart:
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
