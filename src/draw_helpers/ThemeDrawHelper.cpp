#include "tp_maps_ui/draw_helpers/ThemeDrawHelper.h"
#include "tp_maps_ui/layers/UILayer.h"

#include "tp_maps/Map.h"
#include "tp_maps/shaders/FrameShader.h"
#include "tp_maps/textures/BasicTexture.h"

#include "tp_image_utils/SaveImages.h"

namespace tp_maps_ui
{

namespace
{

using Vert = tp_maps::FrameShader::Vertex;

//##################################################################################################
struct FrameDetails_lt
{
  std::vector<GLushort> indexes;
  std::vector<tp_maps::FrameShader::Vertex> verts;
  tp_maps::FrameShader::VertexBuffer* vertexBuffer{nullptr};
  bool updateVertexBuffer{true};
  glm::vec4 textColor{0.0f, 0.0f, 0.0f, 1.0f};
  glm::vec4 placeholderTextColor{0.5f, 0.5f, 0.5f, 1.0f};
};

//##################################################################################################
struct TextureCoords_lt
{
  float x0 = 0.0f;
  float x1 = 0.0f;
  float y0 = 0.0f;
  float y1 = 0.0f;
};

//##################################################################################################
struct TextureBuffer_lt
{
  TPPixel* rawData;
  tp_image_utils::ColorMap& textureData;

  size_t cx=1;
  size_t cy=1;
  size_t rh=1;

  //################################################################################################
  TextureBuffer_lt(TPPixel* rawData_, tp_image_utils::ColorMap& textureData_):
    rawData(rawData_),
    textureData(textureData_)
  {

  }

  //################################################################################################
  TextureCoords_lt drawCell(size_t w, size_t h, const std::function<TPPixel(float, float)>& func)
  {
    TextureCoords_lt textureCoords;

    size_t x=0;
    size_t y=0;

    float fw = float(w);
    float fh = float(h);

    //-- Make sure we have space to draw the cell --------------------------------------------------
    if((cx+3+w)<textureData.width() && (cy+3+h)<textureData.height())
    {
      x  = cx+1;
      cx = cx+3+w;
      y  = cy+1;
    }
    else
    {
      cx = 1;
      cy += rh+3;
      rh=1;
      if((cx+3+w)<textureData.width() && (cy+3+h)<textureData.height())
      {
        x  = cx+1;
        cx = cx+3+w;
        y  = cy+1;
      }
      else
      {
        return textureCoords;
      }
    }

    if(h>rh)
      rh = h;

    //-- Draw the cell -----------------------------------------------------------------------------
    for(size_t yy=0; yy<h; yy++)
    {
      float fy = float(yy)/fh;
      for(size_t xx=0; xx<w; xx++)
        rawData[(x+xx)+(textureData.width()*(y+yy))] = func(float(xx)/fw, fy);
    }

    //-- Draw the cell borders ---------------------------------------------------------------------
    {
      for(size_t xx=0; xx<w; xx++)
      {
        rawData[(x+xx)+(textureData.width()*(y-1))] = rawData[(x+xx)+(textureData.width()*(y))];
        rawData[(x+xx)+(textureData.width()*(y+h))] = rawData[(x+xx)+(textureData.width()*((y+h)-1))];
      }

      for(size_t yy=0; yy<h; yy++)
      {
        rawData[(x-1)+(textureData.width()*(y+yy))] = rawData[(x)+(textureData.width()*(y+yy))];
        rawData[(x+w)+(textureData.width()*(y+yy))] = rawData[((x+w)-1)+(textureData.width()*(y+yy))];
      }

      rawData[(x-1)+(textureData.width()*(y-1))] = rawData[x+(textureData.width()*y)];
      rawData[(x+w)+(textureData.width()*(y-1))] = rawData[((x+w)-1)+(textureData.width()*y)];

      rawData[(x-1)+(textureData.width()*(y+h))] = rawData[x+(textureData.width()*((y+h)-1))];
      rawData[(x+w)+(textureData.width()*(y+h))] = rawData[((x+w)-1)+(textureData.width()*((y+h)-1))];
    }

    //-- Calculate the texture coords --------------------------------------------------------------
    {
      textureCoords.x0 = float((x+w)) / float(textureData.width());
      textureCoords.x1 = float(x)     / float(textureData.width());
      textureCoords.y0 = float(y)     / float(textureData.height());
      textureCoords.y1 = float((y+h)) / float(textureData.height());
    }

    return textureCoords;
  }
};
}

//##################################################################################################
struct ThemeDrawHelper::Private
{
  TP_REF_COUNT_OBJECTS("tp_maps_ui::ThemeDrawHelper::Private");
  TP_NONCOPYABLE(Private);

