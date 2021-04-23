#ifndef tp_maps_ui_ThemeDrawHelper_h
#define tp_maps_ui_ThemeDrawHelper_h

#include "tp_maps_ui/DrawHelper.h"

#include "glm/glm.hpp"

namespace tp_maps_ui
{

//##################################################################################################
enum class FrameDrawMode
{
  Square,
  Rounded
};

//##################################################################################################
struct FrameParameters
{
  TPPixel l; //!< Left border color
  TPPixel r; //!< Right border color
  TPPixel t; //!< Top border color
  TPPixel b; //!< Bottom border color

  TPPixel c; //!< Frame background color

  TPPixel tl; //!< Top left corner color
  TPPixel tr; //!< Top right corner color
  TPPixel bl; //!< Bottom left corner color
  TPPixel br; //!< Bottom right corner color

  size_t borderSize{1};
  size_t radius{0};

  FrameDrawMode drawMode{FrameDrawMode::Square};

  //! For GUI elements that have a natural size, like check boxes and handles.
  glm::vec2 recommendedSize{0.0f, 0.0f};

  TPPixel textColor{0, 0, 0, 255};
  TPPixel placeholderTextColor{128, 128, 128, 255};
};

//##################################################################################################
struct OverlayParameters
{
  TPPixel color{0, 0, 0, 100};
};

//##################################################################################################
struct ThemeParameters
{
  FrameParameters normalPanelFrame;        //!< Dialog windows.
  FrameParameters editableFrame;           //!< Text boxes, spin boxes, and line edits.

  FrameParameters raisedButtonFrame;       //!< Push buttons.
  FrameParameters sunkenButtonFrame;       //!< Push buttons down.

  FrameParameters uncheckedCheckBoxFrame;  //!< Unchecked check box.
  FrameParameters checkedCheckBoxFrame;    //!< Checked check box.

  FrameParameters raisedHandleFrame;       //!< The handle for sliders.
  FrameParameters sunkenHandleFrame;       //!< The handle for sliders.

  OverlayParameters overlay;

  //! Set this and the texture will be saved to file
  std::string debugToFile;
};

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT ThemeDrawHelper : public DrawHelper
{
public:
  //################################################################################################
  ThemeDrawHelper(UILayer* layer, const ThemeParameters& themeParameters);

  //################################################################################################
  ~ThemeDrawHelper() override;

  //################################################################################################
  const ThemeParameters& themeParameters() const;

  //################################################################################################
  void drawBox(const glm::mat4& matrix, float width, float height, BoxType boxType, FillType fillType, VisualModifier visualModifier) override;

  //################################################################################################
  void drawOverlay(const glm::mat4& matrix, float width, float height, float fade) override;

  //################################################################################################
  glm::vec4 textColor(BoxType boxType, FillType fillType, VisualModifier visualModifier) override;

  //################################################################################################
  glm::vec4 placeholderTextColor(BoxType boxType, FillType fillType, VisualModifier visualModifier) override;

protected:
  //################################################################################################
  void invalidateBuffers() override;

private:
  struct Private;
  Private* d;
  friend struct Private;
};
}
#endif

