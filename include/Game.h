#pragma once

#define SCALE_COUNT 6
#define BLOCK_COUNT_X 12
#define BASE_BLOCK_COUNT 16
#define BLOCK_COUNT_Y 6
#define BLOCK_SPACE 105.f
#define COIN_COUNT 5
#define COIN_VALUE 100

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
        void drawBlocks();
        void drawLadders();
        void drawMario();
        void drawStatistics();
        void drawCoins();
        void handleCoins();
        void drawScore();

    private:
        static const float PlayerSpeed;
        static const sf::Time TimePerFrame;

        sf::RenderWindow mWindow;
        sf::Texture mTexture;
        sf::Sprite mPlayer;
        sf::Font mFont;
        sf::Font scoreFont;
        sf::Text mStatisticsText;
        sf::Time mStatisticsUpdateTime;
        sf::Text scoreAnnouncementText;
        sf::Text scoreText;

        std::size_t mStatisticsNumFrames;
        bool mIsMovingUp;
        bool mIsMovingDown;
        bool mIsMovingRight;
        bool mIsMovingLeft;

        sf::Texture _CoinTexture;
        sf::Sprite _Coin[COIN_COUNT];
        sf::Texture _LadderTexture;
        sf::Sprite _Ladder[SCALE_COUNT];
        sf::Texture _TextureBlock;
        sf::Sprite _Block[BASE_BLOCK_COUNT][BLOCK_COUNT_Y];
        sf::Texture _TextureWeapon;
        sf::Sprite _Weapon;
        sf::Vector2u _sizeBlock;
        sf::Vector2u _sizeMario;

        int score;
};
