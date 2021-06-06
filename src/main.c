#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

typedef struct
{
  GLenum shader_type;
  const GLchar *shader_source;
} shader_info;

const char *get_shader_type_name(GLenum shader_type)
{
  static const char vertex_shader_type_name[] = "vertex";
  static const char fragment_shader_type_name[] = "fragment";
  static const char unknown_shader_type_name[] = "";

  switch (shader_type)
  {
  case GL_VERTEX_SHADER:
    return vertex_shader_type_name;
    break;
  case GL_FRAGMENT_SHADER:
    return fragment_shader_type_name;
    break;
  default:
    return unknown_shader_type_name;
    break;
  }
  return unknown_shader_type_name;
}

GLuint buildShader(GLsizei num_shaders, const shader_info *shaders)
{
  const GLuint program_id = glCreateProgram();

  GLuint *const shader_ids = malloc(num_shaders * sizeof(GLuint));

  for (GLsizei i = 0; i < num_shaders; i++)
  {
    shader_ids[i] = glCreateShader(shaders[i].shader_type);
    glShaderSource(shader_ids[i], 1, &shaders[i].shader_source, NULL);
    glCompileShader(shader_ids[i]);

    GLint compile_success = GL_FALSE;
    glGetShaderiv(shader_ids[i], GL_COMPILE_STATUS, &compile_success);
    if (compile_success == GL_FALSE)
    {
      GLint log_length = 0;
      glGetShaderiv(shader_ids[i], GL_INFO_LOG_LENGTH, &log_length);

      GLchar *const infoLog = malloc(log_length * sizeof(GLchar));
      glGetShaderInfoLog(shader_ids[i], log_length, NULL, infoLog);
      fprintf(stderr, "error: %s shader compilation failed:\n%s\n", get_shader_type_name(shaders[i].shader_type), infoLog);
      free(infoLog);
    }
    else
    {
      glAttachShader(program_id, shader_ids[i]);
    }
  }

  glLinkProgram(program_id);

  GLint link_success = GL_FALSE;
  glGetProgramiv(program_id, GL_LINK_STATUS, &link_success);
  if (link_success == GL_FALSE)
  {
    GLint log_length = 0;
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

    GLchar *const infoLog = malloc(log_length * sizeof(GLchar));
    glGetProgramInfoLog(program_id, log_length, NULL, infoLog);
    fprintf(stderr, "error: shader program link failed:\n%s\n", infoLog);
    free(infoLog);
  };

  for (GLsizei i = 0; i < num_shaders; i++)
    glDeleteShader(shader_ids[i]);

  free(shader_ids);

  return program_id;
}

void window_resize_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
}

int main(int argc, char **argv)
{
  static const GLuint WINDOW_WIDTH = 1024;
  static const GLuint WINDOW_HEIGHT = 768;

  glfwInit();

  GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Template OpenGL Project", NULL, NULL);
  glfwMakeContextCurrent(window);
  if (window == NULL)
  {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }

#ifdef GLAD_GLES
  if (!gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress))
#else
  if (!gladLoadGL())
#endif
  {
    fprintf(stderr, "Failed to initialize OpenGL context\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

  glfwSetWindowSizeCallback(window, window_resize_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetScrollCallback(window, scroll_callback);

#ifdef GLAD_GLES
#define GLSL_VERSION "130"
#else
#define GLSL_VERSION "460"
#endif

  static const shader_info shaders[] =
  {
    {
      GL_VERTEX_SHADER,
      "#version " GLSL_VERSION "\n"
      "out vec2 texCoords;\n"
      "void main()\n"
      "{\n"
      "  texCoords = 2.0 * vec2(gl_VertexID & 1, gl_VertexID >> 1);\n"
      "  gl_Position = vec4(texCoords * 2.0 - 1.0, 0.0, 1.0);\n"
      "}\n"
    },
    {
      GL_FRAGMENT_SHADER,
      "#version " GLSL_VERSION "\n"
      "in vec2 texCoords;\n"
      "out vec4 outputColor;\n"
      "void main()\n"
      "{\n"
      "  outputColor = vec4(texCoords, 0.0, 1.0);\n"
      "}\n"
    },
  };
  static const GLsizei num_shaders = ARRAY_SIZE(shaders);
  const GLuint shader_program = buildShader(num_shaders, shaders);

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  glUseProgram(shader_program);

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}
