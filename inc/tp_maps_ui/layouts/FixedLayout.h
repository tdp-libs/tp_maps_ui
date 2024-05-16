#ifndef tp_maps_ui_FixedLayout_h
#define tp_maps_ui_FixedLayout_h

#include "tp_maps_ui/Layout.h"

namespace tp_maps_ui
{

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT FixedLayout : public Layout
{
  TP_DQ;
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

  //################################################################################################
  void updateWidgetDims(Widget* widget, Dim x=Dim::zero(), Dim y=Dim::zero(), Dim width=Dim::full(), Dim height=Dim::full());

  //################################################################################################
  void getWidgetDims(Widget* widget, Dim& x, Dim& y, Dim& width, Dim& height);

  //################################################################################################
  std::function<bool(double)> makeAnimationFunctor(tp_maps_ui::Widget* widget,
                                                   tp_maps_ui::Dim targetX,
                                                   tp_maps_ui::Dim targetY,
                                                   tp_maps_ui::Dim targetWidth,
                                                   tp_maps_ui::Dim targetHeight,
                                                   float speedR,
                                                   float speedP);
};

}
#endif

