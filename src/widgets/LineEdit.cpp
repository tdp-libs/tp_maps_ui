#include "tp_maps_ui/widgets/LineEdit.h"
#include "tp_maps_ui/layers/UILayer.h"
#include "tp_maps_ui/DrawHelper.h"

#include "tp_maps/Map.h"
#include "tp_maps/MouseEvent.h"
#include "tp_maps/KeyEvent.h"
#include "tp_maps/shaders/FontShader.h"

#include "tp_utils/DebugUtils.h"

#include "glm/gtx/transform.hpp"

#include <memory>

namespace tp_maps_ui
{

//##################################################################################################
struct LineEdit::Private
{
  TP_REF_COUNT_OBJECTS("tp_maps_ui::LineEdit::Private");
  TP_NONCOPYABLE(Private);
  Private() = default;

  std::u16string text;
  std::unique_ptr<tp_maps::FontShader::PreparedString> preparedString;

  std::u16string placeholderText;
  std::unique_ptr<tp_maps::FontShader::PreparedString> placeholderPreparedString;

  //This represents the button state, normal, hover, presses, etc...
  VisualModifier currentVisualModifier{VisualModifier::Normal};

  bool hasTextEditing{false};
  bool regenerateText{true};
  bool regeneratePlaceholderText{true};
};

//##################################################################################################
LineEdit::LineEdit(Widget* parent):
  Widget(parent),
  d(new Private())
{

}

//##################################################################################################
LineEdit::~LineEdit()
{
  delete d;
}

//##################################################################################################
std::u16string LineEdit::text() const
{
  return d->text;
}

//##################################################################################################
void LineEdit::setText(const std::u16string& text)
{
  d->text = text;
  d->regenerateText = true;
  update();
}

//##################################################################################################
std::u16string LineEdit::placeholderText() const
{
  return d->placeholderText;
}

//##################################################################################################
void LineEdit::setPlaceholderText(const std::u16string& placeholderText)
{
  d->placeholderText = placeholderText;
  d->regeneratePlaceholderText = true;
  update();
}

//##################################################################################################
void LineEdit::render(tp_maps::RenderInfo& renderInfo)
{
  TP_UNUSED(renderInfo);

  auto m = matrix();

  //Draw the button background.
  if(drawHelper())
  {
    drawHelper()->drawBox(m, width(), height(), BoxType::Sunk, FillType::Editable, d->currentVisualModifier);
  }

  //Draw the text.
  if(font() && !d->text.empty())
  {
    auto shader = layer()->map()->getShader<tp_maps::FontShader>();
    if(shader->error())
      return;

    if(d->regenerateText)
    {
      d->regenerateText = false;
      tp_maps::PreparedStringConfig config;
      config.topDown = true;
      d->preparedString.reset(new tp_maps::FontShader::PreparedString(shader, font(), d->text, config));
    }

    if(!d->preparedString)
      return;

    auto color = drawHelper()->textColor(BoxType::Raised, FillType::Button, d->currentVisualModifier);

    shader->use();
    shader->setMatrix(glm::translate(m, {width()/2.0f, height()/1.55f, 0.0f}));
    shader->setColor(color);
    shader->drawPreparedString(*d->preparedString.get());
  }

  //Draw the placeholder text.
  if(font() && d->text.empty() && !d->placeholderText.empty())
  {
    auto shader = layer()->map()->getShader<tp_maps::FontShader>();
    if(shader->error())
      return;

    if(d->regeneratePlaceholderText)
    {
      d->regeneratePlaceholderText = false;
      tp_maps::PreparedStringConfig config;
      config.topDown = true;
      d->placeholderPreparedString.reset(new tp_maps::FontShader::PreparedString(shader, font(), d->placeholderText, config));
    }

    if(!d->placeholderPreparedString)
      return;

    auto color = drawHelper()->placeholderTextColor(BoxType::Raised, FillType::Button, d->currentVisualModifier);

    shader->use();
    shader->setMatrix(glm::translate(m, {width()/2.0f, height()/1.55f, 0.0f}));
    shader->setColor(color);
    shader->drawPreparedString(*d->placeholderPreparedString.get());
  }
}

//##################################################################################################
void LineEdit::invalidateBuffers()
{
  Widget::invalidateBuffers();
}

//##################################################################################################
bool LineEdit::keyEvent(const tp_maps::KeyEvent& event)
{
  if(event.type != tp_maps::KeyEventType::Press)
    return true;

  switch(event.scancode)
  {
  case 42: //-- Backspace --------------------------------------------------------------------------
  {
    if(!d->text.empty())
    {
      d->text.pop_back();
      d->regenerateText = true;
      update();
    }
    break;
  }

  default:
    break;
  }

  return true;
}

//##################################################################################################
bool LineEdit::textEditingEvent(const tp_maps::TextEditingEvent& event)
{
  return true;
}

//##################################################################################################
bool LineEdit::textInputEvent(const tp_maps::TextInputEvent& event)
{
  d->text += tpFromUTF8(event.text);
  d->regenerateText = true;
  update();
  return true;
}

//##################################################################################################
void LineEdit::focusEvent(Widget* focusedWidget)
{
  Widget::focusEvent(focusedWidget);

  if(d->hasTextEditing)
  {
    d->hasTextEditing = false;
    stopTextInput();
  }

  if(hasFocus())
  {
    d->hasTextEditing = true;
    startTextInput();
  }
}

//##################################################################################################
void LineEdit::animate(double timestampMS)
{
  Widget::animate(timestampMS);
}

}
