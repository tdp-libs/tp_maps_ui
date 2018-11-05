#include "tp_maps_ui/widgets/Label.h"
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
struct Label::Private
{
  std::u16string text;
  std::unique_ptr<tp_maps::FontShader::PreparedString> preparedString;
  bool regenerateText{true};

  //################################################################################################
  Private(const std::u16string& text_=std::u16string()):
    text(text_)
  {

  }
};

//##################################################################################################
Label::Label(Widget* parent):
    Widget(parent),
    d(new Private())
{

}

//##################################################################################################
Label::Label(const std::u16string& text, Widget* parent):
  Widget(parent),
  d(new Private(text))
{

}

//##################################################################################################
Label::~Label()
{
  delete d;
}

//##################################################################################################
const std::u16string& Label::text() const
{
  return d->text;
}

//##################################################################################################
void Label::setText(const std::u16string& text)
{
  d->text = text;
  d->regenerateText = true;
  update();
}

//##################################################################################################
void Label::render(tp_maps::RenderInfo& renderInfo)
{
  TP_UNUSED(renderInfo);

  if(!drawHelper())
    return;

  auto m = matrix();

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

    auto color = drawHelper()->textColor(BoxType::Raised, FillType::Panel, VisualModifier::Normal);

    shader->use();
    shader->setMatrix(glm::translate(m, {width()/2.0f, height()/1.55f, 0.0f}));
    shader->setColor(color);
    shader->drawPreparedString(*d->preparedString.get());
  }
}

//##################################################################################################
void Label::invalidateBuffers()
{
  Widget::invalidateBuffers();
}

//##################################################################################################
bool Label::mouseEvent(const tp_maps::MouseEvent& event)
{
  return Widget::mouseEvent(event);
}

//##################################################################################################
void Label::animate(double timestampMS)
{
  Widget::animate(timestampMS);
}

}
