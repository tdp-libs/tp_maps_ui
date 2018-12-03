#ifndef tp_maps_ui_CollapsiblePanel_h
#define tp_maps_ui_CollapsiblePanel_h

#include "tp_maps_ui/Widget.h"

namespace tp_maps_ui
{

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT CollapsiblePanel : public Widget
{
public:
  //################################################################################################
  CollapsiblePanel(Widget* parent=nullptr);

  //################################################################################################
  ~CollapsiblePanel() override;

  //################################################################################################
  void addPage(Widget* page, const std::u16string& label);

  //################################################################################################
  void setGeometry(float x, float y, float w, float h) override;

private:
  struct Private;
  Private* d;
  friend struct Private;
};
}
#endif

