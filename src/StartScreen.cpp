
#include <memory>
#include "pch.h"
#include "StringHelpers.h"
#include "Game.h"
#include "EntityManager.h"
#include "RandomHelpers.h"
#include "Animation.h"
#include "AnimatedSprite.h"
#include "JsonHelpers.h"

#include "StartScreen.h"

const sf::Time StartScreen::TimePerFrame = sf::seconds(1.f / 60.f);
const auto DonkeySpriteSheetPath = "../Media/Textures/donkey_kong_sprite.png";
const auto BackgroundPath = "../Media/Textures/start-screen.png";


StartScreen::StartScreen():
        mWindow(sf::VideoMode(START_SCREEN_WIDTH, START_SCREEN_HEIGHT), "Donkey Kong 1981", sf::Style::Close),
        mSpriteSheet(),
        mFont(),
        debug(true)
{
    mWindow.setFramerateLimit(160);
    donkey = std::make_unique<Donkey>();
    drawBackground();
    drawDonkey();
}

void StartScreen::drawBackground() {
    backgroundTexture.loadFromFile(BackgroundPath);
    background.setTexture(backgroundTexture);
}

void StartScreen::drawDonkey() {
    donkey->spriteSheet.loadFromFile(DonkeySpriteSheetPath);

    // Setup Donkey
    donkey->m_size = sf::Vector2u(60, 60);
    donkey->m_position = sf::Vector2f(585, 500);

    // Animations
    donkey->chest.setSpriteSheet(donkey->spriteSheet);

    for (auto const& rect : JsonHelpers::OpenSpriteSheetDescriptor("../Media/donkey_kong_sprite_sheet_descriptor.json")) {
        donkey->chest. addFrame(rect);
    }

    donkey->currentAnimation = &donkey->chest;

    auto animatedSprite = AnimatedSprite(sf::seconds(0.055f), true, false);
    animatedSprite.scale(2, 2);
    donkey->animatedSprite = animatedSprite;
    donkey->isMoving = true;

    EntityManager::m_Entities.push_back(donkey);
}

void StartScreen::run() {
    sf::Clock fpsClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen()) {
        sf::Time elapsedTime = fpsClock.restart();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            update(TimePerFrame);
        }

        render();
    }
}

void StartScreen::update(sf::Time elapsedTime){
    donkey->animatedSprite.play(*donkey->currentAnimation);
    donkey->animatedSprite.setPosition(donkey->m_position);
    donkey->animatedSprite.update(elapsedTime);
}

void StartScreen::render() {
    mWindow.clear();
    mWindow.draw(background);

    for (const std::shared_ptr<Entity> &entity : EntityManager::m_Entities) {
        if (entity->isAnimated && entity->isMoving) {
            mWindow.draw(entity->animatedSprite);
        } else {
            mWindow.draw(entity->m_sprite);
        }
    }
    mWindow.display();
}

void StartScreen::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    switch (key) {

        case sf::Keyboard::Space:
            EntityManager::m_Entities.clear();
            mWindow.clear();
            mWindow.display();
            mWindow.close();
            Game game;
            game.run();
            break;
    }
}

void StartScreen::processEvents(){
    sf::Event event{};

    while (mWindow.pollEvent(event)){
        switch (event.type){
            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code, true);
                break;

            case sf::Event::KeyReleased:
                handlePlayerInput(event.key.code, false);
                break;

            case sf::Event::Closed:
                mWindow.close();
                break;

            default: ;
        }
    }
}