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
  ListWidget(Orientation orientation, Widget* parent=nullptr);

  //################################################################################################
  ~ListWidget() override;

  //################################################################################################
  void dataChanged();

  //################################################################################################
  template<typename T>
  void setDelegate(T* delegate)
  {
    setNumberOfRowsCallback([=](){return delegate->numberOfRows();});
    setWidgetForRowCallback([=](size_t row, Widget* existing){return delegate->widgetForRow(row, existing);});
  }

  //################################################################################################
  void setNumberOfRowsCallback(const std::function<size_t()>& numberOfRows);

  //################################################################################################
  void setWidgetForRowCallback(const std::function<Widget*(size_t, Widget*)>& widgetForRow);

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

