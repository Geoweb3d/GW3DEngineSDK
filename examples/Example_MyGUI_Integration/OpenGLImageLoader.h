#pragma once
#include <string>
#include <MyGUI_OpenGLImageLoader.h>

class OpenGLImageLoader_Devil : public MyGUI::OpenGLImageLoader
{
public:
    OpenGLImageLoader_Devil();
    virtual ~OpenGLImageLoader_Devil() {};

    void saveImage(
        int _width, int _height,
        MyGUI::PixelFormat _format,
        void* _texture,
        const std::string& _filename);

    void* loadImage(
        int& _width,
        int& _height,
        MyGUI::PixelFormat& _format,
        const std::string& _filename);
};