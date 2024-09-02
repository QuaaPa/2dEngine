#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Graphics.h"
#include "../pch.h"

using uint = unsigned int;


class FrameBuffer
{
public:
    static void createFramebuffer(int width, int height);

    static void bindFramebuffer();

    static void unbindFramebuffer();

    static void rescaleFramebuffer(float width, float height);

    static void clearBuffer();

    static uint texture_id;

private:
    static uint FBO;
    static uint RBO;
};

#endif // FRAMEBUFFER_H