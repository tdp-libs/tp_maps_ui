#ifndef tp_maps_ui_LineEdit_h
#define tp_maps_ui_LineEdit_h

#include "tp_maps_ui/Widget.h"

namespace tp_maps_ui
{

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT LineEdit : public Widget
{
public:
  //################################################################################################
  LineEdit(Widget* parent=nullptr);

  //################################################################################################
  ~LineEdit() override;

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

