
#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <cglm/struct.h>
#include <glib-object.h>

typedef struct {
    SDL_Window *window;
    SDL_GLContext gl_context;
} AppContext;

SDL_AppResult SDL_Fail() {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error %s", SDL_GetError());
    return SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return SDL_Fail();
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

    SDL_Window* window = SDL_CreateWindow("OpenGL ES Window", 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        return SDL_Fail();
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        return SDL_Fail();
    }

    if (!gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress)) {
        return SDL_Fail();
    }


    AppContext *context = g_new(AppContext, 1);
    context->window = window;
    context->gl_context = gl_context;

    *appstate = context;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate){
    AppContext *app = (AppContext*)appstate;

    glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    SDL_GL_SwapWindow(app->window);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result){
    AppContext *app = (AppContext*)appstate;
    if (app) {
        SDL_GL_DestroyContext(app->gl_context);
        SDL_DestroyWindow(app->window);
        g_free(app);
    }
    SDL_Quit();
}
