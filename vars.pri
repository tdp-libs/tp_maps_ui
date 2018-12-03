TARGET = tp_maps_ui
TEMPLATE = lib

DEFINES += TP_MAPS_UI_WIDGET_LIBRARY

SOURCES += src/Globals.cpp
HEADERS += inc/tp_maps_ui/Globals.h

SOURCES += src/Layout.cpp
HEADERS += inc/tp_maps_ui/Layout.h

SOURCES += src/Widget.cpp
HEADERS += inc/tp_maps_ui/Widget.h

SOURCES += src/DrawHelper.cpp
HEADERS += inc/tp_maps_ui/DrawHelper.h


#-- Layers -----------------------------------------------------------------------------------------
SOURCES += src/layers/UILayer.cpp
HEADERS += inc/tp_maps_ui/layers/UILayer.h


#-- Layouts ----------------------------------------------------------------------------------------
SOURCES += src/layouts/BoxLayout.cpp
HEADERS += inc/tp_maps_ui/layouts/BoxLayout.h

SOURCES += src/layouts/FixedLayout.cpp
HEADERS += inc/tp_maps_ui/layouts/FixedLayout.h

SOURCES += src/layouts/SubWindowLayout.cpp
HEADERS += inc/tp_maps_ui/layouts/SubWindowLayout.h


#-- Widgets ----------------------------------------------------------------------------------------
SOURCES += src/widgets/Frame.cpp
HEADERS += inc/tp_maps_ui/widgets/Frame.h

SOURCES += src/widgets/Overlay.cpp
HEADERS += inc/tp_maps_ui/widgets/Overlay.h

SOURCES += src/widgets/CheckBox.cpp
HEADERS += inc/tp_maps_ui/widgets/CheckBox.h

SOURCES += src/widgets/LineEdit.cpp
HEADERS += inc/tp_maps_ui/widgets/LineEdit.h

SOURCES += src/widgets/ListWidget.cpp
HEADERS += inc/tp_maps_ui/widgets/ListWidget.h

SOURCES += src/widgets/PushButton.cpp
HEADERS += inc/tp_maps_ui/widgets/PushButton.h

SOURCES += src/widgets/TreeWidget.cpp
HEADERS += inc/tp_maps_ui/widgets/TreeWidget.h

SOURCES += src/widgets/Label.cpp
HEADERS += inc/tp_maps_ui/widgets/Label.h

SOURCES += src/widgets/TabBar.cpp
HEADERS += inc/tp_maps_ui/widgets/TabBar.h

SOURCES += src/widgets/TabPanel.cpp
HEADERS += inc/tp_maps_ui/widgets/TabPanel.h

SOURCES += src/widgets/TabWidget.cpp
HEADERS += inc/tp_maps_ui/widgets/TabWidget.h

SOURCES += src/widgets/FPSLabel.cpp
HEADERS += inc/tp_maps_ui/widgets/FPSLabel.h

SOURCES += src/widgets/Slider.cpp
HEADERS += inc/tp_maps_ui/widgets/Slider.h

SOURCES += src/widgets/CollapsiblePanel.cpp
HEADERS += inc/tp_maps_ui/widgets/CollapsiblePanel.h


#-- Draw Helpers -----------------------------------------------------------------------------------
SOURCES += src/draw_helpers/DarkDrawHelper.cpp
HEADERS += inc/tp_maps_ui/draw_helpers/DarkDrawHelper.h

SOURCES += src/draw_helpers/LightDrawHelper.cpp
HEADERS += inc/tp_maps_ui/draw_helpers/LightDrawHelper.h

SOURCES += src/draw_helpers/ThemeDrawHelper.cpp
HEADERS += inc/tp_maps_ui/draw_helpers/ThemeDrawHelper.h
