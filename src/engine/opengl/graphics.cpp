#include <engine/opengl/graphics.h>
#include <engine/opengl/opengl.h>
#include <engine/texture.h>
#include <engine/image.h>
#include <utils/color.h>
#include <stdexcept>

namespace {
class OpenGlTexture : public Texture {
public:
  OpenGlTexture(GLuint id) : _id(id) {}
  ~OpenGlTexture() {
    glBindTexture(GL_TEXTURE_2D, 0); // make sure it's not the active texture
    glDeleteTextures(1, &_id);
  }

  void bind() {
    glBindTexture(GL_TEXTURE_2D, _id);
  }

private:
  GLuint _id;
};
}

void OpenGl::Graphics::clear(const color4 &clearColor) {
  glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

class Texture *OpenGl::Graphics::createTexture(Image *image) {
  GLenum format, type;
  GLuint texId;

  switch (image->format()) {
  case Image::IMAGE_RGB:
    format = GL_RGB;
    break;

  case Image::IMAGE_RGBA:
    format = GL_RGBA;
    break;

  case Image::IMAGE_BGR:
    format = GL_BGR;
    break;

  case Image::IMAGE_BGRA:
    format = GL_BGRA;
    break;

  case Image::IMAGE_LUMINANCE:
    format = GL_LUMINANCE;
    break;

  default:
    throw std::runtime_error("opengl: unsupported image format");
  }
  
  switch (image->dataType()) {
  case Image::IMAGE_UNSIGNED_BYTE:
    type = GL_UNSIGNED_BYTE;
    break;

  default:
    throw std::runtime_error("opengl: unsupported image data type");
  }

  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &texId);
  glBindTexture(GL_TEXTURE_2D, texId);
  glTexImage2D(GL_TEXTURE_2D, 0,
               image->bytesPerPixel(),
               image->size().width(),
               image->size().height(),
               1,
               format,
               type,
               image->data()
               );
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  
  return new OpenGlTexture(texId);
}

void OpenGl::Graphics::drawQuad(const rect &quad) {
  vec2 min, max;
  quad.getCoords(min, max);

  glEnable(GL_COLOR_MATERIAL);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(min.x, min.y);

  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(max.x, min.y);

  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(max.x, max.y);

  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(min.x, max.y);
  glEnd();
}

void OpenGl::Graphics::setOrtho(const rect &size) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  const float scaleX = 1.0f / size.halfSize.x;
  const float scaleY = -1.0f / size.halfSize.y;
  
  glTranslatef(-1.0f, 1.0f, 0.0f);
  glScalef(scaleX, scaleY, 1.0f);
  glTranslatef(0.5f, 0.5f, 0.0f);
  
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}

void OpenGl::Graphics::setViewport(const class rect &size) {
  vec2 min, max;
  size.getCoords(min, max);
  min += size.halfSize;
  max += size.halfSize;
  
  glViewport(static_cast<int>(min.x),
             static_cast<int>(min.y),
             static_cast<int>(max.x - min.x),
             static_cast<int>(max.y - min.y)
    );
}

void OpenGl::Graphics::setBlend(BlendMode mode) {
  switch (mode) {
  case Graphics::BLEND_NONE:
    glDisable(GL_BLEND);
    return;

  case Graphics::BLEND_ALPHA:
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    break;

  case Graphics::BLEND_ADDITIVE:
    glBlendFunc(GL_ONE, GL_ONE);
    break;
  }

  glEnable(GL_BLEND);
}

void OpenGl::Graphics::enableTextures() {
  glEnable(GL_TEXTURE_2D);
}

void OpenGl::Graphics::disableTextures() {
  glDisable(GL_TEXTURE_2D);
}

