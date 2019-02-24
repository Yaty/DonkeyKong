#pragma once

#include <list>
#include "Entity.h"
#include "pch.h"

class JsonHelpers {
    public:
        static std::list<sf::IntRect> OpenSpriteSheetDescriptor(std::string fileName);
};

