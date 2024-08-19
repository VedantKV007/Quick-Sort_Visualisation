#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <algorithm>

using namespace sf;

struct Rectangle {
    int height = 0;
    int width = 1;
    int xPos = 0;
    int yPos = 0;
    Color color = Color::Black;
};

void fillVector(std::vector<Rectangle>& rectHolder, const Vector2u& imgSize) {
    int xOffset = 0;
    while (xOffset < static_cast<int>(imgSize.x)) {
        Rectangle rect;
        rect.height = std::max(3, rand() % static_cast<int>(imgSize.y));
        rect.xPos = xOffset;
        rect.yPos = imgSize.y - rect.height;
        rectHolder.push_back(rect);
        xOffset += 3;
    }
    std::cout << "Rectangles created: " << rectHolder.size() << std::endl;
}

void paintVector(RenderTexture& renderTexture, const std::vector<Rectangle>& rectHolder) {
    renderTexture.clear(Color::White);
    for (const auto& rect : rectHolder) {
        RectangleShape shape(Vector2f(rect.width, rect.height));
        shape.setPosition(rect.xPos, rect.yPos);
        shape.setFillColor(rect.color);
        renderTexture.draw(shape);
    }
    renderTexture.display();
    std::cout << "Painting completed" << std::endl;
}

void bubbleSort(std::vector<Rectangle>& rectHolder, RenderTexture& renderTexture, RenderWindow& window) {
    for (size_t i = 0; i < rectHolder.size(); ++i) {
        for (size_t j = 0; j < rectHolder.size() - 1; ++j) {
            if (rectHolder[j].height > rectHolder[j + 1].height) {
                std::swap(rectHolder[j], rectHolder[j + 1]);
                std::swap(rectHolder[j].xPos, rectHolder[j + 1].xPos);

                paintVector(renderTexture, rectHolder);
                window.clear();
                window.draw(Sprite(renderTexture.getTexture()));
                window.display();
            }
        }
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    const int width = 1000;
    const int height = 800;
    RenderWindow window(VideoMode(width, height), "BubbleSort");
    RenderTexture renderTexture;
    if (!renderTexture.create(width, height)) {
        std::cerr << "Failed to create render texture" << std::endl;
        return 1;
    }

    std::vector<Rectangle> rectVector;
    fillVector(rectVector, renderTexture.getSize());
    paintVector(renderTexture, rectVector);

    bool isSorting = false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::Space) {
                std::cout << "SpaceBar Pressed" << std::endl;
                isSorting = true;
            }
        }

        if (isSorting) {
            bubbleSort(rectVector, renderTexture, window);
            isSorting = false;
        }

        window.clear();
        window.draw(Sprite(renderTexture.getTexture()));
        window.display();
    }

    return 0;
}