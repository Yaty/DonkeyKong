#pragma once

#include "Animation.h"
#include "AnimatedSprite.h"
#include "Mario.h"
#include "Donkey.h"

#define SCALE_COUNT 6
#define BLOCK_COUNT_X 11
#define BASE_BLOCK_COUNT 20
#define BLOCK_COUNT_Y 6
#define BLOCK_SPACE 130.f
#define COIN_COUNT 9
#define COIN_VALUE 100
#define SCREEN_HEIGHT 960
#define SCREEN_WIDTH 1280
#define MARIO_HEIGHT 48
#define MARIO_WIDTH 32
#define MARIO_GRAVITY 200.0f

class Game {
    public:
        Game();

        ~Game() = default;;

        void run();

    private:
        void processEvents();
        void update(sf::Time elapsedTime);
        void render();
        void updateStatistics(sf::Time elapsedTime);
        void updateScore();
        void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
        void drawBackground();
        void drawBlocks();
        void drawLadders();
        void drawMario();
        void drawDonkey();
        void drawStatistics();
        void drawCoins();
        void handleCoinsCollisions();
        void drawScore();
        void handleFloorsCollisions();
        void handleLaddersCollisions();
        void handleElevationCollisions();
        void handleCollisions();

    private:
        static const float PlayerSpeed;
        static const sf::Time TimePerFrame;
        sf::Clock clock;

        sf::RenderWindow mWindow;
        sf::Texture mSpriteSheet;
        sf::Font mFont;
        sf::Font scoreFont;
        sf::Text mStatisticsText;
        sf::Time mStatisticsUpdateTime;
        sf::Text scoreAnnouncementText;
        sf::Text scoreText;

        std::size_t mStatisticsNumFrames;
        bool debug;

        sf::Texture _CoinTexture;
        sf::Sprite _Coin[COIN_COUNT];
        sf::Texture _LadderTexture;
        sf::Sprite _Ladder[SCALE_COUNT];
        sf::Texture _TextureBlock;
        sf::Sprite _Block[BASE_BLOCK_COUNT][BLOCK_COUNT_Y];
        sf::Texture _TextureWeapon;
        sf::Sprite _Weapon;
        sf::Vector2u _sizeBlock;

        int score;
        std::shared_ptr<Mario> mario;
        std::shared_ptr<Donkey> donkey;

        sf::Texture backgroundTexture;
        sf::Sprite background;
};
