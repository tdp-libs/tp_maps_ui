#include "tp_maps_ui/Layout.h"
#include "tp_maps_ui/Widget.h"

#include "tp_utils/RefCount.h"

namespace tp_maps_ui
{

//##################################################################################################
struct Layout::Private
{
  TP_REF_COUNT_OBJECTS("tp_maps_ui::Layout::Private");
  TP_NONCOPYABLE(Private);
  Private() = default;

  Widget* parent{nullptr};
  std::vector<Widget*> orphans;

  Margins contentsMargins;
};

//##################################################################################################
Layout::Layout(Widget* parent):
  d(new Private())
{
  if(parent)
    parent->setLayout(this);
}

//##################################################################################################
Layout::~Layout()
{
  tpDeleteAll(d->orphans);
  delete d;
}

//##################################################################################################
Widget* Layout::parent() const
{
  return d->parent;
}

//################################################################################################
std::pair<Dim, Dim> Layout::sizeHint() const
{
  return {{10.0f, 0.66f}, {10.0f, 0.66f}};
}

//################################################################################################
void Layout::setContentsMargins(const Dim& left, const Dim& top, const Dim& right, const Dim& bottom)
{
  d->contentsMargins.left   = left;
  d->contentsMargins.top    = top;
  d->contentsMargins.right  = right;
  d->contentsMargins.bottom = bottom;
}

//################################################################################################
const Margins& Layout::contentsMargins() const
{
  return d->contentsMargins;
}

//##################################################################################################
void Layout::setParent(Widget* parent)
{
  d->parent = parent;
  if(d->parent)
  {
    for(auto orphan : d->orphans)
      d->parent->addWidget(orphan);
    d->orphans.clear();
  }
}
//##################################################################################################
void Layout::addChildWidget(Widget* child)
{
  if(d->parent)
    d->parent->addWidget(child);
  else
    d->orphans.push_back(child);

}

}
