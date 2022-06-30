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
#include "TransformComponent.h"
#include "BuildingComponent.h"
#include "UnitComponent.h"

#include <string>

#include "Server.h"
#include "Client.h"

#include "TransformMessage.h"

#include "Grid.h"

#include "ImageManager.h"
#include "UI.h"

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

    std::vector<std::shared_ptr<Message>> messages;

    Client client;
    client.n_connect();
    client.set_message_receiver(&messages);

    std::thread t = std::thread(&Client::n_receieve, &client);
    t.detach();
    std::thread t2 = std::thread(&Client::tick_update, &client);
    t2.detach();

    glfwInit();

    GLFWwindow* window = glfwCreateWindow(1600, 900, "1.10", nullptr, nullptr);
    glfwSetWindowPos(window, 150, 100);
    glfwMakeContextCurrent(window);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    gl3wInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    ImageManager imageManager;
    UI ui(window, imageManager.get_abilities());

    PeriodicTimer periodic_timer(200);
    FrameTimer frame_timer;

    CameraSettings camera_settings;
    Camera camera(window, camera_settings);

    ComponentManager component_manager;

    Scene map;
    map.load_assimp("Data/Models/Map/", "map.obj");
    map.set_transform(Transform(glm::vec3(0, 0, 0)));

    Scene akali;
    akali.load_assimp("Data/Models/Akali/", "akali.dae");
    Entity akali_entity;
    auto akali_transform = component_manager.get_transform();
    akali_transform->_transform.set_scale(glm::vec3(.05, .05, .05));

    Scene building;
    building.load_assimp("Data/Models/Box/", "box.obj");
    Entity building_entity;
    building_entity.attach(component_manager.get_building());
    auto building_transform = component_manager.get_transform();
    building_transform->move(glm::vec3(1, 1, 1));

    Program basic_shader;
    basic_shader.load("Data/Shaders/basic shader.glsl");
    map.attach_program(&basic_shader);
    camera.attach_program(&basic_shader);

    Program texture_shader;
    texture_shader.load("Data/Shaders/texture shader.glsl");
    akali.attach_program(&texture_shader);
    camera.attach_program(&texture_shader);
    building.attach_program(&texture_shader);

    Program grid_shader;
    grid_shader.load("Data/Shaders/grid shader.glsl");
    camera.attach_program(&grid_shader);

    camera.set_mode(Camera::LOCKED);

    Grid grid(128, 128);

    bool edit_mode = false;

    while(!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {

        ui.new_frame();

        camera.update();

        component_manager.update();

        if(messages.size() > 0) {
           auto message = std::dynamic_pointer_cast<TransformMessage>(messages[0]);
           akali_transform->move(message->_position);
           std::cout << message->_position.x << " " << message->_position.z << "\n";
           messages.clear();
        }

        akali.set_transform(akali_transform->_transform);
        building.set_transform(building_transform->_transform);

        static bool build_mode = false;
        if (build_mode) {
            auto pos = camera.mouse_position_world();
            building_transform->_transform.set_position(pos);
            building.set_transform(building_transform->_transform);
        }
        
        ui.update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, 0, CLEAR_COLOR);

        //draw stuff
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        map.draw();
        glDisable(GL_CULL_FACE);

        if(edit_mode) grid.draw(glm::vec2(128, 128), &grid_shader);

        akali.draw();

        building.draw();

        ui.draw();
        
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
            camera.rotate((float) xpos, (float) ypos);

            if(glfwGetKey(window, GLFW_KEY_1)) {
                camera.set_mode(Camera::FREE);
            }
            if(glfwGetKey(window, GLFW_KEY_2)) {
                camera.set_mode(Camera::LOCKED);
            }

            if(glfwGetKey(window, GLFW_KEY_W)) {
                camera.move(Camera::FORWARD, frame_timer.get_frame_time_ms());
            }
            if(glfwGetKey(window, GLFW_KEY_S)) {
                camera.move(Camera::BACKWARD, frame_timer.get_frame_time_ms());
            }
            if(glfwGetKey(window, GLFW_KEY_A)) {
                camera.move(Camera::LEFT, frame_timer.get_frame_time_ms());
            }
            if(glfwGetKey(window, GLFW_KEY_D)) {
                camera.move(Camera::RIGHT, frame_timer.get_frame_time_ms());
            }
            if(glfwGetKey(window, GLFW_KEY_Q)) {
                camera.move(Camera::DOWN, frame_timer.get_frame_time_ms());
            }
            if(glfwGetKey(window, GLFW_KEY_E)) {
                camera.move(Camera::UP, frame_timer.get_frame_time_ms());
            }
            
            if (glfwGetKey(window, GLFW_KEY_SPACE)) {
                static PeriodicTimer t(100);
                if (t.alert()) {
                    edit_mode = !edit_mode;
                }
            }

            if (!edit_mode) {
                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
                    //akali_transform->move(camera.mouse_position_world());
                    static PeriodicTimer timer(1000);
                    if (timer.alert()) {
                        auto transform_message = std::make_shared<TransformMessage>(0, camera.mouse_position_world());
                        client.n_send(transform_message);
                    }
                }
            }
            else {
                if (glfwGetKey(window, GLFW_KEY_0)) {
                    grid.load_height_map();
                }
                if (glfwGetKey(window, GLFW_KEY_P)) {
                    grid.save_height_map();
                }
                static Tile tile;
                static bool selected = false;
                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
                    static PeriodicTimer timer(100);
                    if (timer.alert()) {
                        auto pos = camera.mouse_position_world();
                        grid.highlight_tile(pos);
                        tile = grid.get_tile(pos.x, pos.z);
                        std::cout << tile.x << " " << tile.z << "\n";
                        selected = true;
                    }
                }
                if (selected && glfwGetKey(window, GLFW_KEY_UP)) {
                    static PeriodicTimer timer(250);
                    if (timer.alert()) {
                        tile.y += 7.8125f / 2.0f;
                        grid.set_tile(tile);
                    }
                }
                else if (selected && glfwGetKey(window, GLFW_KEY_DOWN)) {
                    static PeriodicTimer timer(250);
                    if (timer.alert()) {
                        tile.y -= 7.8125f / 2.0f;
                        grid.set_tile(tile);
                    }
                }
              
                if (glfwGetKey(window, GLFW_KEY_B)) {
                    build_mode = true;
                }
            }
        }

    }

    t.~thread();
    t2.~thread();

    return 0;
}
