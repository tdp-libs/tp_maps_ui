#include "tp_maps_ui/Layout.h"
#include "tp_maps_ui/Widget.h"

namespace tp_maps_ui
{

//##################################################################################################
struct Layout::Private
{
  Widget* parent{nullptr};

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
}

}
