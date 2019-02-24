#ifndef DONKEYKONG_STARTSCREEN_H
#define DONKEYKONG_STARTSCREEN_H
#define START_SCREEN_HEIGHT 720
#define START_SCREEN_WIDTH 1280


class StartScreen {

    public:
        StartScreen();

        ~StartScreen() = default;;

    void run();

    private:
        void drawDonkey();
        void drawBackground();
        void processEvents();
        void update(sf::Time elapsedTime);
        void render();
        void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

    private:
        static const sf::Time TimePerFrame;
        sf::Clock clock;
        sf::RenderWindow mWindow;
        sf::Texture mSpriteSheet;
        sf::Font mFont;
        bool debug;
        std::shared_ptr<Donkey> donkey;
        sf::Texture backgroundTexture;
        sf::Sprite background;

};


#endif //DONKEYKONG_STARTSCREEN_H
