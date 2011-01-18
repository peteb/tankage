#ifndef ENGINE_TEXTURE_H
#define ENGINE_TEXTURE_H

class Texture {
public:
  virtual ~Texture() {}

  virtual void bind() =0;
};

#endif // !ENGINE_TEXTURE_H