  ThemeDrawHelper* q;

  ThemeParameters themeParameters;

  FrameDetails_lt normalPanelFrameDetails;
  FrameDetails_lt editableFrameDetails;
  FrameDetails_lt raisedButtonFrameDetails;
  FrameDetails_lt sunkenButtonFrameDetails;
  FrameDetails_lt checkedCheckBoxFrameDetails;
  FrameDetails_lt uncheckedCheckBoxFrameDetails;

  FrameDetails_lt raisedHandleFrameDetails;
  FrameDetails_lt sunkenHandleFrameDetails;

  FrameDetails_lt overlayFrameDetails;

  tp_maps::BasicTexture* texture{nullptr};
  GLuint textureID{0};

  glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

  bool bindBeforeRender{true};

  //################################################################################################
  Private(ThemeDrawHelper* q_, const ThemeParameters& themeParameters_):
    q(q_),
    themeParameters(themeParameters_)
  {

  }

  //################################################################################################
  ~Private()
  {
    if(textureID)
    {
      q->map()->makeCurrent();
      q->map()->deleteTexture(textureID);
    }

    delete texture;
    deleteVertexBuffers();
  }

  //################################################################################################
  void deleteVertexBuffers()
  {
    delete normalPanelFrameDetails.vertexBuffer;
    normalPanelFrameDetails.vertexBuffer=nullptr;

    delete editableFrameDetails.vertexBuffer;
    editableFrameDetails.vertexBuffer=nullptr;

    delete raisedButtonFrameDetails.vertexBuffer;
    raisedButtonFrameDetails.vertexBuffer=nullptr;

    delete sunkenButtonFrameDetails.vertexBuffer;
    sunkenButtonFrameDetails.vertexBuffer=nullptr;

    delete checkedCheckBoxFrameDetails.vertexBuffer;
    checkedCheckBoxFrameDetails.vertexBuffer=nullptr;

    delete uncheckedCheckBoxFrameDetails.vertexBuffer;
    uncheckedCheckBoxFrameDetails.vertexBuffer=nullptr;

    delete overlayFrameDetails.vertexBuffer;
    overlayFrameDetails.vertexBuffer=nullptr;

    delete raisedHandleFrameDetails.vertexBuffer;
    raisedHandleFrameDetails.vertexBuffer=nullptr;

    delete sunkenHandleFrameDetails.vertexBuffer;
    sunkenHandleFrameDetails.vertexBuffer=nullptr;
  }

  //################################################################################################
  static void drawCell(TPPixel* rawData, tp_image_utils::ColorMap& textureData, size_t x, size_t y, size_t w, size_t h, const std::function<TPPixel(float, float)>& func)
  {
    float fw = float(w);
    float fh = float(h);
    for(size_t yy=0; yy<h; yy++)
    {
      float fy = float(yy)/fh;
      for(size_t xx=0; xx<w; xx++)
        rawData[(x+xx)+(textureData.width()*(y+yy))] = func(float(xx)/fw, fy);
    }
  }

