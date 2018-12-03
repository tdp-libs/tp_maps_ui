#include "tp_maps_ui/layouts/SubWindowLayout.h"
#include "tp_maps_ui/layers/UILayer.h"
#include "tp_maps_ui/Widget.h"

#include "tp_utils/DebugUtils.h"

namespace tp_maps_ui
{

//##################################################################################################
struct SubWindowLayout::Private
{

  Private()
  {

  }
};

//##################################################################################################
SubWindowLayout::SubWindowLayout(Widget* parent):
  Layout(parent),
  d(new Private())
{
  setContentsMargins(Dim::zero(), Dim::zero(), Dim::zero(), Dim::zero());
}

//##################################################################################################
SubWindowLayout::~SubWindowLayout()
{
  delete d;
}

//##################################################################################################
void SubWindowLayout::updateLayout()
{
  float parentW = parent()->width();
  float parentH = parent()->height();

  for(auto child : parent()->children())
  {
    float x = 0.0f;
    float y = 0.0f;
    float w = child->sizeHint().first.calc(parentW);
    float h = child->sizeHint().second.calc(parentH);

    child->setGeometry(x, y, w, h);
  }
}

//##################################################################################################
void SubWindowLayout::addWidget(Widget* widget)
{
  if(!parent())
  {
    tpWarning() << "SubWindowLayout::addWidget() Error! Add widgets to a layout after adding it to a widget!";
    return;
  }

  parent()->addWidget(widget);
}

//##################################################################################################
void SubWindowLayout::addLayout(Layout* layout)
{
  auto widget = new Widget();
  widget->setLayout(layout);
  addWidget(widget);
}

//##################################################################################################
std::pair<Dim, Dim> SubWindowLayout::sizeHint() const
{
  const auto& c=parent()->children();
  return c.empty()?Layout::sizeHint():c.front()->sizeHint();
}

}
