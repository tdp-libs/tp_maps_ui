#include "tp_maps_ui/widgets/TreeWidget.h"

namespace tp_maps_ui
{

//##################################################################################################
struct TreeWidget::Private
{
  TP_REF_COUNT_OBJECTS("tp_maps_ui::TreeWidget::Private");
  TP_NONCOPYABLE(Private);
  Private() = default;
};

//##################################################################################################
TreeWidget::TreeWidget(Widget* parent):
    Widget(parent),
    d(new Private())
{

}

//##################################################################################################
TreeWidget::~TreeWidget()
{
  delete d;
}

//##################################################################################################
void TreeWidget::render(tp_maps::RenderInfo& renderInfo)
{
  Widget::render(renderInfo);
}

//##################################################################################################
void TreeWidget::invalidateBuffers()
{
  Widget::invalidateBuffers();
}

//##################################################################################################
bool TreeWidget::mouseEvent(const tp_maps::MouseEvent& event)
{
  return Widget::mouseEvent(event);
}

//##################################################################################################
void TreeWidget::animate(double timestampMS)
{
  Widget::animate(timestampMS);
}

}
