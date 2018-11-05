#include "tp_maps_ui/layouts/BoxLayout.h"
#include "tp_maps_ui/Widget.h"

#include "tp_utils/DebugUtils.h"

namespace tp_maps_ui
{

namespace
{
//##################################################################################################
class LayoutParams_lt : public LayoutParams
{
public:
  Dim fraction;
  float stretch;

  HAlignment hAlignment{HAlignment::Center};
  VAlignment vAlignment{VAlignment::Center};

  LayoutParams_lt(const Dim& fraction_=Dim::full(), float stretch_=0):
    fraction(fraction_),
    stretch(stretch_)
  {

  }
};

//##################################################################################################
struct SizeDetails_lt
{
  Widget* child{nullptr};
  LayoutParams_lt* params{nullptr};

  Dim sizeHint;

  //The calculated position and size
  float origin{1.0f};
  float size{1.0f};


  SizeDetails_lt(const Dim& sizeHint_=Dim::full()):
    sizeHint(sizeHint_)
  {

  }
};

//##################################################################################################
std::vector<SizeDetails_lt> calculateSizes(const std::vector<Widget*>& children, float size, const std::function<SizeDetails_lt(Widget*)>& getSize)
{
  std::vector<SizeDetails_lt> results;

  if(children.empty())
    return results;

  results.reserve(children.size());
  for(auto child : children)
  {
    auto params = child->layoutParams<LayoutParams_lt>();
    auto sizeDetails = getSize(child);
    sizeDetails.child = child;
    sizeDetails.params = params;
    results.push_back(sizeDetails);
  }

  float totalStretch=0.0f;
  float totalSize=0.0f;

  float meanSizePerChild =  std::floor(size/float(results.size()));
  for(const auto& sizeDetails : results)
  {
    totalStretch += sizeDetails.params->stretch;
    totalSize += sizeDetails.sizeHint.calc(meanSizePerChild);
  }

  float remainingSize = tpMax(0.0f, size-totalSize);

  float accumulated=0.0f;
  float sizePerChild = 0.0f;

  if(totalStretch<0.1f)
    sizePerChild = std::floor(remainingSize/float(results.size()));
  else
    remainingSize/=totalStretch;

  for(auto& sizeDetails : results)
  {
    sizeDetails.origin = accumulated;
    sizeDetails.size = sizeDetails.sizeHint.calc(sizePerChild) + sizePerChild + (sizeDetails.params->stretch*remainingSize);
    accumulated += sizeDetails.size;
  }

  return results;
}

}

//##################################################################################################
struct BoxLayout::Private
{
  Orientation orientation;

  Private(Orientation orientation_):
    orientation(orientation_)
  {

  }
};

//##################################################################################################
BoxLayout::BoxLayout(Orientation orientation, Widget* parent):
  Layout(parent),
  d(new Private(orientation))
{

}

//##################################################################################################
BoxLayout::~BoxLayout()
{
  delete d;
}

//##################################################################################################
void BoxLayout::updateLayout()
{
  const auto& children = parent()->children();

  float w = parent()->width();
  float h = parent()->height();

  if(d->orientation == Orientation::Vertical)
  {
    for(auto sizeDetails : calculateSizes(children, h, []( Widget* c){return SizeDetails_lt(c->sizeHint().second);}))
    {
      auto params = sizeDetails.params;
      float ww = params->fraction.calc(w);
      float origin = (params->hAlignment==HAlignment::Left)?0.0f:((params->hAlignment==HAlignment::Center)?((w-ww)/2.0f):(w-ww));
      sizeDetails.child->setGeometry(origin, sizeDetails.origin, ww, sizeDetails.size);
    }
  }
  else
  {
    for(auto sizeDetails : calculateSizes(children, h, [](Widget* c){return SizeDetails_lt(c->sizeHint().first);}))
    {
      auto params = sizeDetails.params;
      float hh = params->fraction.calc(h);
      float origin = (params->vAlignment==VAlignment::Top)?0.0f:((params->vAlignment==VAlignment::Center)?((h-hh)/2.0f):(h-hh));
      sizeDetails.child->setGeometry(sizeDetails.origin, origin, sizeDetails.size, hh);
    }
  }
}

//################################################################################################
void BoxLayout::addWidget(Widget* widget, const Dim& fraction, float stretch)
{
  if(!parent())
  {
    tpWarning() << "BoxLayout::addWidget() Error! Add widgets to a layout after adding it to a widget!";
    return;
  }
  widget->layoutParams<LayoutParams_lt>(fraction, stretch);

  parent()->addWidget(widget);
}

//##################################################################################################
void BoxLayout::addLayout(Layout* layout, const Dim& fraction, float stretch)
{
  auto widget = new Widget();
  widget->setLayout(layout);
  addWidget(widget, fraction, stretch);
}

//##################################################################################################
void BoxLayout::addStretch(float stretch)
{
  auto widget = new Widget();
  addWidget(widget, Dim::full(), stretch);
}

//##################################################################################################
void BoxLayout::addSpacing(const Dim& spacing)
{
  auto widget = new Spacing(spacing);
  addWidget(widget);
}

//##################################################################################################
VBoxLayout::VBoxLayout(Widget* parent):
  BoxLayout(Orientation::Vertical, parent)
{

}

//##################################################################################################
HBoxLayout::HBoxLayout(Widget* parent):
  BoxLayout(Orientation::Horizontal, parent)
{

}

}
