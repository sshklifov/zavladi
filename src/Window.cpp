#include "Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include <new>

struct GraphicsContext {
    GLuint program;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint tex;

    GraphicsContext() : program(0), vao(0), vbo(0), ebo(0), tex(0) {}

    ~GraphicsContext() {
        glDeleteTextures(1, &tex);
        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(program);
    }
};

struct WindowContext {
    Framebuffer* fb;
    GraphicsContext* objs;

    WindowContext() : fb(nullptr), objs(nullptr) {}

    ~WindowContext() {
        delete fb;
        delete objs;
    }
};

void logError(const char* fmt, ...) {
#ifndef NDEBUG
    va_list pack;
    va_start(pack, fmt);
    vfprintf(stderr, fmt, pack);
    va_end(pack);
#else
    (void)fmt;
#endif
}

GLuint compileShader(GLenum type, const char* src) {
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int s = GL_TRUE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &s);
    if (s == GL_FALSE) {
        const int bufsiz = 512;
        char* buf = new(std::nothrow) char[bufsiz];
        if (buf) {
            glGetShaderInfoLog(id, bufsiz, NULL, buf);
            logError("Failed to compile shader (type=%d):\n%s\n", type, buf);
            delete[] buf;
        }
        return 0;
    }
    return id;
}

GLuint createProgram() {
    const char* vertSrc = {
        "#version 330 core\n"
        "layout (location = 0) in vec3 pos;\n"
        "layout (location = 1) in vec2 in_uvs;\n"
        "out vec2 uvs;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(pos, 1.0);\n"
        "   uvs = in_uvs;\n"
        "}\n"
    };

    const char* fragSrc = {
        "#version 330 core\n"
        "uniform sampler2D tex;\n"
        "in vec2 uvs;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = texture(tex, uvs);\n"
        "}\n"
    };

    GLuint vertShader = compileShader(GL_VERTEX_SHADER, vertSrc);
    if (vertShader == 0) {
        return 0;
    }

    GLuint fragShader = compileShader(GL_FRAGMENT_SHADER, fragSrc);
    if (fragShader == 0) {
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    int s = GL_TRUE;
    glGetProgramiv(program, GL_LINK_STATUS, &s);
    if (s == GL_FALSE) {
        const int bufsiz = 512;
        char* buf = new(std::nothrow) char[bufsiz];
        if (buf) {
            glGetProgramInfoLog(program, bufsiz, NULL, buf);
            logError("Failed to link program:\n%s\n", buf);
            delete[] buf;
        }
        return 0;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    return program;
}

GLuint createBufferObject() {
    float vertices[] = {
        1.f, 1.f, 0.f, 1.f, 1.f,
        1.f, -1.f, 0.f, 1.f, 0.f,
        -1.f, -1.f, 0.f, 0.f, 0.f,
        -1.f, 1.f, 0.f, 0.f, 1.f,
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);

    return vbo;
}

GLuint createElementObject() {
    unsigned indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    return ebo;
}

GLuint createTextureObject() {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return tex;
}

GraphicsContext* createGraphicsContext() {
    GraphicsContext* ctx = new(std::nothrow) GraphicsContext;
    if (!ctx) {
        return nullptr;
    }

    GLuint program = createProgram();
    if (program == 0) {
        delete ctx;
        return nullptr;
    }
    ctx->program = program;

    glGenVertexArrays(1, &ctx->vao);
    glBindVertexArray(ctx->vao);

    ctx->vbo = createBufferObject();
    ctx->ebo = createElementObject();
    ctx->tex = createTextureObject();
    

    return ctx;
}

void destroyWindow(GLFWwindow* window) {
    void* user = glfwGetWindowUserPointer(window);
    if (user) {
        WindowContext* wctx = reinterpret_cast<WindowContext*>(user);
        delete wctx;
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

void errorCallback(int code, const char* s) {
    logError("GLFW cb error[code=%d]: %s\n", code, s);
}

GLFWwindow* createWindow(int width, int height) {
    if (!glfwInit()) {
        logError("Failed to init GLFW\n");
        return nullptr;
    }

    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width / 2, height / 2, "Vladi malka pishka", NULL, NULL);
    if (!window) {
        logError("Cannot create window (w=%d h=%d)\n", width, height);
        glfwTerminate();
        return nullptr;
    }
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        logError("Could not load opengl library\n");
        destroyWindow(window);
        return nullptr;
    }
    glViewport(0, 0, width, height);

    WindowContext* wctx = new(std::nothrow) WindowContext;
    if (!wctx) {
        logError("Window context creation failed\n");
        destroyWindow(window);
        return nullptr;
    }
    glfwSetWindowUserPointer(window, wctx);

    Framebuffer* fb = new(std::nothrow) Framebuffer(width, height);
    if (!fb) {
        logError("Failed to create framebuffer handle\n");
        destroyWindow(window);
        return nullptr;
    }
    wctx->fb = fb;

    GraphicsContext* objs = createGraphicsContext();
    if (!objs) {
        logError("Failed to initialize graphics context\n");
        destroyWindow(window);
        return nullptr;
    }
    wctx->objs = objs;

    return window;
}

Framebuffer* getWindowFramebuffer(GLFWwindow* window) {
    void* user = glfwGetWindowUserPointer(window);
    if (user) {
        WindowContext* ctx = reinterpret_cast<WindowContext*>(user);
        return ctx->fb;
    }
    return nullptr;
}

void displayWindowFramebuffer(GLFWwindow* window) {
    void* user = glfwGetWindowUserPointer(window);
    if (user) {
        WindowContext* ctx = reinterpret_cast<WindowContext*>(user);
        Framebuffer* fb = ctx->fb;
        GraphicsContext* objs = ctx->objs;

        glBindTexture(GL_TEXTURE_2D, objs->tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fb->getWidth(), fb->getHeight(), 0, GL_RGB, GL_FLOAT, fb->getData());
        glUseProgram(objs->program);
        glBindVertexArray(objs->vao);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
        glfwSwapBuffers(window);
    }
}
