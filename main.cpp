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

#include "EntityManager.h"

#include "UnitSelection.h"
#include "MessageHandler.h"

#include "ProgramManager.h"
#include "SceneManager.h"

#include "Benchmark.h"

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

    glfwInit();

    GLFWwindow* window = glfwCreateWindow(1600, 900, "1.10", nullptr, nullptr);
    glfwSetWindowPos(window, 150, 100);
    glfwMakeContextCurrent(window);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    gl3wInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    ImageManager image_manager;
    UI ui(window, &image_manager);

    PeriodicTimer periodic_timer(200);
    FrameTimer frame_timer;

    CameraSettings camera_settings;
    Camera camera(window, camera_settings);

    ProgramManager program_manager;
    program_manager.attach_camera(&camera);
    SceneManager scene_manager(&program_manager);

    Client client;
    client.n_connect();

    EntityManager entity_manager(&client, &scene_manager);
    MessageHandler message_handler(&client, &entity_manager);

    std::thread t = std::thread(&Client::n_receieve, &client);
    t.detach();
    std::thread t2 = std::thread(&Client::tick_update, &client);
    t2.detach();

    Scene map;
    map.load_assimp("Data/Models/Map/", "map.obj");
    map.attach_program(program_manager.get(1));

    Entity* akali_entity = entity_manager.create(0, client.get_id());

    camera.set_mode(Camera::LOCKED);

    Grid grid(128, 128, program_manager.get(0));

    UnitSelection unit_selection(client.get_id(),
        entity_manager.get_component_manager()->get_transform_components(),
        program_manager.get(3),
        ui.get_unit_panel()
    );

    bool edit_mode = false;

    while(!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {

        ui.new_frame();

        camera.update();

        message_handler.process();

        entity_manager.update();

        static bool build_mode = false;
        if (build_mode) {
            auto pos = camera.mouse_position_world();
            //building_transform->_transform.set_position(pos);
            //building.set_transform(building_transform->_transform);
        }
        
        ui.update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, 0, CLEAR_COLOR);

        //draw stuff
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        map.draw();
        glDisable(GL_CULL_FACE);

        if(edit_mode) grid.draw(glm::vec2(128, 128));

        static bool lm_released = true;
        if(lm_released == false) unit_selection.draw();

        entity_manager.draw();

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
                    static PeriodicTimer timer(1000);
                    if (timer.alert()) {
                        auto transform_message = std::make_shared<TransformMessage>(unit_selection.get_indices(), camera.mouse_position_world());
                        client.n_send(transform_message);
                    }
                }

                bool pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
                int w, h;
                glfwGetWindowSize(window, &w, &h);
                if (!lm_released && pressed) {
                    unit_selection.end(glm::vec2(xpos / (double)w, ypos / (double)h), camera.mouse_position_world());
                }
                else if (lm_released && pressed) {
                    unit_selection.start(glm::vec2(xpos / (double)w, ypos / (double)h), camera.mouse_position_world());
                    lm_released = false;
                }
                else if (!lm_released && !pressed) {
                    unit_selection.finalize();
                    lm_released = true;
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
