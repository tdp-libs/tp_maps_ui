#include "tp_maps_ui/widgets/Label.h"
#include "tp_maps_ui/layers/UILayer.h"
#include "tp_maps_ui/DrawHelper.h"

#include "tp_maps/Map.h"
#include "tp_maps/MouseEvent.h"
#include "tp_maps/Font.h"
#include "tp_maps/FontRenderer.h"
#include "tp_maps/shaders/FontShader.h"

#include "tp_utils/DebugUtils.h"

#include "glm/gtx/transform.hpp"

#include <memory>

namespace tp_maps_ui
{

//##################################################################################################
struct Label::Private
{
  std::u16string text;
  std::unique_ptr<tp_maps::FontShader::PreparedString> preparedString;
  HAlignment hAlignment{HAlignment::Left};

  glm::vec4 color{0.0f, 0.0f, 0.0f, 1.0f};
  bool colorSet{false};

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
void Label::setHAlignment(HAlignment hAlignment)
{
  d->hAlignment = hAlignment;
  d->regenerateText = true;
  update();
}

//##################################################################################################
void Label::setColor(const glm::vec4& color)
{
  d->color = color;
  d->colorSet = true;
  update();
}

//##################################################################################################
std::pair<Dim, Dim> Label::sizeHint() const
{
  auto f = font()?font()->font():nullptr;

  if(!f)
    return Widget::sizeHint();

  return {{0.0f, 1.0f}, {f->lineHeight()+8.0f, 0.0f}};
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
      d->regenerateText = false;
      tp_maps::PreparedStringConfig config;
      config.topDown = true;

      switch(d->hAlignment)
      {
      case HAlignment::Left:   config.relativeOffset.x =  1.0f; break;
      case HAlignment::Center: config.relativeOffset.x =  0.0f; break;
      case HAlignment::Right:  config.relativeOffset.x = -1.0f; break;
      };

      d->preparedString.reset(new tp_maps::FontShader::PreparedString(shader, font(), d->text, config));
    }

    if(!d->preparedString)
      return;

    auto color = d->colorSet?d->color:drawHelper()->textColor(BoxType::Raised, FillType::Panel, VisualModifier::Normal);

    shader->use();

    float xTrans=0.0f;
    switch(d->hAlignment)
    {
    case HAlignment::Left:   xTrans = 0.0f;         break;
    case HAlignment::Center: xTrans = width()/2.0f; break;
    case HAlignment::Right:  xTrans = width();      break;
    };

    shader->setMatrix(glm::translate(m, {xTrans, height()/1.55f, 0.0f}));
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
