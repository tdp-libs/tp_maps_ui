#ifndef tp_maps_ui_SubWindowLayout_h
#define tp_maps_ui_SubWindowLayout_h

#include "tp_maps_ui/Layout.h"

namespace tp_maps_ui
{

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT SubWindowLayout : public Layout
{
public:
  //################################################################################################
  SubWindowLayout(Widget* parent=nullptr);

  //################################################################################################
  ~SubWindowLayout() override;

  //################################################################################################
  void updateLayout() override;

  //################################################################################################
  void addWidget(Widget* widget);

  //################################################################################################
  void addLayout(Layout* layout);

  //################################################################################################
  std::pair<Dim, Dim> sizeHint() const override;

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}
#endif

