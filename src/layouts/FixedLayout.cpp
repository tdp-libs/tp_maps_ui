#include "tp_maps_ui/layouts/FixedLayout.h"
#include "tp_maps_ui/layers/UILayer.h"
#include "tp_maps_ui/Widget.h"

#include "tp_maps/Map.h"

#include "tp_utils/DebugUtils.h"

namespace tp_maps_ui
{

namespace
{
//##################################################################################################
class LayoutParams_lt : public LayoutParams
{
public:
  Dim x;
  Dim y;
  Dim width;
  Dim height;

  LayoutParams_lt(const Dim& x_=Dim::zero(),
                  const Dim& y_=Dim::zero(),
                  const Dim& width_=Dim::full(),
                  const Dim& height_=Dim::full()):
    x(x_),
    y(y_),
    width(width_),
    height(height_)
  {

  }
};
}

//##################################################################################################
struct FixedLayout::Private
{

  Private()
  {

  }
};

//##################################################################################################
FixedLayout::FixedLayout(Widget* parent):
  Layout(parent),
  d(new Private())
{

}

//##################################################################################################
FixedLayout::~FixedLayout()
{
  delete d;
}

//##################################################################################################
void FixedLayout::updateLayout()
{
  float parentW = parent()->width();
  float parentH = parent()->height();

  for(auto child : parent()->children())
  {
    auto params = child->layoutParams<LayoutParams_lt>();

    float x=params->x     .calc(parentW);
    float y=params->y     .calc(parentH);
    float w=params->width .calc(parentW);
    float h=params->height.calc(parentH);

    child->setGeometry(x, y, w, h);
  }
}

//##################################################################################################
void FixedLayout::addWidget(Widget* widget, Dim x, Dim y, Dim width, Dim height)
{
  if(!parent())
  {
    tpWarning() << "FixedLayout::addWidget() Error! Add widgets to a layout after adding it to a widget!";
    return;
  }

  widget->layoutParams<LayoutParams_lt>(x, y, width, height);
  parent()->addWidget(widget);
}

//##################################################################################################
void FixedLayout::addLayout(Layout* layout, Dim x, Dim y, Dim width, Dim height)
{
  auto widget = new Widget();
  widget->setLayout(layout);
  addWidget(widget, x, y, width, height);
}

//##################################################################################################
void FixedLayout::updateWidgetDims(Widget* widget, Dim x, Dim y, Dim width, Dim height)
{
  auto dims = widget->layoutParams<LayoutParams_lt>(x, y, width, height);
  dims->x = x;
  dims->y = y;
  dims->width = width;
  dims->height = height;
  updateLayout();
}

//##################################################################################################
void FixedLayout::getWidgetDims(Widget* widget, Dim& x, Dim& y, Dim& width, Dim& height)
{
  auto params = widget->layoutParams<LayoutParams_lt>();
  x = params->x;
  y = params->y;
  width = params->width;
  height = params->height;
}

}
