#ifndef tp_maps_ui_Widget_h
#define tp_maps_ui_Widget_h

#include "tp_maps_ui/Globals.h"

#include "glm/glm.hpp"

namespace tp_maps
{
class RenderInfo;
struct MouseEvent;
class FontRenderer;
}

namespace tp_maps_ui
{
class UILayer;
class Layout;
class DrawHelper;

//##################################################################################################
class LayoutParams
{
public:
  virtual ~LayoutParams();
};

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT Widget
{
  friend class UILayer;
public:
  //################################################################################################
  Widget(Widget* parent=nullptr);

  //################################################################################################
  virtual ~Widget();

  //################################################################################################
  UILayer* layer() const;

  //################################################################################################
  Widget* parent() const;

  //################################################################################################
  const std::vector<Widget*>& children() const;

  //################################################################################################
  void addWidget(Widget* child, int index=-1);

  //################################################################################################
  void setLayout(Layout* layout);

  //################################################################################################
  Layout* layout() const;

  //################################################################################################
  void setGeometry(float x, float y, float w, float h);

  //################################################################################################
  float x() const;

  //################################################################################################
  float y() const;

  //################################################################################################
  float width() const;

  //################################################################################################
  float height() const;

  //################################################################################################
  float screenX() const;

  //################################################################################################
  float screenY() const;

  //################################################################################################
  float screenWidth() const;

  //################################################################################################
  float screenHeight() const;

  //################################################################################################
  glm::mat4 matrix() const;

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

  //################################################################################################
  void setVisible(bool visible);

  //################################################################################################
  bool visible() const;

  //################################################################################################
  void setTransparentToMouseEvents(bool transparentToMouseEvents);

  //################################################################################################
  bool transparentToMouseEvents() const;

  //################################################################################################
  virtual std::pair<Dim, Dim> sizeHint() const;

  //################################################################################################
  //! Sets a functor that will be called to update an animation.
  /*!
  This will replace an existing animation with a new animation or if the functor is null it will
  stop an existing animation. The functor should return true if the animation is not yet complete,
  returning false will end the animation.

  \param animation The animation functor.
  */
  void setCurrentAnimation(const std::function<bool(double)>& animation);

  //################################################################################################
  //! Used by layout classes to store information about widgets that they layout
  template<typename T, typename... Args>
  T* layoutParams(Args&&... args)
  {
    T* t = dynamic_cast<T*>(layoutParams());
    if(!t)
    {
      t=new T(std::forward<Args>(args)...);
      setLayoutParams(t);
    }
    return t;
  }

private:
  LayoutParams* layoutParams();
  void setLayoutParams(LayoutParams* layoutParams);

protected:
  //################################################################################################
  //! This is called to get the layer to render.
  /*!
  This should be sub-classed to perform any drawing that the layer should do. This will be called
  multiple times to perform different types of rendering. See the \link RenderInfo \endlink
  documentation for a complete description of how to handle the different render passes.

  \param renderInfo - Details about the type of render to perform.
  */
  virtual void render(tp_maps::RenderInfo& renderInfo);

  //################################################################################################
  //! Called when buffers become invalid.
  /*!
  This is called when the OpenGL context becomes invalid, all OpenGL resources should be ignored.
  */
  virtual void invalidateBuffers();

  //################################################################################################
  //! Called with mouse press events.
  /*!
  \param event - Details of the event that was passed to the map.
  */
  virtual bool mouseEvent(const tp_maps::MouseEvent& event);

  //################################################################################################
  //! Update the state of the animation
  virtual void animate(double timestampMS);

  //################################################################################################
  //! Calls update on the map
  void update();

private:
  //################################################################################################
  void renderInternal(tp_maps::RenderInfo& renderInfo);

  //################################################################################################
  void invalidateBuffersInternal();

  //################################################################################################
  bool mouseEventInternal(const tp_maps::MouseEvent& event);

  //################################################################################################
  void animateInternal(double timestampMS);

  //################################################################################################
  void setLayer(UILayer* layer);

  struct Private;
  Private* d;
  friend struct Private;
};

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT Spacing : public Widget
{
  Dim m_spacing;
public:
  //################################################################################################
  Spacing(const Dim& spacing, Widget* parent=nullptr);

  //################################################################################################
  std::pair<Dim, Dim> sizeHint() const override;
};


}
#endif

