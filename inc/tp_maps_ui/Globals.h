#ifndef tp_maps_ui_Globals_h
#define tp_maps_ui_Globals_h

#include "tp_utils/StringID.h"

#if defined(TP_MAPS_UI_LIBRARY)
#  define TP_MAPS_UI_SHARED_EXPORT TP_EXPORT
#else
#  define TP_MAPS_UI_SHARED_EXPORT TP_IMPORT
#endif

//##################################################################################################
//! UI conponents for use in tp_maps.
namespace tp_maps_ui
{

//##################################################################################################
enum class Orientation : size_t
{
  Horizontal,
  Vertical
};

//##################################################################################################
enum class HAlignment : size_t
{
  Left,
  Center,
  Right
};

//##################################################################################################
enum class VAlignment : size_t
{
  Top,
  Center,
  Bottom
};

//##################################################################################################
enum class CheckState : size_t
{
  Unchecked,
  PartiallyChecked,
  Checked
};

//##################################################################################################
struct Dim
{
  float sizePixels{0.0f};
  float sizeFraction{1.0f};

  //################################################################################################
  Dim()=default;

  //################################################################################################
  Dim(float sizePixels_, float sizeFraction_):
    sizePixels(sizePixels_),
    sizeFraction(sizeFraction_)
  {

  }

  //################################################################################################
  float calc(float parentSize) const
  {
    return (parentSize*sizeFraction) + sizePixels;
  }

  //################################################################################################
  static inline Dim full()
  {
    return Dim(0.0f, 1.0f);
  }

  //################################################################################################
  static inline Dim zero()
  {
    return Dim(0.0f, 0.0f);
  }

  //################################################################################################
  static inline Dim pixels(float pixels)
  {
    return Dim(pixels, 0.0f);
  }
};

//##################################################################################################
struct Margins
{
  Dim left  {Dim::pixels(6.0f)};
  Dim top   {Dim::pixels(6.0f)};
  Dim right {Dim::pixels(6.0f)};
  Dim bottom{Dim::pixels(6.0f)};
};

//##################################################################################################
bool calculateAnimationValue(float targetValue, float& currentValue, float speed);

//##################################################################################################
bool calculateAnimationDim(const tp_maps_ui::Dim& target, tp_maps_ui::Dim& dim, float speedR, float speedP);

}

#endif
