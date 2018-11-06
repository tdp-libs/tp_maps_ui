#include "tp_maps_ui/widgets/FPSLabel.h"

#include "tp_utils/TimeUtils.h"

#include <queue>

namespace tp_maps_ui
{

//##################################################################################################
struct FPSLabel::Private
{
  std::queue<int64_t> frameTimes;
  int64_t lastUpdate{0};
};

//##################################################################################################
FPSLabel::FPSLabel(Widget* parent):
  Label(parent),
  d(new Private())
{

}

//##################################################################################################
FPSLabel::~FPSLabel()
{
  delete d;
}

//##################################################################################################
void FPSLabel::render(tp_maps::RenderInfo& renderInfo)
{
  auto now = tp_utils::currentTimeMS();

  d->frameTimes.push(now);
  if(d->frameTimes.size()>20)
    d->frameTimes.pop();

  if((now - d->lastUpdate) >= 1000)
  {
    d->lastUpdate = now;
    double first = double(tp_utils::currentTimeMS() - d->frameTimes.front())/1000.0;

    double fps = d->frameTimes.size();
    if(first>0.01)
      fps = fps / first;

    std::string str = "FPS: " + std::to_string(fps);
    setText(tpFromUTF8(str));
  }

  Label::render(renderInfo);
}

}
