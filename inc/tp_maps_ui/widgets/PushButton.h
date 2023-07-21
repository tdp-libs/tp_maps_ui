#ifndef tp_maps_ui_PushButton_h
#define tp_maps_ui_PushButton_h

#include "tp_maps_ui/Widget.h"
#include "tp_maps_ui/DrawHelper.h"

#include "tp_utils/CallbackCollection.h"

namespace tp_image_utils
{
class ColorMap;
}

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
  void setImage(const tp_image_utils::ColorMap& normalImage, const tp_image_utils::ColorMap& pressedImage);

  //################################################################################################
  bool drawBackground() const;

  //################################################################################################
  void setDrawBackground(bool drawBackground);

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

  //################################################################################################
  VisualModifier currentVisualModifier() const;

  //################################################################################################
  void setCurrentVisualModifier(VisualModifier currentVisualModifier);

private:
  struct Private;
  Private* d;
  friend struct Private;
};
}
#endif

