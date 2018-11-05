#include "tp_maps_ui/widgets/Label.h"

namespace tp_maps_ui
{

//##################################################################################################
struct Label::Private
{

};

//##################################################################################################
Label::Label(Widget* parent):
    Widget(parent),
    d(new Private())
{

}

//##################################################################################################
Label::~Label()
{
  delete d;
}

//##################################################################################################
void Label::render(tp_maps::RenderInfo& renderInfo)
{
  Widget::render(renderInfo);
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
