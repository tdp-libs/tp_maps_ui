#include "tp_maps_ui/widgets/CollapsiblePanel.h"
#include "tp_maps_ui/widgets/PushButton.h"
#include "tp_maps_ui/layouts/FixedLayout.h"
#include "tp_maps_ui/layouts/SubWindowLayout.h"
#include "tp_maps_ui/layers/UILayer.h"

#include "tp_utils/DebugUtils.h"

#include <memory>

namespace tp_maps_ui
{

namespace
{
//##################################################################################################
struct PageDetails_lt
{
  Widget* page{nullptr};
  PushButton* button{nullptr};

  std::unique_ptr<tp_utils::Callback<void()>> buttonClicked;

  float fraction{0.0f};
  float target{0.0f};
};
}

//##################################################################################################
struct CollapsiblePanel::Private
{
  TP_REF_COUNT_OBJECTS("tp_maps_ui::CollapsiblePanel::Private");
  TP_NONCOPYABLE(Private);

  CollapsiblePanel* q;
  std::vector<PageDetails_lt*> pages;
  FixedLayout* layout{nullptr};

  double lastAnimationMS{0.0};

  //################################################################################################
  Private(CollapsiblePanel* q_):
    q(q_)
  {

  }

  //################################################################################################
  ~Private()
  {
    tpDeleteAll(pages);
  }

  //################################################################################################
  void updateGeometry()
  {
    const float buttonHeight=24.0f;
    float total=0.0f;

    auto h = q->height();

    for(auto page : pages)
    {
      layout->updateWidgetDims(page->button, Dim::zero(), Dim::pixels(total), Dim::full(), Dim::pixels(buttonHeight));
      total += buttonHeight;

      float hh = page->page->sizeHint().second.calc(h)*page->fraction;
      layout->updateWidgetDims(page->page, Dim::zero(), Dim::pixels(total), Dim::full(), Dim::pixels(hh));
      total += hh;
    }
  }

  //################################################################################################
  void animate()
  {
    lastAnimationMS=0.0;
    q->setCurrentAnimation([&](double timeMS)
    {
      bool workDone=false;

      if(lastAnimationMS<1.0)
        lastAnimationMS = timeMS;

      float speed=float(timeMS-lastAnimationMS) / 200.0f;

      lastAnimationMS = timeMS;

      for(auto page : pages)
      {
        if(std::fabs(page->target - page->fraction)>0.00001f)
        {
          workDone = true;
          if(page->target > page->fraction)
            page->fraction = tpMin(page->target, page->fraction+speed);
          else
            page->fraction = tpMax(page->target, page->fraction-speed);
        }
        else
          page->fraction = page->target;
      }

      updateGeometry();
      return workDone;
    });
  }
};

//##################################################################################################
CollapsiblePanel::CollapsiblePanel(Widget* parent):
  Widget(parent),
  d(new Private(this))
{
  d->layout = new FixedLayout(this);
}

//##################################################################################################
CollapsiblePanel::~CollapsiblePanel()
{
  delete d;
}

//##################################################################################################
void CollapsiblePanel::addPage(Widget* page, const std::u16string& label)
{
  auto pageDetails = new PageDetails_lt();
  pageDetails->page = new Widget();
  (new SubWindowLayout(pageDetails->page))->addWidget(page);
  pageDetails->button = new PushButton(label);

  d->layout->addWidget(pageDetails->button);
  d->layout->addWidget(pageDetails->page);

  d->pages.push_back(pageDetails);

  pageDetails->buttonClicked.reset(new tp_utils::Callback<void()>([this, pageDetails]()
  {
    pageDetails->target = (pageDetails->target>0.5f)?0.0f:1.0f;
    for(auto page : d->pages)
      if(page!=pageDetails)
        page->target = 0.0f;

    d->animate();
    d->updateGeometry();
  }));
  pageDetails->buttonClicked->connect(pageDetails->button->clicked);

  d->animate();
  d->updateGeometry();
}

//##################################################################################################
void CollapsiblePanel::setGeometry(float x, float y, float w, float h)
{
  Widget::setGeometry(x, y, w, h);
  d->updateGeometry();
}

}
