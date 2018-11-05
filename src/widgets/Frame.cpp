#include "tp_maps_ui/widgets/Frame.h"
#include "tp_maps_ui/layers/UILayer.h"
#include "tp_maps_ui/DrawHelper.h"

namespace tp_maps_ui
{

//##################################################################################################
struct Frame::Private
{

};

//##################################################################################################
Frame::Frame(Widget* parent):
  Widget(parent),
  d(new Private())
{

}

//##################################################################################################
Frame::~Frame()
{
  delete d;
}

//##################################################################################################
void Frame::render(tp_maps::RenderInfo& renderInfo)
{
  TP_UNUSED(renderInfo);

  if(!drawHelper())
    return;

  auto m = matrix();

  //Draw the frame background.
  drawHelper()->drawBox(m, width(), height(), BoxType::Flat, FillType::Panel, VisualModifier::Normal);
}

//##################################################################################################
void Frame::invalidateBuffers()
{
  Widget::invalidateBuffers();
}

}
