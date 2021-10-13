#include "tp_maps_ui/widgets/PushButton.h"
#include "tp_maps_ui/layers/UILayer.h"
#include "tp_maps_ui/DrawHelper.h"

#include "tp_maps/Map.h"
#include "tp_maps/MouseEvent.h"
#include "tp_maps/shaders/FontShader.h"
#include "tp_maps/shaders/ImageShader.h"
#include "tp_maps/textures/BasicTexture.h"

#include "glm/gtx/transform.hpp"

#include <memory>

namespace tp_maps_ui
{

//##################################################################################################
struct PushButton::Private
{
  TP_REF_COUNT_OBJECTS("tp_maps_ui::PushButton::Private");
  TP_NONCOPYABLE(Private);
  Private() = default;

  std::u16string text;
  std::unique_ptr<tp_maps::FontShader::PreparedString> preparedString;

  tp_image_utils::ColorMap normalImage;
  tp_image_utils::ColorMap pressedImage;
  std::unique_ptr<tp_maps::BasicTexture> normalImageTexture;
  std::unique_ptr<tp_maps::BasicTexture> pressedImageTexture;

  tp_maps::ImageShader::VertexBuffer* normalImageVertexBuffer{nullptr};
  tp_maps::ImageShader::VertexBuffer* pressedImageVertexBuffer{nullptr};

  //This represents the button state, normal, hover, presses, etc...
  VisualModifier currentVisualModifier{VisualModifier::Normal};

  GLuint normalImageTextureID{0};
  GLuint pressedImageTextureID{0};

