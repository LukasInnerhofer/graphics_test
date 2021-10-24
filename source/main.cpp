#include <chrono>
#include <clocale>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <system_error>
#include <thread>
#include <utility>

#include "libgraphics/rectangle.h"
#include "libgraphics/window.h"

int main()
{
    std::setlocale(LC_ALL, "en_US.utf8");

    std::unique_ptr<LibGraphics::Window> window;
    std::u32string const title{U"z\u00DF\u6C34\U0001F34C"};
    try
    {
        window = std::make_unique<LibGraphics::Window>(title);
    }
    catch (std::exception const &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    std::vector<uint8_t> data(100 * 100 * 3);
    uint8_t temp = 0x80;
    for (size_t i = 0; i < data.size(); ++i)
    {
        data[i] = temp++;
    }
    LibGraphics::Rectangle rectangle{
        {0.5f, 0.5f},
        {0x80, 0x00, 0x00},
        std::make_shared<LibGraphics::Texture>(
            std::move(data),
            LibGraphics::Texture::SizeVector{100, 100}
        )
    };

    LibGraphics::Rectangle rectangle1{
        {0.25f, 0.25f},
        {0x00, 0x00, 0x80}
    };

    auto texture{std::make_shared<LibGraphics::Texture>(
        std::vector<uint8_t>{
            0x80, 0x00, 0x00,
            0x00, 0x80, 0x00,
            0x00, 0x00, 0x80,
            0x80, 0x80, 0x80
        },
        LibGraphics::Texture::SizeVector{2, 2}
    )};
    LibGraphics::Rectangle rectangle2{
        {0.1f, 0.1f},
        {0, 0, 0},
        texture
    };

    LibGraphics::Window::Event event;
    std::thread timerThread;
    LibGraphics::Color color{LibGraphics::Color::cornflowerBlue};
    bool colorCountUp = true;
    uint64_t time = 0;
    while(window)
    {
        timerThread = std::thread{[]() { std::this_thread::sleep_for(std::chrono::microseconds(6944)); }};

        window->clear(color);

        if(color.getG() == LibGraphics::Color::max)
        {
            colorCountUp = false;
        }
        if(color.getG() == 0)
        {
            colorCountUp = true;
        }

        color.setG(color.getG() + colorCountUp - !colorCountUp);
        
        texture->setPixel({0,0}, {static_cast<uint8_t>(time), static_cast<uint8_t>(time), static_cast<uint8_t>(time)});
        rectangle.setPosition({std::sin(time / 50.0f), std::cos(time / 100.0f)});
        rectangle1.setPosition({std::cos(time / 20.0f), std::sin(time / 10.0f)});
        window->draw(rectangle);
        window->draw(rectangle1);
        window->draw(rectangle2);

        window->display();
        timerThread.join();
        ++time;

        if (window->pollEvent(event))
        {
            switch (event.type)
            {
            case LibGraphics::Window::EventType::Closed:
                window = nullptr;
                break;
            }
        }
    }

    return 0;
}
