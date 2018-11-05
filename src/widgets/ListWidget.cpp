#include "tp_maps_ui/widgets/ListWidget.h"

namespace tp_maps_ui
{

//##################################################################################################
struct ListWidget::Private
{

};

//##################################################################################################
ListWidget::ListWidget(Widget* parent):
    Widget(parent),
    d(new Private())
{

}

//##################################################################################################
ListWidget::~ListWidget()
{
  delete d;
}

//##################################################################################################
void ListWidget::render(tp_maps::RenderInfo& renderInfo)
{
  Widget::render(renderInfo);
}

//##################################################################################################
void ListWidget::invalidateBuffers()
{
  Widget::invalidateBuffers();
}

//##################################################################################################
bool ListWidget::mouseEvent(const tp_maps::MouseEvent& event)
{
  return Widget::mouseEvent(event);
}

//##################################################################################################
void ListWidget::animate(double timestampMS)
{
  Widget::animate(timestampMS);
}

}