  bool regenerateText{true};
  bool bindBeforeRender{false};
  bool updateVertexBuffer{true};
};

//##################################################################################################
PushButton::PushButton(Widget* parent):
  Widget(parent),
  d(new Private())
{

}

//##################################################################################################
PushButton::PushButton(const std::u16string& text, Widget* parent):
  Widget(parent),
  d(new Private())
{
  setText(text);
}

//##################################################################################################
PushButton::~PushButton()
{
  delete d;
}

//##################################################################################################
const std::u16string& PushButton::text() const
{
  return d->text;
}

//##################################################################################################
void PushButton::setText(const std::u16string& text)
{
  d->text = text;
  d->regenerateText = true;
  update();
}

//################################################################################################
void PushButton::setImage(const tp_image_utils::ColorMap& normalImage, const tp_image_utils::ColorMap& pressedImage)
{
  d->normalImage  = normalImage.clone2();
  d->pressedImage = pressedImage.clone2();

  d->bindBeforeRender = true;
}

//##################################################################################################
void PushButton::render(tp_maps::RenderInfo& renderInfo)
{
  TP_UNUSED(renderInfo);

  auto m = matrix();

  //Draw the button background.
  if(drawHelper())
  {
    drawHelper()->drawBox(m, width(), height(), BoxType::Raised, FillType::Button, d->currentVisualModifier);
  }

  //Draw the text.
  if(font() && !d->text.empty())
  {
    auto shader = layer()->map()->getShader<tp_maps::FontShader>();
    if(shader->error())
      return;

    if(d->regenerateText)
    {
      tp_maps::PreparedStringConfig config;
      config.topDown = true;
      d->preparedString.reset(new tp_maps::FontShader::PreparedString(font(), d->text, config));
    }

    if(!d->preparedString)
      return;

    auto color = drawHelper()->textColor(BoxType::Raised, FillType::Button, d->currentVisualModifier);

    shader->use();
    shader->setMatrix(glm::translate(m, glm::floor(glm::vec3(width()/2.0f, height()/2.0f, 0.0f))));
    shader->setColor(color);
    shader->drawPreparedString(*d->preparedString.get());
  }

  //Draw image
  {
    if(d->bindBeforeRender)
    {
      d->bindBeforeRender=false;
      d->updateVertexBuffer=true;

      {
        layer()->map()->deleteTexture(d->normalImageTextureID);
        d->normalImageTextureID = 0;

        if(d->normalImage.size()>0)
        {
          if(!d->normalImageTexture)
            d->normalImageTexture.reset(new tp_maps::BasicTexture(layer()->map()));
          d->normalImageTexture->setImage(d->normalImage);

          if(d->normalImageTexture->imageReady())
            d->normalImageTextureID = d->normalImageTexture->bindTexture();
        }
      }

      {
        layer()->map()->deleteTexture(d->pressedImageTextureID);
        d->pressedImageTextureID = 0;

        if(d->pressedImage.size()>0)
        {
          if(!d->pressedImageTexture)
            d->pressedImageTexture.reset(new tp_maps::BasicTexture(layer()->map()));
          d->pressedImageTexture->setImage(d->pressedImage);

          if(d->pressedImageTexture->imageReady())
            d->pressedImageTextureID = d->pressedImageTexture->bindTexture();
        }
      }
    }

    if(d->normalImageTextureID>0 || d->pressedImageTextureID>0)
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

        if(d->normalImageTextureID>0)
        {
          glm::vec2 t = d->normalImageTexture->textureDims();

          std::vector<tp_maps::ImageShader::Vertex> verts;
          verts.push_back(tp_maps::ImageShader::Vertex({w,y,0.5f}, {0,0,1}, { t.x, 0.0f}));
          verts.push_back(tp_maps::ImageShader::Vertex({w,h,0.5f}, {0,0,1}, { t.x,  t.y}));
          verts.push_back(tp_maps::ImageShader::Vertex({x,h,0.5f}, {0,0,1}, {0.0f,  t.y}));
          verts.push_back(tp_maps::ImageShader::Vertex({x,y,0.5f}, {0,0,1}, {0.0f, 0.0f}));
          std::vector<GLuint> indexes{3,2,1,0};

          delete d->normalImageVertexBuffer;
          d->normalImageVertexBuffer = shader->generateVertexBuffer(layer()->map(), indexes, verts);
        }

        if(d->pressedImageTextureID>0)
        {
          glm::vec2 t = d->pressedImageTexture->textureDims();

          std::vector<tp_maps::ImageShader::Vertex> verts;
          verts.push_back(tp_maps::ImageShader::Vertex({w,y,0.5f}, {0,0,1}, { t.x, 0.0f}));
          verts.push_back(tp_maps::ImageShader::Vertex({w,h,0.5f}, {0,0,1}, { t.x,  t.y}));
          verts.push_back(tp_maps::ImageShader::Vertex({x,h,0.5f}, {0,0,1}, {0.0f,  t.y}));
          verts.push_back(tp_maps::ImageShader::Vertex({x,y,0.5f}, {0,0,1}, {0.0f, 0.0f}));
          std::vector<GLuint> indexes{3,2,1,0};

          delete d->pressedImageVertexBuffer;
          d->pressedImageVertexBuffer = shader->generateVertexBuffer(layer()->map(), indexes, verts);
        }
      }

      shader->use();
      shader->setMatrix(glm::scale(m,{width(), height(), 1.0f}));

      if(d->currentVisualModifier == VisualModifier::Pressed)
      {
        if(d->pressedImageTextureID>0 && d->pressedImageVertexBuffer)
        {
          shader->setTexture(d->pressedImageTextureID);
          shader->draw(GL_TRIANGLE_FAN, d->pressedImageVertexBuffer, {1.0f, 1.0f, 1.0f, 1.0f});
        }
      }
      else
      {
        if(d->normalImageTextureID>0 && d->normalImageVertexBuffer)
        {
          shader->setTexture(d->normalImageTextureID);
          shader->draw(GL_TRIANGLE_FAN, d->normalImageVertexBuffer, {1.0f, 1.0f, 1.0f,1.0f});
        }
      }
    }
  }
}

//##################################################################################################
void PushButton::invalidateBuffers()
{
  d->bindBeforeRender=true;
  d->normalImageTextureID=0;
  d->pressedImageTextureID=0;

  Widget::invalidateBuffers();
}

//##################################################################################################
bool PushButton::mouseEvent(const tp_maps::MouseEvent& event)
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
void PushButton::animate(double timestampMS)
{
  Widget::animate(timestampMS);
}

}
