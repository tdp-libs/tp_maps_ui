#ifndef tp_maps_ui_LineEdit_h
#define tp_maps_ui_LineEdit_h

#include "tp_maps_ui/Widget.h"

namespace tp_maps_ui
{

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT LineEdit : public Widget
{
  TP_DQ;
public:
  //################################################################################################
  LineEdit(Widget* parent=nullptr);

  //################################################################################################
  ~LineEdit() override;

  //################################################################################################
  std::u16string text() const;

  //################################################################################################
  void setText(const std::u16string& text);

  //################################################################################################
  std::u16string placeholderText() const;

  //################################################################################################
  void setPlaceholderText(const std::u16string& placeholderText);

protected:
  //################################################################################################
  void render(tp_maps::RenderInfo& renderInfo) override;

  //################################################################################################
  void invalidateBuffers() override;

  //################################################################################################
  bool keyEvent(const tp_maps::KeyEvent& event) override;

  //################################################################################################
  bool textEditingEvent(const tp_maps::TextEditingEvent& event) override;

  //################################################################################################
  bool textInputEvent(const tp_maps::TextInputEvent& event) override;

  //################################################################################################
  void focusEvent(Widget* focusedWidget) override;

  //################################################################################################
  void animate(double timestampMS) override;
};
}
#endif

