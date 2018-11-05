#ifndef tp_maps_ui_BoxLayout_h
#define tp_maps_ui_BoxLayout_h

#include "tp_maps_ui/Layout.h"

namespace tp_maps_ui
{

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT BoxLayout : public Layout
{
public:
  //################################################################################################
  BoxLayout(Orientation orientation, Widget* parent=nullptr);

  //################################################################################################
  ~BoxLayout() override;

  //################################################################################################
  void updateLayout() override;

  //################################################################################################
  void addWidget(Widget* widget, const Dim& fraction=Dim::full(), float stretch=0.0f);

  //################################################################################################
  void addLayout(Layout* layout, const Dim& fraction=Dim::full(), float stretch=0.0f);

  //################################################################################################
  void addStretch(float stretch=1.0f);

  //################################################################################################
  void addSpacing(const Dim& spacing);

private:
  struct Private;
  Private* d;
  friend struct Private;
};

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT VBoxLayout : public BoxLayout
{
public:
  //################################################################################################
  VBoxLayout(Widget* parent=nullptr);
};

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT HBoxLayout : public BoxLayout
{
public:
  //################################################################################################
  HBoxLayout(Widget* parent=nullptr);
};

}
#endif

