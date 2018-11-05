#ifndef tp_maps_ui_PushButton_h
#define tp_maps_ui_PushButton_h

#include "tp_maps_ui/Widget.h"
#include "tp_utils/CallbackCollection.h"

namespace tp_maps_ui
{

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT PushButton : public Widget
{
public:
  //################################################################################################
  PushButton(Widget* parent=nullptr);

  //################################################################################################
  PushButton(const std::u16string& text, Widget* parent=nullptr);

  //################################################################################################
  ~PushButton() override;

  //################################################################################################
  const std::u16string& text() const;

  //################################################################################################
  void setText(const std::u16string& text);

  //################################################################################################
  tp_utils::CallbackCollection<void()> clicked;

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

