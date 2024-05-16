#ifndef tp_maps_ui_Frame_h
#define tp_maps_ui_Frame_h

#include "tp_maps_ui/Widget.h"

namespace tp_maps_ui
{

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT Frame : public Widget
{
  TP_DQ;
public:
  //################################################################################################
  Frame(Widget* parent=nullptr);

  //################################################################################################
  ~Frame() override;

protected:
  //################################################################################################
  void render(tp_maps::RenderInfo& renderInfo) override;

  //################################################################################################
  void invalidateBuffers() override;
};
}
#endif

