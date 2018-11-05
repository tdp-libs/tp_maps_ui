#ifndef tp_maps_ui_ListWidget_h
#define tp_maps_ui_ListWidget_h

#include "tp_maps_ui/Widget.h"

namespace tp_maps_ui
{

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT ListWidget : public Widget
{
public:
  //################################################################################################
  ListWidget(Widget* parent=nullptr);

  //################################################################################################
  ~ListWidget() override;

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

