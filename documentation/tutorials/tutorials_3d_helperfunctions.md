:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# 3D helper functions

## Overview
After Opening the OpenGL display, native OpenGL functions can be used, such as glRotate and glDrawElements. Another options is to use the helper functions provided by the EDGELIB SDK. The advantage is that most of them will also work with the internal renderer. These functions use fixed point math where possible.

## Rendering models
To load and render 3D models easily, you can use the E3DSurface class. Here is an example on how to load a model stored in a MilkShape 3D file (model is a class member variable of type E3DSurface):

```c++
if (display->CreateSurface(&model, "model.ms3d") != E_OK) 
    return(E_ERROR);
```

The following example opens a 3D perspective and renders it onscreen with gouraud shading:


```c++
#if defined(EGL_USEGL) 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
#else 
    display->buffer.Clear(); 
#endif 
display->Perspective3D(display->GetWidth(), display->GetHeight()); 
display->SetShading(E3D_GOURAUD); 
display->Render(&model);
```

## Lighting
In OpenGL the model isn't visible, because there is no light source. Both OpenGL and the EDGELIB internal renderer supports up to 8 light sources. The following example sets up a light source with diffuse and ambient lighting respectively:

```c++
display->SetLightSource(0, 4096, 4096, 4096, ECOLOR_WHITE, E_RGBX(32, 32, 32)); 
display->UseLight(0);
```

This light source is positioned over your right shoulder. Diffuse lighting is a directional light source, the model polygons facing at the right angle will be the brightest. Ambient lighting is everywhere, each polygon is lit by the same amount.

## World and model transformation
World transformation is like a camera which can take a different position and rotate around an area. It's possible to translate (move), rotate and scale. It's also possible to transform a model individually. In most cases the camera needs to move backwards to properly render the model. The following example places 4 models in a circle around the world. The world is rotated by a timer and the camera moves backwards to show all models.

```c++
#if defined(EGL_USEGL) 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
#else 
    display->buffer.Clear(); 
#endif 
rotation += timedelta; 
display->Perspective3D(display->GetWidth(), display->GetHeight()); 
model1.Reset(); 
model1.Translate(-4096, 0, 0); 
model2.Reset(); 
model2.Translate(4096, 0, 0); 
model3.Reset(); 
model3.Translate(0, 0, -4096); 
model4.Reset(); 
model4.Translate(0, 0, 4096); 
display->WorldReset(); 
display->WorldTranslate(0, 0, -4096); 
display->WorldRotate(0, rotation, 0); 
display->SetShading(E3D_GOURAUD); 
display->Render(&model1); 
display->Render(&model2); 
display->Render(&model3); 
display->Render(&model4);
```

## Animation
If available, model animations are stored in a 3D surface. To load animated models, use MilkShape 3D (.ms3d) files. Animations are stored as joints, their movements are stored as keyframes. This is also known as skeletal animation.

Here is an example which animates a model before rendering it onscreen. Models animation is based on time, instead of frames.

```c++
animatetimer += timedelta; 
model.ResetAnimation(); 
model.Animate(animatetimer); 
display->Render(&model);
```

## Uploading and binding textures
To use texture mapping, upload a 2D (RGBA) surface to OpenGL and bind it before rendering. When using the internal 3D EDGELIB renderer, the upload function isn't needed and does nothing. Almost all OpenGL implementations require the texture to be a speficic size and must be a power of 2. For example, textures of 256x256 and 64x128 pixels are supported. A texture of 96x128 is not supported because the width isn't a power of 2. Also, some implementations may not require texture sizes bigger than 256x256, like the implementation on the Nokia E60 device.

If a texture with an odd size is uploaded, it will be padded by EDGELIB. This may give incorrect texture mapping results when rendering 3D models. The following example loads a texture, uploads it to OpenGL and renders a model with texture mapping (texture is a class member variable of type `E2DSurface`):

```c++
//On display init 
display->Perspective3D(display->GetWidth(), display->GetHeight()); 
if (display->CreateSurface(&texture, "texture.png") == E_OK) 
    display->UploadTexture(&texture); 
display->BindTexture(&texture); 
display->SetShading(E3D_GOURAUD | E3D_TEXTURE); 
 
//On next frame 
display->Render(&model);
```

## Mix 2D and 3D rendering
In many cases it's desired to combine 2D and 3D rendering, for example displaying game status or writing text in a 3D game. Another option is to program an entire game using hardware accelerated OpenGL.

The following example writes an FPS counter using the internal font. This font is automatically uploaded by EDGELIB:

```c++
display->Perspective2D(display->GetWidth(), display->GetHeight(), false); 
display->WriteFont(0, 0, &display->fontinternal, "Hello world");
```

Other 2D functions like BltFast and FillRect are also available. When not using OpenGL, these 2D functions will render on the backbuffer.

## Chapters
* [OpenGL (ES) implementations](tutorials_3d_opengl.md)
* **3D helper functions**
* [Advanced 3D functions](tutorials_3d_advanced.md)

