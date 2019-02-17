#include "Mario.h"

sf::Rect<float> Mario::getBounds() const {
    return {
        m_position.x,
        m_position.y,
        (float) m_size.x ,
        (float) m_size.y
    };
}