#include "tp_maps_ui/Widget.h"
#include "tp_maps_ui/Layout.h"
#include "tp_maps_ui/layers/UILayer.h"
#include "tp_maps_ui/widgets/PushButton.h"

#include "tp_maps/Map.h"
#include "tp_maps/RenderInfo.h"
#include "tp_maps/MouseEvent.h"

#include "tp_utils/DebugUtils.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace tp_maps_ui
{
//##################################################################################################
struct Widget::Private
{
  TP_REF_COUNT_OBJECTS("tp_maps_ui::Widget::Private");
  TP_NONCOPYABLE(Private);

  Widget* parent;
  UILayer* layer{nullptr};
  Layout* layout{nullptr};
  tp_maps::FontRenderer* font{nullptr};
  DrawHelper* drawHelper{nullptr};
  LayoutParams* layoutParams{nullptr};

  std::function<bool(double)> animation;

  std::vector<Widget*> children;

  glm::mat4 matrix{1.0f};

  float x     {1.0f};
  float y     {1.0f};
  float width {1.0f};
  float height{1.0f};

  float screenX     {1.0f};
  float screenY     {1.0f};
  float screenWidth {1.0f};
  float screenHeight{1.0f};

  GLint scissorX{1};
  GLint scissorY{1};
  GLint scissorW{1};
  GLint scissorH{1};

  bool visible{true};
  bool transparentToMouseEvents{false};
  bool hasFocus{false};
  bool tabFocus{false};

  bool calculateGeometry{false};

  //################################################################################################
  Private(Widget* parent_):
    parent(parent_)
  {

  }

  //################################################################################################
  void updateLayout()
  {
    calculateGeometry = true;
    if(layer)
      layer->update();
  }

  //################################################################################################
  void updateGeometry()
  {
    if(calculateGeometry)
    {
      calculateGeometry = false;
      if(layout)
        layout->updateLayout();

      if(parent)
      {
        matrix = parent->matrix();
        matrix = glm::translate(matrix, {x, y, 0.0f});
        screenX = parent->screenX() + x;
        screenY = parent->screenY() + y;
        screenWidth  = tpMin(tpMax(0.0f, parent->width()  - x), width );
        screenHeight = tpMin(tpMax(0.0f, parent->height() - y), height);
      }
      else
      {
        matrix = glm::mat4(1.0f);
        matrix = glm::translate(matrix, {-1.0f, 1.0f, 0.0f});
        matrix = glm::scale(matrix, {2.0f/width, -2.0f/height, 1.0f});

        screenX = x;
        screenY = y;
        screenWidth  = width;
        screenHeight = height;
      }

      scissorX = GLint(std::lround(screenX));
      scissorY = GLint(layer->map()->height()) - (GLint(std::lround(screenY+screenHeight)));
      scissorW = GLint(std::lround(screenWidth));
      scissorH = GLint(std::lround(screenHeight));

      if(parent)
      {
        GLint parentScissorX = parent->d->scissorX;
        GLint parentScissorY = parent->d->scissorY;
        GLint parentScissorW = parent->d->scissorW;
        GLint parentScissorH = parent->d->scissorH;

        if(scissorX<parentScissorX)
        {
          scissorW = tpMax(0, scissorW-(parentScissorX-scissorX));
          scissorX=parentScissorX;
        }

        if(scissorY<parentScissorY)
        {
          scissorH = tpMax(0, scissorH-(parentScissorY-scissorY));
          scissorY=parentScissorY;
        }

        auto projScissorW = scissorW + scissorX;
        auto projScissorH = scissorH + scissorY;

        auto projParentScissorW = parentScissorW + parentScissorX;
        auto projParentScissorH = parentScissorH + parentScissorY;

        if(projScissorW>projParentScissorW)
          scissorW = tpMax(0, scissorW-(projScissorW-projParentScissorW));

        if(projScissorH>projParentScissorH)
          scissorH = tpMax(0, scissorH-(projScissorH-projParentScissorH));
      }
    }
  }
};

//##################################################################################################
LayoutParams::~LayoutParams()
{

}

//##################################################################################################
Widget::Widget(Widget* parent):
  d(new Private(parent))
{
  if(d->parent)
    d->parent->addWidget(this);
}

//##################################################################################################
Widget::~Widget()
{
  while(!d->children.empty())
    delete d->children.back();

  delete d->layout;

  if(d->parent)
    tpRemoveOne(d->parent->d->children, this);

  delete d->layoutParams;

  delete d;
}

//##################################################################################################
UILayer* Widget::layer() const
{
  return d->layer;
}

//##################################################################################################
Widget* Widget::parent() const
{
  return d->parent;
}

//##################################################################################################
const std::vector<Widget*>& Widget::children() const
{
  return d->children;
}

//##################################################################################################
void Widget::addWidget(Widget* child, int index)
{
  child->setLayer(d->layer);
  child->d->parent = this;

  if(index<0)
    d->children.push_back(child);
  else
    d->children.insert(d->children.begin()+tpBound(0, index, int(d->children.size())), child);
  d->updateLayout();
}

//##################################################################################################
void Widget::reinsertWidget(Widget* child, int index)
{
  tpRemoveOne(d->children, child);
  d->children.insert(d->children.begin()+tpBound(0, index, int(d->children.size())), child);
  d->updateLayout();
}

//##################################################################################################
void Widget::setLayout(Layout* layout)
{
  if(d->layout == layout)
    return;

  if(d->layout)
    delete d->layout;

  d->layout = layout;

  if(d->layout)
    d->layout->setParent(this);

  d->updateLayout();
}

//##################################################################################################
Layout* Widget::layout() const
{
  return d->layout;
}

//##################################################################################################
void Widget::setGeometry(float x, float y, float w, float h)
{
  d->x = x;
  d->y = y;
  d->width  = w;
  d->height = h;
  d->updateLayout();
}

//##################################################################################################
float Widget::x() const
{
  return d->x;
}

//##################################################################################################
float Widget::y() const
{
  return d->y;
}

//##################################################################################################
float Widget::width() const
{
  return d->width;
}

//##################################################################################################
float Widget::height() const
{
  return d->height;
}

//################################################################################################
float Widget::screenX() const
{
  return d->screenX;
}

//################################################################################################
float Widget::screenY() const
{
  return d->screenY;
}

//################################################################################################
float Widget::screenWidth() const
{
  return d->screenWidth;
}

//################################################################################################
float Widget::screenHeight() const
{
  return d->screenHeight;
}

//##################################################################################################
glm::mat4 Widget::matrix() const
{
  return d->matrix;
}

//##################################################################################################
void Widget::setFont(tp_maps::FontRenderer* font)
{
  d->font = font;
}

//##################################################################################################
tp_maps::FontRenderer* Widget::font() const
{
  if(!d->font)
  {
    if(d->parent)
      return d->parent->font();
    else if(d->layer)
      return d->layer->font();
  }

  return d->font;
}

//##################################################################################################
void Widget::setDrawHelper(DrawHelper* drawHelper)
{
  d->drawHelper = drawHelper;
}

//##################################################################################################
DrawHelper* Widget::drawHelper() const
{
  if(!d->drawHelper)
  {
    if(d->parent)
      return d->parent->drawHelper();
    else if(d->layer)
      return d->layer->drawHelper();
  }

  return d->drawHelper;
}

//##################################################################################################
void Widget::setVisible(bool visible)
{
  if(d->visible != visible)
  {
    d->visible = visible;
    update();
  }
}

//##################################################################################################
bool Widget::visible() const
{
  return d->visible;
}

//################################################################################################
void Widget::setTransparentToMouseEvents(bool transparentToMouseEvents)
{
  d->transparentToMouseEvents = transparentToMouseEvents;
}

//################################################################################################
bool Widget::transparentToMouseEvents() const
{
  return d->transparentToMouseEvents;
}

//##################################################################################################
std::pair<Dim, Dim> Widget::sizeHint() const
{
  if(d->layout)
    return d->layout->sizeHint();

  return {{0.0f, 1.0f}, {32.0f, 0.0f}};
}

//##################################################################################################
void Widget::setCurrentAnimation(const std::function<bool(double)>& animation)
{
  d->animation = animation;
}
//##################################################################################################
void Widget::focus()
{
  if(d->layer)
    d->layer->focus(this);
}

//##################################################################################################
bool Widget::hasFocus() const
{
  return d->hasFocus;
}

//##################################################################################################
bool Widget::tabFocus() const
{
  return d->tabFocus;
}

//##################################################################################################
void Widget::setTabFocus(bool tabFocus)
{
  d->tabFocus = tabFocus;
}

//##################################################################################################
LayoutParams* Widget::layoutParams()
{
  return d->layoutParams;
}

//##################################################################################################
void Widget::setLayoutParams(LayoutParams* layoutParams)
{
  delete d->layoutParams;
  d->layoutParams = layoutParams;
}

//##################################################################################################
void Widget::render(tp_maps::RenderInfo& renderInfo)
{
  TP_UNUSED(renderInfo);
}

//##################################################################################################
void Widget::invalidateBuffers()
{

}

//##################################################################################################
bool Widget::mouseEvent(const tp_maps::MouseEvent& event)
{
  if(event.type == tp_maps::MouseEventType::Press)
  {
    focus();
    return true;
  }

  return false;
}

//##################################################################################################
bool Widget::keyEvent(const tp_maps::KeyEvent& event)
{
  TP_UNUSED(event);
  return false;
}

//##################################################################################################
bool Widget::textEditingEvent(const tp_maps::TextEditingEvent& event)
{
  TP_UNUSED(event);
  return true;
}

//##################################################################################################
bool Widget::textInputEvent(const tp_maps::TextInputEvent& event)
{
  TP_UNUSED(event);
  return true;
}

//##################################################################################################
void Widget::focusEvent(Widget* focusedWidget)
{
  d->hasFocus = (focusedWidget == this);
  for(auto child : d->children)
    child->focusEvent(focusedWidget);
}

//##################################################################################################
void Widget::animate(double timestampMS)
{
  TP_UNUSED(timestampMS);
}

//##################################################################################################
void Widget::update()
{
  if(d->layer)
    d->layer->update();
}

//##################################################################################################
void Widget::startTextInput()
{
  if(d->layer)
    d->layer->startTextInput(this);
}

//##################################################################################################
void Widget::stopTextInput()
{
  if(d->layer)
    d->layer->stopTextInput(this);
}

//##################################################################################################
void Widget::renderInternal(tp_maps::RenderInfo& renderInfo)
{
  if(!d->visible)
    return;

  auto s=[s=layer()->map()->pixelScale()](GLint i)
  {
    return GLint(float(i)*s+0.5f);
  };

  d->updateGeometry();
  glScissor(s(d->scissorX), s(d->scissorY), s(d->scissorW), s(d->scissorH));
  render(renderInfo);

  for(auto child : d->children)
    child->renderInternal(renderInfo);
}

//##################################################################################################
void Widget::invalidateBuffersInternal()
{
  invalidateBuffers();
  for(auto child : d->children)
    child->invalidateBuffersInternal();
}

//##################################################################################################
bool Widget::mouseEventInternal(const tp_maps::MouseEvent& event)
{
  if(!d->visible)
    return false;

  d->updateGeometry();

  if((float(event.pos.x)+0.001f) < d->screenX                  ||
     (float(event.pos.y)+0.001f) < d->screenY                  ||
     (float(event.pos.x)-0.001f) > (d->screenX+d->screenWidth) ||
     (float(event.pos.y)-0.001f) > (d->screenY+d->screenHeight))
    return false;

  for(auto i=d->children.rbegin(); i!=d->children.rend(); ++i)
    if((*i)->mouseEventInternal(event))
      return true;

  if(d->transparentToMouseEvents)
    return false;

  return mouseEvent(event);
}

//##################################################################################################
bool Widget::keyEventInternal(const tp_maps::KeyEvent& event)
{
  if(d->hasFocus)
    return keyEvent(event);

  for(auto i=d->children.rbegin(); i!=d->children.rend(); ++i)
    if((*i)->keyEventInternal(event))
      return true;

  return false;
}
//##################################################################################################
bool Widget::tabNext()
{
  bool focusFound=false;
  for(auto i : d->children)
  {
    if(focusFound)
    {
      if(i->tabFocus())
      {
        i->focus();
        return true;
      }
    }
    else
    {
      if(i->hasFocus())
      {
        focusFound = true;
      }
    }
  }

  if(!focusFound)
  {
    for(auto i=d->children.rbegin(); i!=d->children.rend(); ++i)
      if((*i)->tabNext())
        return true;
  }

  return false;
}

//##################################################################################################
bool Widget::textEditingEventInternal(const tp_maps::TextEditingEvent& event)
{
  if(d->hasFocus)
    return textEditingEvent(event);

  for(auto i=d->children.rbegin(); i!=d->children.rend(); ++i)
    if((*i)->textEditingEventInternal(event))
      return true;

  return false;
}

//##################################################################################################
bool Widget::textInputEventInternal(const tp_maps::TextInputEvent& event)
{
  if(d->hasFocus)
    return textInputEvent(event);

  for(auto i=d->children.rbegin(); i!=d->children.rend(); ++i)
    if((*i)->textInputEventInternal(event))
      return true;

  return false;
}

//##################################################################################################
void Widget::animateInternal(double timestampMS)
{
  if(!d->visible)
    return;

  d->updateGeometry();

  if(d->animation)
    if(!d->animation(timestampMS))
      d->animation = std::function<bool(double)>();

  animate(timestampMS);
  for(auto child : d->children)
    child->animateInternal(timestampMS);
}

//##################################################################################################
void Widget::setLayer(UILayer* layer)
{
  d->layer = layer;
  for(auto child : d->children)
    child->setLayer(layer);
}

//##################################################################################################
Spacing::Spacing(const Dim& spacing, Widget* parent):
  Widget(parent),
  m_spacing(spacing)
{

}

//##################################################################################################
std::pair<Dim, Dim> Spacing::sizeHint() const
{
  return {m_spacing, m_spacing};
}
}
