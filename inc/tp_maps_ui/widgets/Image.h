#ifndef tp_maps_ui_Image_h
#define tp_maps_ui_Image_h

#include "tp_maps_ui/Widget.h"
#include "tp_utils/CallbackCollection.h"

namespace tp_image_utils
{
class ColorMap;
}

namespace tp_maps_ui
{

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT Image : public Widget
{
  TP_DQ;
public:
  //################################################################################################
  Image(Widget* parent=nullptr);

  //################################################################################################
  ~Image() override;

  //################################################################################################
  void setImage(const tp_image_utils::ColorMap& image);

  //################################################################################################
  tp_utils::CallbackCollection<void()> clicked;

protected:
  //################################################################################################
  void render(tp_maps::RenderInfo& renderInfo) override;

  //################################################################################################
  void invalidateBuffers() override;

  //################################################################################################
  bool mouseEvent(const tp_maps::MouseEvent& event) override;

  //################################################################################################
  void animate(double timestampMS) override;
};
}
#endif

