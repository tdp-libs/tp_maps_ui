#ifndef tp_maps_ui_ThemeDrawHelper_h
#define tp_maps_ui_ThemeDrawHelper_h

#include "tp_maps_ui/DrawHelper.h"

#include "glm/glm.hpp"

namespace tp_maps_ui
{

//##################################################################################################
struct FrameParameters
{
  tp_maps::Pixel l;
  tp_maps::Pixel r;
  tp_maps::Pixel t;
  tp_maps::Pixel b;

  tp_maps::Pixel c;

  tp_maps::Pixel tl;
  tp_maps::Pixel tr;
  tp_maps::Pixel bl;
  tp_maps::Pixel br;

  tp_maps::Pixel textColor{0, 0, 0, 255};
};

//##################################################################################################
struct OverlayParameters
{
  tp_maps::Pixel color{0, 0, 0, 100};
};

//##################################################################################################
struct ThemeParameters
{
  FrameParameters normalPanelFrame;

  FrameParameters raisedButtonFrame;
  FrameParameters sunkenButtonFrame;

  FrameParameters checkedCheckBoxFrame;
  FrameParameters uncheckedCheckBoxFrame;

  OverlayParameters overlay;
};

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT ThemeDrawHelper : public DrawHelper
{
public:
  //################################################################################################
  ThemeDrawHelper(UILayer* layer, const ThemeParameters& themeParameters);

  //################################################################################################
  ~ThemeDrawHelper() override;

  //################################################################################################
  const ThemeParameters& themeParameters() const;

  //################################################################################################
  void drawBox(const glm::mat4& matrix, float width, float height, BoxType boxType, FillType fillType, VisualModifier visualModifier) override;

  //################################################################################################
  void drawOverlay(const glm::mat4& matrix, float width, float height, float fade) override;

  //################################################################################################
  glm::vec4 textColor(BoxType boxType, FillType fillType, VisualModifier visualModifier) override;

protected:
  //################################################################################################
  void invalidateBuffers() override;

private:
  struct Private;
  Private* d;
  friend struct Private;
};
}
#endif

