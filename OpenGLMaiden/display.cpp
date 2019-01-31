// Author: pampas_83
// abhijit.93@hotmail.com

#include "display.h"
#include "GL\glew.h"

#include <iostream>

Display::Display(int width, int height, const std::string& title)
{
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); // Sets 2^8 = 256 shades of red (8 bits)
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32); // SDL will allocate 32bits for each pixel. 
                                               // 32 because 8*4, meaning each pixel can use all above 4 colors
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Executing double buffering

  /* 
  SDL_CreateWindow takes a C string, not C++
  SDL_WIDNOWPOS_CENTERED is used to show the window in the center height and width of screen
  SDL_WINDOW_OPENGL flag is used to let the SDL window know that we're using OpenGL to draw the window
  Since we're using OpenGL to draw window, we can set attributes (see above)
  */
  m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

  /*
  SDL_GLContext is used to let the OS know that GPU is going take power over the window (window which OS created
  because OpenGL speaks to the GPU. So now, indirectly, opengl can control the window.
  */
  m_glContext = SDL_GL_CreateContext(m_window);

  // Going to search OS and find every Opengl function we support
  GLenum status = glewInit();

  if (status != GLEW_OK) {
    std::cerr << "Glew failed to inistalize" << std::endl;
  }
}

Display::~Display()
{
  SDL_GL_DeleteContext(m_glContext);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

void Display::Update()
{
  SDL_GL_SwapWindow(m_window);

  // Looks for any OS events (SDL_PollEvent)
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      m_isClosed = true;
    }
  }
}

void Display::SwapBuffers()
{
  SDL_GL_SwapWindow(m_window);
}

bool Display::IsClosed()
{
  return m_isClosed;
}

void Display::Clear(float r, float g, float b, float a)
{
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT);
}
