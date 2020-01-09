#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

GLuint VAO, VBO, shader;

// Vertex Shader
static const char* vShader = "                                  \n\
#version 330                                                    \n\
                                                                \n\
layout (location = 0) in vec3 pos;                              \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
  gl_Position = vec4(0.4*pos.x, 0.4*pos.y, pos.z, 1.0);         \n\
}";
// gl_Position = vec4(pos.x, pos.y, pos.z, 1.0) This will draw a tri with the whole screen


// Fragment Shader
// Creating vec4 which is basically rgd and alpha
static const char* fShader = "                                  \n\
#version 330                                                    \n\
                                                                \n\
out vec4 colour;                                                \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
  colour = vec4(1.0, 0.0, 0.0, 1.0);                            \n\
}";

void CreateTriangle()
{
  GLfloat vertices[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
  };

  // Creating VAO
  // Graphic card is creating memory for 1 vertex array and its giving you the ID
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

    // Creating a buffer object
    // All below created VBO is come under the above binded VAO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      // GL_STATIC_DRAW -> We won't change the values in the array once we pass

      // index -> We'll create a layout in the shader with ID 0. That is the index here
      // size -> x,y,z = 3
      // type ->  Type of the above values, that'll be a GLFloat
      // stride -> 0 because we're not going to skip any values from the vertices. Same color for all vertices
      // pointer -> Basically the offset. That is starting from the beignning (0)
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
      // First value in the glVertexAttribPointer - layout value Enabling that layout
      glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Undoing what we did in line 23
  glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
  // Creating the induvidual shader and stores the ID in theShader
  GLuint theShader = glCreateShader(shaderType);

  // Store the code itself
  const GLchar* theCode[1];
  theCode[0] = shaderCode;

  GLint codeLength[1];
  codeLength[0] = strlen(shaderCode);

  // Grad the shader code and put it into the shader we created (theShader)
  // count -> We have just one file for passing in -> 1
  glShaderSource(theShader, 1, theCode, codeLength);
  glCompileShader(theShader);

  // Getting the error code from the shader compilation (similar to shader creationg errorfetch)
  GLint result = 0;
  GLchar eLog[1024] = { 0 };

  glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
  if (!result) {
    glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
    printf("Erroe compiling the %d shader program: '%s'\n", shaderType, eLog);
    return;
  }

  // Now that the shader is compiled, we're now attaching it to the program
  glAttachShader(theProgram, theShader);

}

void CompileShaders()
{
  shader = glCreateProgram();

  // Make sure the shader was created correctly
  if (!shader) {
    printf("Error creating shader program");
    // Need to actually exit the program in a better way
    return;
  }

  // Add the shaders to the program
  AddShader(shader, vShader, GL_VERTEX_SHADER);
  AddShader(shader, fShader, GL_FRAGMENT_SHADER);

  // Getting the error code from the shader creation
  GLint result = 0;
  // Place to log the error (1024 chars)
  GLchar eLog[1024] = { 0 };

  // Creating the executables on the gfx
  glLinkProgram(shader);
  glGetProgramiv(shader, GL_LINK_STATUS, &result);

  // Detailed output of the GL link program
  if (!result) {
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Erroe linking program: '%s'\n", eLog);
    return;
  }

  // Checking if the created shader is valid in the current context
  glValidateProgram(shader);
  glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Erroe validating program: '%s'\n", eLog);
    return;
  }
}

int main()
{
  // Initialize GLFW
  if (!glfwInit()) {
    printf("GLFW initialization failed");
    glfwTerminate();
    return 1;
  }

  // Setup GLFW window properties
  // OpenGL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Core profile = No Backward compatibility
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


  // Creating the GLFW window
  GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Maiden OpenGL", NULL, NULL);
  if (!mainWindow) {
    printf("GLFW window creation failed");
    glfwTerminate();
    return 1;
  }

  // Get buffer size information
  int bufferWidth, bufferHeight;
  glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

  // Set context for GLEW to use
  glfwMakeContextCurrent(mainWindow);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK) {
    printf("GLEW initialization failed");
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 1;
  }

  // Setup viewport size
  glViewport(0, 0, bufferWidth, bufferHeight);

  CreateTriangle();
  CompileShaders();

  while (!glfwWindowShouldClose(mainWindow)) {
    
    // Get & Handle user input events
    glfwPollEvents();

    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Grab the id and go to the gfx and use that one
    glUseProgram(shader);

    glBindVertexArray(VAO);
    // mode ->  It could be line, triangle etc. 
    //  We chose tri because then openGL will know to add color in the middle of the 3 lines
    // first -> Where in the array we need to start = 0
    // count -> How many points are we drawing
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Unbinding the above VAO
    glBindVertexArray(0);

    // Unassigning the shader
    glUseProgram(0);

    // Swap the buffers from where we drew to what we see
    glfwSwapBuffers(mainWindow);
  }

  return 0;
}