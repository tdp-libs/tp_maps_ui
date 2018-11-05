#ifndef tdp_maps_widget_global_h
#define tdp_maps_widget_global_h

#include "tp_utils/StringID.h"

#if defined(TDP_MAPS_WIDGET_LIBRARY)
#  define TDP_MAPS_WIDGET_SHARED_EXPORT TP_EXPORT
#else
#  define TDP_MAPS_WIDGET_SHARED_EXPORT TP_IMPORT
#endif

//##################################################################################################
//! A simple 3D engine for widget based applications.
namespace tdp_maps_widget
{

}

#endif
