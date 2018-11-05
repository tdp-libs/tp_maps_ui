#ifndef tp_maps_ui_UILayer_h
#define tp_maps_ui_UILayer_h

#include "tp_maps_ui/Globals.h"

#include "tp_maps/Layer.h"

namespace tp_maps
{
class FontRenderer;
}

namespace tp_maps_ui
{
class Widget;
class DrawHelper;

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT UILayer : public tp_maps::Layer
{
  friend class Widget;
  friend class DrawHelper;
public:
  //################################################################################################
  UILayer();

  //################################################################################################
  ~UILayer() override;

  //################################################################################################
  Widget* rootWidget() const;

  //################################################################################################
  //! Set the font that will be used to render text
  /*!
  Most widgets only use a single font, this method should be used to set it.
  \note This does not take ownership.
  \param font The font to use for drawing text on this widget.
  */
  void setFont(tp_maps::FontRenderer* font);

  //################################################################################################
  tp_maps::FontRenderer* font() const;

  //################################################################################################
  //! Set the draw helper that will be used to render graphical elements of the widget
  /*!
  \note This does not take ownership.
  \param drawHelper The font to use for drawing text on this widget.
  */
  void setDrawHelper(DrawHelper* drawHelper);

  //################################################################################################
  DrawHelper* drawHelper() const;

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
  //################################################################################################
  void addDrawHelper(DrawHelper* drawHelper);

  //################################################################################################
  void removeDrawHelper(DrawHelper* drawHelper);

private:
  struct Private;
  Private* d;
  friend struct Private;
};
}
#endif

