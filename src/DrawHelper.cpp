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

  glm::vec2 defaultRecommendedSize{0.0f, 0.0f};
  std::unordered_map<FillType, glm::vec2> recommendedSizes;

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
const glm::vec2& DrawHelper::recommendedSize(FillType fillType) const
{
  auto i = d->recommendedSizes.find(fillType);
  return (i!=d->recommendedSizes.end())?i->second:d->defaultRecommendedSize;
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

//##################################################################################################
void DrawHelper::setRecommendedSize(FillType fillType, const glm::vec2& recommendedSize)
{
  d->recommendedSizes[fillType] = recommendedSize;
}

}
