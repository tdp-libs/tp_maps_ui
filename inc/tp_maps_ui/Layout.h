#ifndef tp_maps_ui_Layout_h
#define tp_maps_ui_Layout_h

#include "tp_maps_ui/Globals.h"

namespace tp_maps_ui
{
class Widget;

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT Layout
{
  friend class Widget;
public:
  //################################################################################################
  Layout(Widget* parent=nullptr);

  //################################################################################################
  virtual ~Layout();

  //################################################################################################
  Widget* parent() const;

  //################################################################################################
  virtual std::pair<Dim, Dim> sizeHint() const;

  //################################################################################################
  virtual void updateLayout() = 0;

private:
  //################################################################################################
  void setParent(Widget* parent);

private:
  struct Private;
  Private* d;
  friend struct Private;
};
}
#endif

