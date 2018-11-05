#ifndef tp_maps_ui_Overlay_h
#define tp_maps_ui_Overlay_h

#include "tp_maps_ui/Widget.h"
#include "tp_utils/CallbackCollection.h"

namespace tp_maps_ui
{

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT Overlay : public Widget
{
public:
  //################################################################################################
  Overlay(Widget* parent=nullptr);

  //################################################################################################
  ~Overlay() override;

  //################################################################################################
  tp_utils::CallbackCollection<void(void)> clicked;

protected:
  //################################################################################################
  void render(tp_maps::RenderInfo& renderInfo) override;

  //################################################################################################
  void invalidateBuffers() override;

  //################################################################################################
  bool mouseEvent(const tp_maps::MouseEvent& event) override;

  //################################################################################################
  void animate(double timestampMS) override;

private:
  struct Private;
  Private* d;
  friend struct Private;
};
}
#endif

