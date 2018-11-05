#ifndef tp_maps_ui_Label_h
#define tp_maps_ui_Label_h

#include "tp_maps_ui/Widget.h"
#include "tp_utils/CallbackCollection.h"

namespace tp_maps_ui
{

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT Label : public Widget
{
public:
  //################################################################################################
  Label(Widget* parent=nullptr);

  //################################################################################################
  Label(const std::u16string& text, Widget* parent=nullptr);

  //################################################################################################
  ~Label() override;

  //################################################################################################
  const std::u16string& text() const;

  //################################################################################################
  void setText(const std::u16string& text);

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

