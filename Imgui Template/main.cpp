#include "imgui.h"
#include "imgui-SFML.h"
#include "zlib.h"

#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1777, 1000), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::View centreView;
    sf::Vector2u size = window.getSize();
    centreView.setSize(sf::Vector2f(size.x, size.y));
    centreView.setCenter(0, 0);
    const float minZoom = 0.5;
    const float maxZoom = 256.f;
    float prevZoom = 1.f;
    float zoom = 1.f;
    sf::Vector2f prevCamPos = { 0.f,0.f };
    sf::Vector2f camPos = { 0.f,0.f };
    sf::Vector2f mousePos = { 0.f,0.f };
    uint8_t KEYW = 1;
    uint8_t KEYS = 2;
    uint8_t KEYA = 4;
    uint8_t KEYD = 8;
    uint8_t pressed = 0;

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::MouseMoved)
            {
                mousePos = sf::Vector2f(event.mouseMove.x - float(size.x) / 2, event.mouseMove.y - float(size.y) / 2);
            }
            else if (event.type == sf::Event::Resized)
            {
                size = window.getSize();
                centreView.setSize(sf::Vector2f(size.x, size.y));
                centreView.setCenter(0, 0);
                prevZoom = 1.f;
                zoom = 1.f;
            }
            else if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.delta != 0)
                {
                    zoom -= event.mouseWheelScroll.delta * 0.5f;
                    if (zoom < minZoom)
                        zoom = minZoom;
                    if (zoom > maxZoom)
                        zoom = maxZoom;
                }
            }
            else if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::Resized)
            {
                size = window.getSize();
                centreView.setSize(sf::Vector2f(size.x, size.y));
                centreView.setCenter(0, 0);
            }
        }
        const float move_speed = 10.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            camPos.y -= move_speed * zoom;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            camPos.y += move_speed * zoom;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            camPos.x -= move_speed * zoom;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            camPos.x += move_speed * zoom;
        }
        if (prevZoom != zoom)
        {
            if (zoom < prevZoom)
                camPos += (mousePos * prevZoom * 0.5f * (prevZoom / zoom));
            centreView.zoom(pow(zoom / prevZoom, 2));
            prevZoom = zoom;
        }
        if (prevCamPos != camPos)
        {
            centreView.move(camPos - prevCamPos);
            prevCamPos = camPos;
        }
        window.setView(centreView);
        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::ShowDemoWindow();

        ImGui::Begin("Hello, world!");
        ImGui::SetWindowFontScale(1.5f);
        if (ImGui::Button("Look at this pretty button"))
            std::cout << "Hey its rudimentary but it works \n";
        ImGui::End();

        window.clear();
        window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    
    return 0;
}