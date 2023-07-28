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
  UILayer(size_t fromStage);

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
  \param drawHelper The draw helper used to draw backgrounds, frames, and buttons.
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
  bool keyEvent(const tp_maps::KeyEvent& event) override;

  //################################################################################################
  bool textEditingEvent(const tp_maps::TextEditingEvent& event) override;

  //################################################################################################
  bool textInputEvent(const tp_maps::TextInputEvent& event) override;

  //################################################################################################
  void animate(double timestampMS) override;

  //################################################################################################
  //! Give a widget focus.
  void focus(Widget* widget);

  //################################################################################################
  //! Register widget to receive text editing events.
  /*!
  This will register widget for text editing events and if required show the keyboard. The geometry
  of the widget may be used to position the on screen keyboard and the screen.
  */
  void startTextInput(Widget* widget);

  //################################################################################################
  //! Unregister widget to receive text editing events.
  /*!
  This will unregister widget for text editing events and if required hide the keyboard.
  */
  void stopTextInput(Widget* widget);

  //################################################################################################
  void update();

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

