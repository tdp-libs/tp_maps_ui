#ifndef tp_maps_ui_FixedLayout_h
#define tp_maps_ui_FixedLayout_h

#include "tp_maps_ui/Layout.h"

namespace tp_maps_ui
{

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT FixedLayout : public Layout
{
public:
  //################################################################################################
  FixedLayout(Widget* parent=nullptr);

  //################################################################################################
  ~FixedLayout() override;

  //################################################################################################
  void updateLayout() override;

  //################################################################################################
  void addWidget(Widget* widget, Dim x=Dim::zero(), Dim y=Dim::zero(), Dim width=Dim::full(), Dim height=Dim::full());

  //################################################################################################
  void addLayout(Layout* layout, Dim x=Dim::zero(), Dim y=Dim::zero(), Dim width=Dim::full(), Dim height=Dim::full());

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}
#endif

