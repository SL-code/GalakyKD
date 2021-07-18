#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

static unsigned int CompileShader(unsigned int type,
                                  const std::string &source) {

  unsigned int id = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile" << '\n';
    std::cout << message << '\n';
    glDeleteShader(id);
    return 0;
  }

  return id;
}

static unsigned int CreateShader(const std::string &vertexShader,
                                 const std::string &fragmentShader) {

  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}


static int windowWidth = 640,  windowHeight = 480;
void window_size_callback(GLFWwindow* window, int width, int height)
{
  windowWidth = width;
  windowHeight= height;
  glViewport(0, 0, windowWidth, windowHeight);
}

int main() {
  // Initialze GLFW - if not seccesful exit
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }
  // Create a windows using GLFW
  GLFWwindow *window = glfwCreateWindow(windowWidth,  windowHeight, "GalaxyKD", NULL, NULL);

  // Make the context of the window the current glContext
  glfwMakeContextCurrent(window);

  // Extract modern OpenGL using GLEW
  glewInit();

  int numberOfLanes = 5;
  float gridVertical[numberOfLanes + 1][4];
  for(int i = 0; i <= numberOfLanes; ++i)
  {
    float x1 = -1.0 + 2.0 * (float)(i)/ (float)(numberOfLanes);
    float y1 = -1.0;
    float x2 = -1.0 + 2.0 * (float)(i)/ (float)(numberOfLanes);
    float y2 =  1.0;
    gridVertical[i][0] = x1;
    gridVertical[i][1] = y1;
    gridVertical[i][2] = x2;
    gridVertical[i][3] = y2;
  }

  int numberOfBlocksInFront = 3;
  float gridHorizontal[numberOfBlocksInFront + 1][4];
  for(int i = 0; i <= numberOfBlocksInFront; ++i)
  {
    float x1 = -1.0;
    float y1 = -1.0 + 2.0 * (float)(i)/ (float)(numberOfBlocksInFront);
    float x2 =  1.0;
    float y2 = -1.0 + 2.0 * (float)(i)/ (float)(numberOfBlocksInFront);
    gridHorizontal[i][0] = x1;
    gridHorizontal[i][1] = y1;
    gridHorizontal[i][2] = x2;
    gridHorizontal[i][3] = y2;
  }
  float ship[6] = {-0.1, -0.9,
                   0.0, -0.5,
                   0.1, -0.9};
  
  // Set up a Vertex Buffer for rendering
  unsigned int shipBuffer;
  glGenBuffers(1, &shipBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, shipBuffer);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &ship[0],
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
  glEnableVertexAttribArray(0);

  unsigned int gridV;
  glGenBuffers(2, &gridV);
  glBindBuffer(GL_ARRAY_BUFFER, gridV);
  glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertical) * sizeof(float), &gridVertical[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
  glEnableVertexAttribArray(1);

  unsigned int gridH;
  glGenBuffers(2, &gridH);
  glBindBuffer(GL_ARRAY_BUFFER, gridH);
  glBufferData(GL_ARRAY_BUFFER, sizeof(gridHorizontal) * sizeof(float), &gridHorizontal[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
  glEnableVertexAttribArray(1);

  std::string vertexShader = "#version 330 core\n"
                             "\n"
                             "layout(location = 0) in vec4 position;\n"
                             "void main ()\n"
                             "{\n"
                             "   gl_Position = position;\n"
                             "}\n";

  std::string shipFragmentShader = "#version 330 core\n"
                               "\n"
                               "layout(location = 0) out vec4 color;\n"
                               "void main ()\n"
                               "{\n"
                               "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
                               "}\n";


  std::string gridFragmentShader = "#version 330 core\n"
                               "\n"
                               "layout(location = 0) out vec4 color;\n"
                               "void main ()\n"
                               "{\n"
                               "   color = vec4(1.0, 1.0, 1.0, 1.0);\n"
                               "}\n";

  unsigned int shipShader = CreateShader(vertexShader, shipFragmentShader);
  unsigned int gridShader = CreateShader(vertexShader, gridFragmentShader);

  glViewport(0, 0, windowWidth, windowHeight);
  // Render loop
  while (!glfwWindowShouldClose(window)) {
    //
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(gridShader);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertical) * sizeof(float), &gridVertical[0],
                 GL_STATIC_DRAW);
    glDrawArrays(GL_LINES, 0, 22);

    glBufferData(GL_ARRAY_BUFFER, sizeof(gridHorizontal) * sizeof(float), &gridHorizontal[0],
                 GL_STATIC_DRAW);
    glDrawArrays(GL_LINES, 0, 18);



    glUseProgram(shipShader);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &ship[0],
                 GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 3);







    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for process events
    glfwPollEvents();
    glfwSetWindowSizeCallback(window, window_size_callback);
  }


  glDeleteProgram(shipShader);
  glDeleteProgram(gridShader);
  glfwDestroyWindow(window);
  glfwTerminate();
  std::cout << "Good Bye, Cruel World!" << '\n';
  return 0;
}
