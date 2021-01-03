#include "precompile.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Framework
{
    Scope<RendererAPI> RenderCommand::ourRendererAPI = CreateScope<OpenGLRendererAPI>();
}