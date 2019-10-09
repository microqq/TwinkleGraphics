#ifndef _VERMILION_H_
#define _VERMILION_H_

#include <stdlib.h>
#include <string.h>
#include <glew/glew.h>


#ifdef _DEBUG
    #include <iostream>
#endif


#define BUFFER_OFFSET(a) ((void*)(a))

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

// Enough mips for 16K x 16K, which is the minumum required for OpenGL 4.x
#define MAX_TEXTURE_MIPS    14

// Each texture image data structure contains an array of MAX_TEXTURE_MIPS
// of these mipmap structures. The structure represents the mipmap data for
// all slices at that level.
struct vglImageMipData
{
    GLsizei width;                              // Width of this mipmap level
    GLsizei height;                             // Height of this mipmap level
    GLsizei depth;                              // Depth pof mipmap level
    GLsizeiptr mipStride;                       // Distance in bytes between mip levels in memory
    GLvoid* data;                               // Pointer to data

    vglImageMipData()
        : data(nullptr)
    {}

    vglImageMipData(const vglImageMipData& copy)
        : width(copy.width)
        , height(copy.height)
        , depth(copy.depth)
        , mipStride(copy.mipStride)
        , data(copy.data)
    {}

    vglImageMipData(vglImageMipData&& copy)
        : width(copy.width)
        , height(copy.height)
        , depth(copy.depth)
        , mipStride(copy.mipStride)
        , data(copy.data)
    {
        copy.data = nullptr;
    }

    ~vglImageMipData()
    {
        data = nullptr;
    }

    vglImageMipData& operator =(const vglImageMipData& mipdata)
    {
        width = mipdata.width;
        height = mipdata.height;
        depth = mipdata.depth;
        mipStride = mipdata.mipStride;
        data = mipdata.data;        
    }
};

// This is the main image data structure. It contains all the parameters needed
// to place texture data into a texture object using OpenGL.
struct vglImageData
{
    GLenum target;                              // Texture target (1D, 2D, cubemap, array, etc.)
    GLenum internalFormat;                      // Recommended internal format (GL_RGBA32F, etc).
    GLenum format;                              // Format in memory
    GLenum type;                                // Type in memory (GL_UNSIGNED_BYTE, etc.)
    GLenum swizzle[4];                          // Swizzle for RGBA
    GLsizei mipLevels;                          // Number of present mipmap levels
    GLsizei slices;                             // Number of slices (for arrays)
    GLsizeiptr sliceStride;                     // Distance in bytes between slices of an array texture
    GLsizeiptr totalDataSize;                   // Complete amount of data allocated for texture
    vglImageMipData mip[MAX_TEXTURE_MIPS];      // Actual mipmap data

    vglImageData()
    {}

    vglImageData(const vglImageData& copy)
        : target(copy.target)
        , internalFormat(copy.internalFormat)
        , format(copy.format)
        , type(copy.type)
        , mipLevels(copy.mipLevels)
        , slices(copy.slices)
        , sliceStride(copy.sliceStride)
        , totalDataSize(copy.totalDataSize)
    {
        swizzle[0] = copy.swizzle[0];
        swizzle[1] = copy.swizzle[1];
        swizzle[2] = copy.swizzle[2];
        swizzle[3] = copy.swizzle[3];

        for(int i = 0; i < mipLevels; i++)
        {
            mip[i].width = copy.mip[i].width;
            mip[i].height = copy.mip[i].height;
            mip[i].depth = copy.mip[i].depth;
            mip[i].mipStride = copy.mip[i].mipStride;

            mip[i].data = copy.mip[i].data;
            // if(copy.mip[i].data != nullptr)
            // {
            //     if(i == 0)
            //     {
            //         mip[i].data = malloc(totalDataSize);
            //     }
            //     else
            //     {
            //         mip[i].data = (char*)(mip[i - 1].data) + mip[i - 1].mipStride;
            //     }

            //     memcpy(mip[i].data, copy.mip[i].data, mip[i].mipStride);
            // }
        }
    }

    vglImageData(vglImageData&& copy)
        : target(copy.target)
        , internalFormat(copy.internalFormat)
        , format(copy.format)
        , type(copy.type)
        , mipLevels(copy.mipLevels)
        , slices(copy.slices)
        , sliceStride(copy.sliceStride)
        , totalDataSize(copy.totalDataSize)
    {
#ifdef _DEBUG
    std::cout << "Vermilion: move constructor executed." << std::endl;
#endif

        swizzle[0] = copy.swizzle[0];
        swizzle[1] = copy.swizzle[1];
        swizzle[2] = copy.swizzle[2];
        swizzle[3] = copy.swizzle[3];

        for(int i = 0; i < mipLevels; i++)
        {
            mip[i].data = copy.mip[i].data;
            mip[i].width = copy.mip[i].width;
            mip[i].height = copy.mip[i].height;
            mip[i].depth = copy.mip[i].depth;
            mip[i].mipStride = copy.mip[i].mipStride;

            copy.mip[i].data = nullptr;
        }
    }

    vglImageData& operator =(const vglImageData& data)
    {
        target = data.target;
        internalFormat = data.internalFormat;
        format = data.format;
        type = data.type;
        mipLevels = data.mipLevels;
        slices = data.slices;
        sliceStride = data.sliceStride;
        totalDataSize = data.totalDataSize;

        swizzle[0] = data.swizzle[0];
        swizzle[1] = data.swizzle[1];
        swizzle[2] = data.swizzle[2];
        swizzle[3] = data.swizzle[3];

        for(int i = 0; i < mipLevels; i++)
        {
            mip[i].data = data.mip[i].data;
            // if(data.mip[i].data != nullptr)
            // {
            //     if(i == 0)
            //     {
            //         mip[i].data = malloc(totalDataSize);
            //     }
            //     else
            //     {
            //         mip[i].data = (char*)(mip[i - 1].data) + mip[i - 1].mipStride;
            //     }

            //     memcpy(mip[i].data, data.mip[i].data, mip[i].mipStride);
            // }

            mip[i].width = data.mip[i].width;
            mip[i].height = data.mip[i].height;
            mip[i].depth = data.mip[i].depth;
            mip[i].mipStride = data.mip[i].mipStride;
        }

        return *this;
    }

    void ReleaseMipData()
    {   
        for(int i = 0; i < mipLevels; i++)
        {
            free(mip[i].data);
            mip[i].data = nullptr;
        }
    }
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _VERMILION_H_ */
