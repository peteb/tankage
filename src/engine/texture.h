#ifndef ENGINE_TEXTURE_H
#define ENGINE_TEXTURE_H

#include <utils/rect.h>

class Texture {
public:
  virtual ~Texture() {}

  virtual void bind() =0;
  virtual rect size() const =0;
  virtual void setFiltering(bool smooth) =0;
};

#endif // !ENGINE_TEXTURE_H
