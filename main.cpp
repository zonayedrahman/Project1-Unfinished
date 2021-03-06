#define GL_SILENCE_DEPRECATION




#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, x_wing, death_star, projectionMatrix;

//glm::mat4 death_star;

float player_x = 0;
float player_rotate = 0;


GLuint x_wing_texture_ID;
GLuint death_star_textureID;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(image);
    return textureID;
}



void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 1!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

 

    viewMatrix = glm::mat4(1.0f);
    x_wing = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    death_star = glm::mat4(1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    x_wing_texture_ID = LoadTexture("x_wing.png");
    death_star_textureID = LoadTexture("death_star.png");
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

float lastTicks = 0.0f;

void Update() 
{   
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    if (player_x <= 6) {
        player_x += 1.0f * deltaTime;
        player_rotate += 90.0f * deltaTime;

    }
    
    x_wing = glm::mat4(1.0f);
    //x_wing = glm::translate(x_wing, glm::vec3(-5.0f, -1.0f, 0.0f));
    //x_wing = glm::rotate(x_wing, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //x_wing = glm::rotate(x_wing, glm::radians(player_rotate), glm::vec3(0.5f, 0.0f, 0.0f));
    //x_wing = glm::translate(x_wing, glm::vec3( 0.0f, -(player_x), 0.0f));

    death_star = glm::mat4(1.0f);
    death_star = glm::scale(death_star, glm::vec3(1.5f, 1.5f, 1.0f));
    death_star = glm::translate(death_star, glm::vec3(2.0f, 0.0f, 0.0f));
    //second_modelMatrix = glm::rotate(second_modelMatrix, 90.0f, glm::vec3(0.0f, 0.5f, 0.0f));

}


void Draw_x_wing() {
    program.SetModelMatrix(x_wing);
    glBindTexture(GL_TEXTURE_2D, x_wing_texture_ID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Draw_death_star() {
    program.SetModelMatrix(death_star);
    glBindTexture(GL_TEXTURE_2D, death_star_textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void Render() {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };


    glClear(GL_COLOR_BUFFER_BIT);

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    Draw_x_wing();
    Draw_death_star();


    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}