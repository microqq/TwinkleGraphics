

#ifndef TW_TEXTURE_H
#define TW_TEXTURE_H

#include "twCommon.h"

namespace TwinkleGraphics
{
class Texture : public Object
{
public:
    Texture();
    virtual ~Texture();

protected:

};

class Texture1D : public Texture
{};

class Texture2D : public Texture
{};

class Texture3D : public Texture
{};

class TextureCube : public Texture
{};
} // namespace TwinkleGraphics

#endif