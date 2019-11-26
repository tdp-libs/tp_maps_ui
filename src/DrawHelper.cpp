#include "tp_maps_ui/DrawHelper.h"
#include "tp_maps_ui/layers/UILayer.h"

namespace tp_maps_ui
{

//##################################################################################################
struct DrawHelper::Private
{
  TP_REF_COUNT_OBJECTS("tp_maps_ui::DrawHelper::Private");
  TP_NONCOPYABLE(Private);

  UILayer* layer;

  BoxParams defaultBoxParams;
  std::unordered_map<BoxType, BoxParams> boxParams;
  bool defaultSet{false};

  //################################################################################################
  Private(UILayer* layer_):
    layer(layer_)
  {

  }
};

//##################################################################################################
DrawHelper::DrawHelper(UILayer* layer):
  d(new Private(layer))
{
  d->layer->addDrawHelper(this);
}

//##################################################################################################
DrawHelper::~DrawHelper()
{
  d->layer->removeDrawHelper(this);
  delete d;
}

//##################################################################################################
UILayer* DrawHelper::layer() const
{
  return d->layer;
}

//##################################################################################################
tp_maps::Map* DrawHelper::map() const
{
  return d->layer?d->layer->map():nullptr;
}

//##################################################################################################
const BoxParams& DrawHelper::boxParams(BoxType boxType) const
{
  auto i = d->boxParams.find(boxType);
  return (i!=d->boxParams.end())?i->second:d->defaultBoxParams;
}

//##################################################################################################
void DrawHelper::invalidateBuffers()
{

}

//##################################################################################################
void DrawHelper::setBoxParams(BoxType boxType, const BoxParams& boxParams)
{
  d->boxParams[boxType] = boxParams;
  if(!d->defaultSet)
  {
    d->defaultSet = true;
    d->defaultBoxParams = boxParams;
  }
}

}
