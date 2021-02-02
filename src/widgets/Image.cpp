#include "tp_maps_ui/widgets/Image.h"
#include "tp_maps_ui/layers/UILayer.h"
#include "tp_maps_ui/DrawHelper.h"

#include "tp_maps/Map.h"
#include "tp_maps/MouseEvent.h"
#include "tp_maps/shaders/ImageShader.h"
#include "tp_maps/textures/BasicTexture.h"

#include "glm/gtx/transform.hpp"

#include <memory>

namespace tp_maps_ui
{

//##################################################################################################
struct Image::Private
{
  TP_REF_COUNT_OBJECTS("tp_maps_ui::Image::Private");
  TP_NONCOPYABLE(Private);
  Private() = default;

  tp_image_utils::ColorMap image;
  std::unique_ptr<tp_maps::BasicTexture> imageTexture;

  tp_maps::ImageShader::VertexBuffer* imageVertexBuffer{nullptr};

  //This represents the button state, normal, hover, presses, etc...
  VisualModifier currentVisualModifier{VisualModifier::Normal};

  GLuint imageTextureID{0};

  bool bindBeforeRender{false};
  bool updateVertexBuffer{true};
};

//##################################################################################################
Image::Image(Widget* parent):
  Widget(parent),
  d(new Private())
{

}

//##################################################################################################
Image::~Image()
{
  delete d;
}

//################################################################################################
void Image::setImage(const tp_image_utils::ColorMap& image)
{
  d->image  = image.clone2();

  d->bindBeforeRender = true;
}

//##################################################################################################
void Image::render(tp_maps::RenderInfo& renderInfo)
{
  TP_UNUSED(renderInfo);

  auto m = matrix();

  //Draw image
  {
    if(d->bindBeforeRender)
    {
      d->bindBeforeRender=false;
      d->updateVertexBuffer=true;

      {
        layer()->map()->deleteTexture(d->imageTextureID);
        d->imageTextureID = 0;

        if(d->image.size()>0)
        {
          if(!d->imageTexture)
            d->imageTexture.reset(new tp_maps::BasicTexture(layer()->map()));
          d->imageTexture->setImage(d->image);

          if(d->imageTexture->imageReady())
            d->imageTextureID = d->imageTexture->bindTexture();
        }
      }
    }

    if(d->imageTextureID>0)
    {
      tp_maps::ImageShader* shader = layer()->map()->getShader<tp_maps::ImageShader>();
      if(shader->error())
        return;

      if(d->updateVertexBuffer)
      {
        d->updateVertexBuffer=false;

        float w = 1.0f;
        float h = 1.0f;
        float x = 0.0f;
        float y = 0.0f;

        if(d->imageTextureID>0)
        {
          glm::vec2 t = d->imageTexture->textureDims();

          std::vector<tp_maps::ImageShader::Vertex> verts;
          verts.push_back(tp_maps::ImageShader::Vertex({w,y,0.5f}, {0,0,1}, { t.x, 0.0f}));
          verts.push_back(tp_maps::ImageShader::Vertex({w,h,0.5f}, {0,0,1}, { t.x,  t.y}));
          verts.push_back(tp_maps::ImageShader::Vertex({x,h,0.5f}, {0,0,1}, {0.0f,  t.y}));
          verts.push_back(tp_maps::ImageShader::Vertex({x,y,0.5f}, {0,0,1}, {0.0f, 0.0f}));
          std::vector<GLuint> indexes{3,2,1,0};

          delete d->imageVertexBuffer;
          d->imageVertexBuffer = shader->generateVertexBuffer(layer()->map(), indexes, verts);
        }
      }

      shader->use();
      shader->setMatrix(glm::scale(m,{width(), height(), 1.0f}));

      if(d->imageTextureID>0 && d->imageVertexBuffer)
      {
        shader->setTexture(d->imageTextureID);
        shader->draw(GL_TRIANGLE_FAN, d->imageVertexBuffer, {1.0f, 1.0f, 1.0f,1.0f});
      }
    }
  }
}

//##################################################################################################
void Image::invalidateBuffers()
{
  d->bindBeforeRender=true;
  d->imageTextureID=0;

  Widget::invalidateBuffers();
}

//##################################################################################################
bool Image::mouseEvent(const tp_maps::MouseEvent& event)
{
  switch(event.type)
  {
  case tp_maps::MouseEventType::Press:
  {
    d->currentVisualModifier = VisualModifier::Pressed;
    update();
    break;
  }

  case tp_maps::MouseEventType::Move:
  {
    break;
  }

  case tp_maps::MouseEventType::Release:
  {
    if(d->currentVisualModifier == VisualModifier::Pressed)
    {
      d->currentVisualModifier = VisualModifier::Normal;
      clicked();
      update();
    }
    break;
  }

  case tp_maps::MouseEventType::Wheel:
  {
    break;
  }

  case tp_maps::MouseEventType::DoubleClick:
  {
    break;
  }

  case tp_maps::MouseEventType::Click:
  {
    break;
  }
  }

  return Widget::mouseEvent(event);
}

//##################################################################################################
void Image::animate(double timestampMS)
{
  Widget::animate(timestampMS);
}

}