  //################################################################################################
  static void generateFrame(TextureBuffer_lt& textureBuffer, FrameDetails_lt& frame, const FrameParameters& colors)
  {
    frame.textColor.x = float(colors.textColor.r)/255.0f;
    frame.textColor.y = float(colors.textColor.g)/255.0f;
    frame.textColor.z = float(colors.textColor.b)/255.0f;
    frame.textColor.w = float(colors.textColor.a)/255.0f;

    frame.placeholderTextColor.x = float(colors.placeholderTextColor.r)/255.0f;
    frame.placeholderTextColor.y = float(colors.placeholderTextColor.g)/255.0f;
    frame.placeholderTextColor.z = float(colors.placeholderTextColor.b)/255.0f;
    frame.placeholderTextColor.w = float(colors.placeholderTextColor.a)/255.0f;

    size_t borderSize = colors.borderSize;
    size_t centerSize = 10;

    float borderSizeF = float(borderSize);

    size_t   leftColumnW = borderSize;
    size_t centerColumnW = centerSize;
    size_t  rightColumnW = borderSize;

    size_t    topRowH = borderSize;
    size_t centerRowH = centerSize;
    size_t bottomRowH = borderSize;


    TextureCoords_lt ttl;
    TextureCoords_lt ttc;
    TextureCoords_lt ttr;

    TextureCoords_lt tcl;
    TextureCoords_lt tcc;
    TextureCoords_lt tcr;

    TextureCoords_lt tbl;
    TextureCoords_lt tbc;
    TextureCoords_lt tbr;

    switch(colors.drawMode)
    {
    case FrameDrawMode::Square: //------------------------------------------------------------------
    {
      ttl = textureBuffer.drawCell(  leftColumnW,    topRowH, [&](float, float){return colors.tl;});
      ttc = textureBuffer.drawCell(centerColumnW,    topRowH, [&](float, float){return colors.t; });
      ttr = textureBuffer.drawCell( rightColumnW,    topRowH, [&](float, float){return colors.tr;});

      tcl = textureBuffer.drawCell(  leftColumnW, centerRowH, [&](float, float){return colors.l; });
      tcc = textureBuffer.drawCell(centerColumnW, centerRowH, [&](float, float){return colors.c; });
      tcr = textureBuffer.drawCell( rightColumnW, centerRowH, [&](float, float){return colors.r; });

      tbl = textureBuffer.drawCell(  leftColumnW, bottomRowH, [&](float, float){return colors.bl;});
      tbc = textureBuffer.drawCell(centerColumnW, bottomRowH, [&](float, float){return colors.b; });
      tbr = textureBuffer.drawCell( rightColumnW, bottomRowH, [&](float, float){return colors.br;});

      break;
    }

    case FrameDrawMode::Rounded: //-----------------------------------------------------------------
    {
      auto drawRadius = [&](const TPPixel& c, float fx, float fy)
      {
        return [=](float x, float y)
        {
          x = fx-x;
          y = fy-y;

          float a = 1.0f;
          float l=std::sqrt(x*x + y*y);
          if(l>0.8f)
            a = std::clamp(1.0f - (l-0.8f)*5, 0.0f, 1.0f);

          a = a*a*255.0f;

          return TPPixel(c.r, c.g, c.b, uint8_t(a));
        };
      };

      ttl = textureBuffer.drawCell(  leftColumnW,    topRowH, drawRadius(colors.tl, 0.0f, 1.0f));
      ttc = textureBuffer.drawCell(centerColumnW,    topRowH, [&](float, float){return colors.t; });
      ttr = textureBuffer.drawCell( rightColumnW,    topRowH, drawRadius(colors.tr, 1.0f, 1.0f));

      tcl = textureBuffer.drawCell(  leftColumnW, centerRowH, [&](float, float){return colors.l; });
      tcc = textureBuffer.drawCell(centerColumnW, centerRowH, [&](float, float){return colors.c; });
      tcr = textureBuffer.drawCell( rightColumnW, centerRowH, [&](float, float){return colors.r; });

      tbl = textureBuffer.drawCell(  leftColumnW, bottomRowH, drawRadius(colors.bl, 0.0f, 0.0f));
      tbc = textureBuffer.drawCell(centerColumnW, bottomRowH, [&](float, float){return colors.b; });
      tbr = textureBuffer.drawCell( rightColumnW, bottomRowH, drawRadius(colors.br, 1.0f, 0.0f));

      break;
    }
    }

    frame.verts.clear();
    frame.indexes.clear();
    frame.updateVertexBuffer = true;

    glm::fquat tbnq = {0.0f, 0.0f, 0.0f, 1.0f};

#if 0
    //Print out the full texture
    frame.verts.resize(4);
    frame.verts[0] = Vert({0, 0, 0.0f}, {0.0f, 0.0f, 0.0f}, normal, tangent, {0, 0});
    frame.verts[1] = Vert({0, 0, 0.0f}, {1.0f, 0.0f, 0.0f}, normal, tangent, {1, 0});
    frame.verts[2] = Vert({0, 0, 0.0f}, {1.0f, 1.0f, 0.0f}, normal, tangent, {1, 1});
    frame.verts[3] = Vert({0, 0, 0.0f}, {0.0f, 1.0f, 0.0f}, normal, tangent, {0, 1});

    frame.indexes = {3,2,0,2,1,0};
#else
    auto addIndexes = [&]()
    {
      for(auto i : {0, 1, 2, 0, 2, 3})
        frame.indexes.push_back(GLushort(size_t(i)+frame.verts.size()));
    };

    auto addCell = [&](float px0, float py0, float px1, float py1, float rx0, float ry0, float rx1, float ry1, const TextureCoords_lt& t)
    {
      addIndexes();
      frame.verts.push_back(Vert({px0, py0, 0.0f}, {rx0, ry0, 0.0f}, tbnq, {t.x0, t.y0}));
      frame.verts.push_back(Vert({px1, py0, 0.0f}, {rx1, ry0, 0.0f}, tbnq, {t.x1, t.y0}));
      frame.verts.push_back(Vert({px1, py1, 0.0f}, {rx1, ry1, 0.0f}, tbnq, {t.x1, t.y1}));
      frame.verts.push_back(Vert({px0, py1, 0.0f}, {rx0, ry1, 0.0f}, tbnq, {t.x0, t.y1}));
    };

    float px0 = 0.0f;
    float px1 = borderSizeF;
    float px2 = -borderSizeF;
    float px3 = 0.0f;

    float py0 = 0.0f;
    float py1 = borderSizeF;
    float py2 = -borderSizeF;
    float py3 = 0.0f;

    float rx0 = 0.0f;
    float rx1 = 0.0f;
    float rx2 = 1.0f;
    float rx3 = 1.0f;

    float ry0 = 0.0f;
    float ry1 = 0.0f;
    float ry2 = 1.0f;
    float ry3 = 1.0f;

    addCell(px0, py0, px1, py1, rx0, ry0, rx1, ry1, ttl);
    addCell(px1, py0, px2, py1, rx1, ry0, rx2, ry1, ttc);
    addCell(px2, py0, px3, py1, rx2, ry0, rx3, ry1, ttr);

    addCell(px0, py1, px1, py2, rx0, ry1, rx1, ry2, tcl);
    addCell(px1, py1, px2, py2, rx1, ry1, rx2, ry2, tcc);
    addCell(px2, py1, px3, py2, rx2, ry1, rx3, ry2, tcr);

    addCell(px0, py2, px1, py3, rx0, ry2, rx1, ry3, tbl);
    addCell(px1, py2, px2, py3, rx1, ry2, rx2, ry3, tbc);
    addCell(px2, py2, px3, py3, rx2, ry2, rx3, ry3, tbr);

    std::reverse(frame.indexes.begin(), frame.indexes.end());
#endif


  }

