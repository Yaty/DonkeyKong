#include "JsonHelpers.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

std::list<sf::IntRect> JsonHelpers::OpenSpriteSheetDescriptor(std::string fileName) {
    std::ifstream file(fileName, std::ifstream::binary);
    json j = json::parse(file);

    auto rectanglesArray = j["rectangles"];
    std::list<sf::IntRect> rectangles;

    for (auto const& rectangle : rectanglesArray) {
        rectangles.emplace_back(sf::IntRect(rectangle[0], rectangle[1], rectangle[2], rectangle[3]));
    }

    return rectangles;
}