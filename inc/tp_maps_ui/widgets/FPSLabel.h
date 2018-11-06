#ifndef tp_maps_ui_FPSLabel_h
#define tp_maps_ui_FPSLabel_h

#include "tp_maps_ui/widgets/Label.h"

namespace tp_maps_ui
{

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT FPSLabel : public Label
{
public:
  //################################################################################################
  FPSLabel(Widget* parent=nullptr);

  //################################################################################################
  ~FPSLabel() override;

protected:
  //################################################################################################
  void render(tp_maps::RenderInfo& renderInfo) override;

private:
  struct Private;
  Private* d;
  friend struct Private;
};
}
#endif

