#include <rend/rend.h>
#include <sr1/memory>
#include <SDL2/SDL.h>

#include <iostream>

using namespace rend;

const char* src =
"\n#ifdef VERTEX\n" \
"attribute vec3 in_Position;" \
"attribute vec4 in_Color;" \
"" \
"varying vec4 ex_Color;" \
"void main()" \
"{" \
"  gl_Position = vec4(in_Position, 1);" \
"  ex_Color = in_Color;" \
"}" \
"" \
"\n#endif\n" \
"\n#ifdef FRAGMENT\n" \
"varying vec4 ex_Color;" \
"" \
"void main()" \
"{" \
"  gl_FragColor = ex_Color;"\
"}" \
"" \
"\n#endif\n";

int main()
{
  std::cout << "Rend Test" << std::endl;

  SDL_Window* window = SDL_CreateWindow("Rend Test",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    800, 600,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if(!window)
  {
    throw Exception("Failed to create window");
  }

  SDL_GLContext glContext = SDL_GL_CreateContext(window);

  if(!glContext)
  {
    throw Exception("Failed to create OpenGL context");
  }

  std::sr1::shared_ptr<Context> context = Context::initialize();
  std::sr1::shared_ptr<Shader> shader = context->createShader();

  shader->setSource(src);

  std::sr1::shared_ptr<Buffer> shape = context->createBuffer();
  shape->add(vec3(-0.5f, 0.5f,0));
  shape->add(vec3(-0.5f, -0.5f,0));
  shape->add(vec3(0.5f, -0.5f,0));

  std::sr1::shared_ptr<Buffer> colour = context->createBuffer();
  colour->add(vec4(1.0f, 0, 0,1.0f));
  colour->add(vec4(0, 1.0f, 0, 1.0f));
  colour->add(vec4(0, 0, 1.0f,1.0f));

  bool running = true;
  SDL_Event e = {0};

  while(running)
  {
    while(SDL_PollEvent(&e) != 0)
    {
      if(e.type == SDL_QUIT)
      {
        running = false;
      }
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->setAttribute("in_Position", shape);
	shader->setAttribute("in_Color", colour);

    shader->render();

    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

