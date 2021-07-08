#ifndef tp_maps_ui_DrawHelper_h
#define tp_maps_ui_DrawHelper_h

#include "tp_maps_ui/Globals.h"

#include "tp_maps/textures/BasicTexture.h"

#include "glm/glm.hpp"

namespace tp_maps
{
class Map;
}

namespace tp_maps_ui
{
class UILayer;

//##################################################################################################
enum class BoxType
{
  Raised,
  Sunk,
  Flat,
  Ridge,
  Groove,
  Solid
};

//##################################################################################################
enum class FillType
{
  Button,   //!< A raised or depressed button.
  CheckBox, //!< Style of check boxes.
  Radio,    //!< Style of radio buttons.
  Editable, //!< An editable panel like a text box or spin box.
  Panel,    //!< A raised floading panel.
  Float,    //!< For example the white panel that appears below a combo box, or a tooltip.
  Slider    //!< Slider handle.
};

//##################################################################################################
enum class VisualModifier : size_t
{
  Normal,
  Focus,
  Pressed,
  Disabled,
  Partial,
  Checked,
  Unchecked
};

//##################################################################################################
struct BoxParams
{
  float marginLeft     {0.0f};
  float marginRight    {0.0f};
  float marginTop      {0.0f};
  float marginBottom   {0.0f};
};

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT DrawHelper
{
  TP_NONCOPYABLE(DrawHelper);
  friend class UILayer;
public:
  //################################################################################################
  DrawHelper(UILayer* layer);

  //################################################################################################
  virtual ~DrawHelper();

  //################################################################################################
  UILayer* layer() const;

  //################################################################################################
  tp_maps::Map* map() const;

  //################################################################################################
  virtual void drawBox(const glm::mat4& matrix, float width, float height, BoxType boxType, FillType fillType, VisualModifier visualModifier) = 0;

  //################################################################################################
  //! Draw a semi-transparent overlay to obscure out the background.
  virtual void drawOverlay(const glm::mat4& matrix, float width, float height, float fade) = 0;

  //################################################################################################
  virtual glm::vec4 textColor(BoxType boxType, FillType fillType, VisualModifier visualModifier) = 0;

  //################################################################################################
  //! The color for background text, for example the placeholder in a line edit.
  virtual glm::vec4 placeholderTextColor(BoxType boxType, FillType fillType, VisualModifier visualModifier) = 0;

  //################################################################################################
  const BoxParams& boxParams(BoxType boxType) const;

  //################################################################################################
  const glm::vec2& recommendedSize(FillType fillType) const;

protected:
  //################################################################################################
  //! Called when buffers become invalid.
  /*!
  This is called when the OpenGL context becomes invalid, all OpenGL resources should be ignored.
  */
  virtual void invalidateBuffers();

  //################################################################################################
  //! Should be called by subclasses to set details of box geometry.
  /*!
  \note The fist call will be used as the default.
  \param boxType The type of box that these parameters are relevant for.
  \param boxParams The geometry details for this type of box.
  */
  void setBoxParams(BoxType boxType, const BoxParams& boxParams);

  //################################################################################################
  void setRecommendedSize(FillType fillType, const glm::vec2& recommendedSize);

private:
  struct Private;
  friend struct Private;
  Private* d;
};
}
#endif

