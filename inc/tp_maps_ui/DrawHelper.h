#ifndef tp_maps_ui_DrawHelper_h
#define tp_maps_ui_DrawHelper_h

#include "tp_maps_ui/Globals.h"

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
  Editable, //!< An editable panel like a text box or spin box.
  Panel,    //!< A raised floading panel.
  Float     //!< For example the white panel that appears below a combo box, or a tooltip.
};

//##################################################################################################
enum class VisualModifier
{
  Normal,
  Focus,
  Pressed,
  Disabled
};

//##################################################################################################
struct BoxParams
{
  float marginLeft{0};
  float marginRight{0};
  float marginTop{0};
  float marginBottom{0};
};

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT DrawHelper
{
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
  const BoxParams& boxParams(BoxType boxType) const;

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

private:
  struct Private;
  friend struct Private;
  Private* d;
};
}
#endif

