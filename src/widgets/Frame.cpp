#include "tp_maps_ui/widgets/Frame.h"
#include "tp_maps_ui/DrawHelper.h"

#include "tp_utils/TimeUtils.h"

namespace tp_maps_ui
{

//##################################################################################################
struct Frame::Private
{
  TP_REF_COUNT_OBJECTS("tp_maps_ui::Frame::Private");
  TP_NONCOPYABLE(Private);
  Private() = default;
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
  TP_FUNCTION_TIME("Frame::render");

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
