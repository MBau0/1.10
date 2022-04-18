#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "PeriodicTimer.h"
#include "FrameTimer.h"

#include "Scene.h"
#include "Program.h"
#include "Camera.h"
#include "Entity.h"

#include "ComponentManager.h"

#include <string>

#include "Server.h"
#include "Client.h"

#include "TransformMessage.h"

constexpr GLfloat CLEAR_COLOR[4] = { 0, 0, 0, 0 };

void check_for_gl_error() {
    int r = 0;
    do {
        r = glGetError();
        if(r != 0) {
            std::cout << "GL Error: " << r << '\n';
        }
    } while(r != 0);
}

void start_server() {
    Server server;
    server.start();
    server.main();
    server.stop();
}

int main(int argc, char* argv[]) {
   /* int in;
    std::cin >> in;
    if(in == 1) {
        start_server();
        return 0;
    }*/

    if(argc == 2) {
        start_server();
        return 0;
    }

    Client client;
    client.n_connect();

    std::thread t = std::thread(&Client::n_receieve, &client);
    t.detach();

    glfwInit();

    GLFWwindow* window = glfwCreateWindow(1600, 900, "1.10", nullptr, nullptr);
    glfwSetWindowPos(window, 150, 100);
    glfwMakeContextCurrent(window);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    gl3wInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    PeriodicTimer periodic_timer(200);
    FrameTimer frame_timer;

    Camera::Settings camera_settings;
    Camera camera(window, camera_settings);

    ComponentManager component_manager;

    Scene map;
    map.load_assimp("Data/Models/Map/", "map.obj");
    map.set_transform(Transform(glm::vec3(0, 0, 0)));

    Scene akali;
    akali.load_assimp("Data/Models/Akali/", "akali.dae");
    Entity akali_entity;
    auto akali_transform = component_manager.get_transform();
    akali_transform->_transform.set_scale(glm::vec3(.1, .1, .1));
    akali.set_transform(akali_transform->_transform);

    akali_transform->move(glm::vec3(50, 1, -50));

    Program basic_shader;
    basic_shader.load("Data/Shaders/basic shader.glsl");
    map.attach_program(&basic_shader);
    camera.attach_program(&basic_shader);

    Program texture_shader;
    texture_shader.load("Data/Shaders/texture shader.glsl");
    akali.attach_program(&texture_shader);
    camera.attach_program(&texture_shader);

    camera.set_mode(CAMERA_LOCKED);

    while(!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {

        camera.update();

        component_manager.update();
        akali.set_transform(akali_transform->_transform);
        TransformMessage transform_message(0, akali_transform->_transform.get_position());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, 0, CLEAR_COLOR);

        //draw stuff
        map.draw();
        akali.draw();
        
        glfwSwapBuffers(window);

        frame_timer.update();
        if(periodic_timer.time() == -1) {
            std::string title = "fps: " + std::to_string(frame_timer.get_fps()) + " ft: " + std::to_string(frame_timer.get_frame_time_ms());
            glfwSetWindowTitle(window, title.c_str());
        }

        check_for_gl_error();
        
        glfwPollEvents();

        if(glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            //camera.move_angle((float) xpos, (float) ypos);

            if(glfwGetKey(window, GLFW_KEY_W)) {
                camera.move(CAMERA_FORWARD, frame_timer.get_frame_time_ms());
            }
            if(glfwGetKey(window, GLFW_KEY_S)) {
                camera.move(CAMERA_BACKWARD, frame_timer.get_frame_time_ms());
            }
            if(glfwGetKey(window, GLFW_KEY_A)) {
                camera.move(CAMERA_LEFT, frame_timer.get_frame_time_ms());
            }
            if(glfwGetKey(window, GLFW_KEY_D)) {
                camera.move(CAMERA_RIGHT, frame_timer.get_frame_time_ms());
            }
            if(glfwGetKey(window, GLFW_KEY_Q)) {
                camera.move(CAMERA_DOWN, frame_timer.get_frame_time_ms());
            }
            if(glfwGetKey(window, GLFW_KEY_E)) {
                camera.move(CAMERA_UP, frame_timer.get_frame_time_ms());
            }
            if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
                akali_transform->move(camera.mouse_position_world());
                client.n_send(&transform_message);
            }
        }

    }

    return 0;
}