  //################################################################################################
  static void generateOverlay(TextureBuffer_lt& textureBuffer, FrameDetails_lt& frame, const OverlayParameters& params)
  {
    size_t cellSize = 1;


    auto t = textureBuffer.drawCell(cellSize, cellSize, [&](float, float){return params.color;});

    frame.verts.clear();
    frame.indexes.clear();
    frame.updateVertexBuffer = true;

    glm::fquat tbnq = {0.0f, 0.0f, 0.0f, 1.0f};

    frame.verts.resize(4);
    frame.verts[0] = Vert({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, tbnq, {t.x0, t.y0});
    frame.verts[1] = Vert({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, tbnq, {t.x1, t.y0});
    frame.verts[2] = Vert({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, tbnq, {t.x1, t.y1});
    frame.verts[3] = Vert({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, tbnq, {t.x0, t.y1});

    frame.indexes = {3,2,0,2,1,0};
  }

  //################################################################################################
  void populateGeometry()
  {
    if(texture)
      return;

    texture = new tp_maps::BasicTexture(q->map());
    texture->setMagFilterOption(GL_NEAREST);
    texture->setMinFilterOption(GL_NEAREST);

    tp_image_utils::ColorMap textureData(512, 512);

    TextureBuffer_lt textureBuffer(textureData.data(), textureData);

    generateFrame(textureBuffer,       normalPanelFrameDetails, themeParameters.normalPanelFrame      );
    generateFrame(textureBuffer,          editableFrameDetails, themeParameters.editableFrame         );
    generateFrame(textureBuffer,      raisedButtonFrameDetails, themeParameters.raisedButtonFrame     );
    generateFrame(textureBuffer,      sunkenButtonFrameDetails, themeParameters.sunkenButtonFrame     );
    generateFrame(textureBuffer,   checkedCheckBoxFrameDetails, themeParameters.checkedCheckBoxFrame  );
    generateFrame(textureBuffer, uncheckedCheckBoxFrameDetails, themeParameters.uncheckedCheckBoxFrame);
    generateFrame(textureBuffer,      raisedHandleFrameDetails, themeParameters.raisedHandleFrame     );
    generateFrame(textureBuffer,      sunkenHandleFrameDetails, themeParameters.sunkenHandleFrame     );

    generateOverlay(textureBuffer, overlayFrameDetails, themeParameters.overlay);

    q->setRecommendedSize(FillType::CheckBox, themeParameters.checkedCheckBoxFrame.recommendedSize);
    q->setRecommendedSize(FillType::Slider  , themeParameters.   raisedHandleFrame.recommendedSize);

    if(!themeParameters.debugToFile.empty())
      tp_image_utils::saveImage(themeParameters.debugToFile, textureData);

    texture->setImage(textureData, tp_maps::NChannels::RGBA);
  }

  //################################################################################################
  void drawFrame(const glm::mat4& matrix, FrameDetails_lt& frame, float width, float height, const glm::vec4& color)
  {
    populateGeometry();

    if(!texture || !texture->imageReady())
      return;

    auto shader = q->map()->getShader<tp_maps::FrameShader>();
    if(shader->error())
      return;

    if(bindBeforeRender)
    {
      bindBeforeRender=false;
      q->map()->deleteTexture(textureID);
      textureID = texture->bindTexture();
    }

    if(!textureID)
      return;

    if(!frame.vertexBuffer || frame.updateVertexBuffer)
    {
      delete frame.vertexBuffer;
      frame.vertexBuffer = shader->generateVertexBuffer(q->map(), frame.indexes, frame.verts);
      frame.updateVertexBuffer=false;
    }

    shader->use(q->map()->renderInfo().shaderType());
    shader->setMatrix(matrix);
    shader->setScale({width, height, 1.0f});
    shader->setTexture(textureID);

    shader->draw(GL_TRIANGLES, frame.vertexBuffer, color);
  }


  //################################################################################################
  FrameDetails_lt& selectFrameDetails(BoxType boxType, FillType fillType, VisualModifier visualModifier)
  {
    TP_UNUSED(boxType);

    switch(fillType)
    {
    case FillType::CheckBox:
    {
      switch(visualModifier)
      {
      case VisualModifier::Checked:
        return checkedCheckBoxFrameDetails;

      default:
        return uncheckedCheckBoxFrameDetails;
      }
    }
    case FillType::Editable:
    {
      return editableFrameDetails;
    }
    case FillType::Panel:
    {
      return normalPanelFrameDetails;
    }
    case FillType::Slider:
    {
      switch(visualModifier)
      {
      case VisualModifier::Pressed:
        return sunkenHandleFrameDetails;

      default:
        return raisedHandleFrameDetails;
      }
    }
    default:
    {
      switch(visualModifier)
      {
      case VisualModifier::Pressed:
        return sunkenButtonFrameDetails;

      default:
        return raisedButtonFrameDetails;
      }
    }
    }
  }
};

//##################################################################################################
ThemeDrawHelper::ThemeDrawHelper(UILayer* layer, const ThemeParameters& themeParameters):
  DrawHelper(layer),
  d(new Private(this, themeParameters))
{

}

//##################################################################################################
ThemeDrawHelper::~ThemeDrawHelper()
{
  delete d;
}

//##################################################################################################
const ThemeParameters& ThemeDrawHelper::themeParameters() const
{
  return d->themeParameters;
}

//##################################################################################################
void ThemeDrawHelper::drawBox(const glm::mat4& matrix, float width, float height, BoxType boxType, FillType fillType, VisualModifier visualModifier)
{
  d->drawFrame(matrix, d->selectFrameDetails(boxType, fillType, visualModifier), width, height, d->color);
}

//##################################################################################################
void ThemeDrawHelper::drawOverlay(const glm::mat4& matrix, float width, float height, float fade)
{
  auto c = d->color;
  c.w = fade;
  d->drawFrame(matrix, d->overlayFrameDetails, width, height, c);
}

//##################################################################################################
glm::vec4 ThemeDrawHelper::textColor(BoxType boxType, FillType fillType, VisualModifier visualModifier)
{
  d->populateGeometry();
  return d->selectFrameDetails(boxType, fillType, visualModifier).textColor;
}

//##################################################################################################
glm::vec4 ThemeDrawHelper::placeholderTextColor(BoxType boxType, FillType fillType, VisualModifier visualModifier)
{
  d->populateGeometry();
  return d->selectFrameDetails(boxType, fillType, visualModifier).placeholderTextColor;
}

//##################################################################################################
void ThemeDrawHelper::invalidateBuffers()
{
  DrawHelper::invalidateBuffers();

  d->deleteVertexBuffers();
  d->normalPanelFrameDetails      .updateVertexBuffer = true;
  d->editableFrameDetails         .updateVertexBuffer = true;
  d->raisedButtonFrameDetails     .updateVertexBuffer = true;
  d->sunkenButtonFrameDetails     .updateVertexBuffer = true;
  d->checkedCheckBoxFrameDetails  .updateVertexBuffer = true;
  d->uncheckedCheckBoxFrameDetails.updateVertexBuffer = true;
  d->overlayFrameDetails          .updateVertexBuffer = true;

  d->textureID = 0;
  d->bindBeforeRender = true;
}

}
