#include <memory>
#include "pch.h"
#include "StringHelpers.h"
#include "Game.h"
#include "EntityManager.h"
#include "RandomHelpers.h"
#include "Animation.h"
#include "AnimatedSprite.h"
#include "JsonHelpers.h"

const float Game::PlayerSpeed = 100.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);
const auto BlockTexturePath = "../Media/Textures/Block.png";
const auto LadderTexturePath = "../Media/Textures/Echelle.PNG";
const auto MarioSpriteSheetPath = "../Media/Textures/mario_sprite.png";
const auto DonkeySpriteSheetPath = "../Media/Textures/donkey_kong_sprite.png";
const auto StatisticsFontPath = "../Media/Sansation.ttf";
const auto CoinTexturePath = "../Media/Textures/coin.png";
const auto ScoreFontPath = "../Media/BlockyLettersHollow.ttf";
const auto BackgroundPath = "../Media/Textures/background.png";
const auto PeachPath = "../Media/Textures/peach.png";
const std::string BarrelTexturePath = "../Media/Textures/barrels.png";
const auto jumpTime = sf::seconds(0.2f);

Game::Game() :
    mWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Donkey Kong 1981", sf::Style::Close),
    mSpriteSheet(),
    mFont(),
    scoreFont(),
    scoreAnnouncementText(),
    scoreText(),
    mStatisticsText(),
    mStatisticsUpdateTime(),
    mStatisticsNumFrames(0),
    score(0),
    debug(true)
{
    mWindow.setFramerateLimit(160);
    mario = std::make_shared<Mario>();
    donkey = std::make_shared<Donkey>();
    drawBackground();
    drawBlocks();
    drawLadders();
    drawCoins();
    drawPeach();
    drawMario();
    drawDonkey();
    drawStatistics();
    drawScore();
    drawBarrel();
}

void Game::drawBackground() {
    backgroundTexture.loadFromFile(BackgroundPath);
    background.setTexture(backgroundTexture);
}

void Game::drawBlocks() {
    _TextureBlock.loadFromFile(BlockTexturePath);
    _sizeBlock = _TextureBlock.getSize();

    int up_base = 0;

    for (int i = 0; i < BASE_BLOCK_COUNT; i++) {
        _Block[i][BLOCK_COUNT_Y].setTexture(_TextureBlock);

        if (i < (BASE_BLOCK_COUNT / 2)) {
            _Block[i][BLOCK_COUNT_Y].setPosition(-70.f + 70.f * (i + 1), 0.f + BLOCK_SPACE * (BLOCK_COUNT_Y + 1.7f));
        } else {
            up_base += 1;
            _Block[i][BLOCK_COUNT_Y].setPosition(-70.f + 70.f * (i + 1), 0.f + BLOCK_SPACE * (BLOCK_COUNT_Y + 1.7f) - up_base);
        }

        std::shared_ptr<Entity> se = std::make_shared<Entity>(false, EntityType::block);
        se->m_sprite = _Block[i][BLOCK_COUNT_Y];
        se->m_size = _TextureBlock.getSize();
        se->m_position =_Block[i][BLOCK_COUNT_Y].getPosition();
        EntityManager::m_Entities.push_back(se);
    }

    for (int i = 0; i < BLOCK_COUNT_X; i++) {
        for (int j = 0; j < BLOCK_COUNT_Y; j++) {
            _Block[i][j].setTexture(_TextureBlock);

            if (j % 2) {
                _Block[i][j].setPosition(130.f + 70.f * (i + 1),-5.f + BLOCK_SPACE * (j + 1.7f) + (i + 1));
            } else {
                _Block[i][j].setPosition(190.f + 70.f * (i + 1), 5.f + BLOCK_SPACE * (j + 1.7f) - (i + 1));
            }

            std::shared_ptr<Entity> se = std::make_shared<Entity>(false, EntityType::block);
            se->m_sprite = _Block[i][j];
            se->m_size = _TextureBlock.getSize();
            se->m_position = _Block[i][j].getPosition();
            EntityManager::m_Entities.push_back(se);
        }
    }
}

void Game::drawPeach() {
    for (int i = 0; i < PEACH_PLATFORM_WIDTH; i++) {
        peachPlatform[i].setTexture(_TextureBlock);
        peachPlatform[i].setPosition(550 + (i * _TextureBlock.getSize().x), 85);

        std::shared_ptr<Entity> se = std::make_shared<Entity>(false, EntityType::block);
        se->m_sprite = peachPlatform[i];
        se->m_size = _TextureBlock.getSize();
        se->m_position = peachPlatform[i].getPosition();
        EntityManager::m_Entities.push_back(se);
    }

    peachLadder.setTexture(_LadderTexture);
    peachLadder.setPosition(800, _sizeBlock.y + 50);

    std::shared_ptr<Entity> se = std::make_shared<Entity>(false, EntityType::ladder);
    se->m_sprite = peachLadder;
    se->m_size = _LadderTexture.getSize();
    se->m_position = peachLadder.getPosition();
    EntityManager::m_Entities.push_back(se);

    peachTexture.loadFromFile(PeachPath);
    peachSprite.setTexture(peachTexture);
    peachSprite.setScale(0.45f, 0.45f);
    peachSprite.setPosition(550, 10);

    std::shared_ptr<Entity> peachEntity = std::make_shared<Entity>(false, EntityType::peach);
    se->m_sprite = peachSprite;
    se->m_size = peachTexture.getSize();
    se->m_position = peachSprite.getPosition();
    EntityManager::m_Entities.push_back(peachEntity);
}

void Game::drawLadders() {
    _LadderTexture.loadFromFile(LadderTexturePath);

    for (int i = 0; i < SCALE_COUNT; i++) {
        _Ladder[i].setTexture(_LadderTexture);

        if (i % 2) {
            _Ladder[i].setPosition(830.f + 70.f, -30.f + BLOCK_SPACE * (i + 1.7f) + _sizeBlock.y);
        } else {
            _Ladder[i].setPosition(230.f + 70.f, -30.f + BLOCK_SPACE * (i + 1.7f) + _sizeBlock.y);
        }

        std::shared_ptr<Entity> se = std::make_shared<Entity>(false, EntityType::ladder);
        se->m_sprite = _Ladder[i];
        se->m_size = _LadderTexture.getSize();
        se->m_position = _Ladder[i].getPosition();
        EntityManager::m_Entities.push_back(se);
    }
}

void Game::drawMario() {
    mSpriteSheet.loadFromFile(MarioSpriteSheetPath);

    const auto FRAME_WIDTH = 16;
    const auto FRAME_HEIGHT = 16;
    const auto SCALE_WIDTH = MARIO_WIDTH / FRAME_WIDTH;
    const auto SCALE_HEIGHT = MARIO_HEIGHT / FRAME_HEIGHT;

    // Setup Mario
    mario->m_size = sf::Vector2u(MARIO_WIDTH, MARIO_HEIGHT);
    mario->m_position = sf::Vector2f(100.f + 30.f, BLOCK_SPACE * 7.7f - MARIO_HEIGHT);

    // Static sprites
    const auto standingRightSprite = sf::IntRect(162, 0, FRAME_WIDTH, FRAME_HEIGHT);
    const auto standingLeftSprite = sf::IntRect(122, 0, FRAME_WIDTH, FRAME_HEIGHT);
    mario->standingRightRect = sf::Sprite(mSpriteSheet, standingRightSprite);
    mario->standingLeftRect = sf::Sprite(mSpriteSheet, standingLeftSprite);
    mario->m_sprite = mario->standingRightRect;

    mario->standingLeftRect.scale(SCALE_WIDTH, SCALE_HEIGHT);
    mario->standingRightRect.scale(SCALE_WIDTH, SCALE_HEIGHT);

    // Animations
    mario->walkingAnimationRight.setSpriteSheet(mSpriteSheet);
    mario->walkingAnimationRight.addFrame(standingRightSprite);
    mario->walkingAnimationRight.addFrame(sf::IntRect(200, 0, FRAME_WIDTH, FRAME_HEIGHT));
    mario->walkingAnimationRight.addFrame(sf::IntRect(240, 0, FRAME_WIDTH, FRAME_HEIGHT));

    mario->walkingAnimationLeft.setSpriteSheet(mSpriteSheet);
    mario->walkingAnimationLeft.addFrame(standingLeftSprite);
    mario->walkingAnimationLeft.addFrame(sf::IntRect(80, 0, FRAME_WIDTH, FRAME_HEIGHT));
    mario->walkingAnimationLeft.addFrame(sf::IntRect(40, 0, FRAME_WIDTH, FRAME_HEIGHT));

    mario->currentAnimation = &mario->walkingAnimationRight;

    // set up AnimatedSprite
    auto animatedSprite = AnimatedSprite(sf::seconds(0.1f), true, false);
    animatedSprite.scale(SCALE_WIDTH, SCALE_HEIGHT);
    mario->animatedSprite = animatedSprite;

    // Add Mario to entities
    EntityManager::m_Entities.push_back(mario);
}

void Game::drawDonkey() {
    donkey->spriteSheet.loadFromFile(DonkeySpriteSheetPath);

    // Setup Donkey
    donkey->m_size = sf::Vector2u(60, 60);
    donkey->m_position = sf::Vector2f(900, 90);

    // Animations
    donkey->chest.setSpriteSheet(donkey->spriteSheet);

    for (auto const& rect : JsonHelpers::OpenSpriteSheetDescriptor("../Media/donkey_kong_sprite_sheet_descriptor.json")) {
        donkey->chest.addFrame(rect);
    }

    donkey->currentAnimation = &donkey->chest;

    // set up AnimatedSprite
    auto animatedSprite = AnimatedSprite(sf::seconds(0.055f), true, false);
    animatedSprite.scale(2, 2);
    donkey->animatedSprite = animatedSprite;
    donkey->isMoving = true;

    // Add Mario to entities
    EntityManager::m_Entities.push_back(donkey);
}

void Game::drawStatistics() {
    mFont.loadFromFile(StatisticsFontPath);

    mStatisticsText.setString("Welcome to Donkey Kong 1981");
    mStatisticsText.setFont(mFont);
    mStatisticsText.setPosition(5.f, 5.f);
    mStatisticsText.setCharacterSize(10);
    mStatisticsText.setFillColor(sf::Color::Black);
}

void Game::drawScore() {
    scoreFont.loadFromFile(ScoreFontPath);

    scoreAnnouncementText.setString("Score");
    scoreAnnouncementText.setFont(scoreFont);
    scoreAnnouncementText.setPosition(1080.f, 5.f);
    scoreAnnouncementText.setCharacterSize(40);
    scoreAnnouncementText.setFillColor(sf::Color::Black);

    scoreText.setString(std::to_string(score));
    scoreText.setFont(scoreFont);
    scoreText.setPosition(1080.f, 50.f);
    scoreText.setCharacterSize(26);
    scoreText.setFillColor(sf::Color::Black);
}

void Game::drawCoins() {
    _CoinTexture.loadFromFile(CoinTexturePath);
    auto ladders = EntityManager::GetLadders();

    for (auto& coin : _Coin) {
        coin.setTexture(_CoinTexture);

        while (true) {
            // Get random block, then put a coin upon it
            const auto blockX = getRandomNumber(0, BLOCK_COUNT_X);
            const auto blockY = getRandomNumber(0, BLOCK_COUNT_Y);
            const auto randomBlock = _Block[blockX][blockY];
            const auto blockBound = randomBlock.getGlobalBounds();

            coin.setPosition(
                    blockBound.left + (blockBound.width / 2),
                    blockBound.top - _CoinTexture.getSize().y - 5
            );

            const auto coinBounds = coin.getGlobalBounds();
            auto collision = false;

            for (auto const& ladder : ladders) {
                if (ladder->m_sprite.getGlobalBounds().intersects(coinBounds)) {
                    collision = true;
                    break;
                }
            }

            if (!collision) {
                break;
            }
        }

        auto se = std::make_shared<Entity>(false, EntityType::coin);
        se->m_sprite = coin;
        se->m_size = _CoinTexture.getSize();
        se->m_position = coin.getPosition();
        EntityManager::m_Entities.push_back(se);
    }
}

void Game::drawBarrel() {
    _BarrelTexture.loadFromFile(BarrelTexturePath);

    _Barrel.setTexture(_BarrelTexture);
    _Barrel.setPosition(950,100);

    auto se = std::make_shared<Barrel>();

    se->falling.setSpriteSheet(_BarrelTexture);
    se->falling.addFrame(sf::IntRect(4, 24, 16, 11));
    se->falling.addFrame(sf::IntRect(25, 24, 16, 11));

    se->rolling.setSpriteSheet(_BarrelTexture);
    se->rolling.addFrame(sf::IntRect(4, 40, 12, 10));
    se->rolling.addFrame(sf::IntRect(21, 40, 12, 10));
    se->rolling.addFrame(sf::IntRect(38, 40, 12, 10));
    se->rolling.addFrame(sf::IntRect(55, 40, 12, 10));
    //se->currentAnimation = &se->rolling;

    // set up AnimatedSprite
    auto animatedSprite = AnimatedSprite(sf::seconds(0.1f), true, false);
    animatedSprite.scale(2, 2);
    se->animatedSprite = animatedSprite;

    se->m_size = sf::Vector2u(32, 20);
    se->m_position = _Barrel.getPosition();

    barrels.push_back(se);
}

void Game::run() {
    sf::Clock fpsClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (!lost) {
        sf::Time elapsedTime = fpsClock.restart();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            handleCollisions();
            update(TimePerFrame);
        }
        updateScore();
        updateStatistics(elapsedTime);
        render();
    }
}

void Game::processEvents(){
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

void Game::update(sf::Time elapsedTime) {
    updateMario(elapsedTime);
    updateBarrels(elapsedTime);
}

void Game::updateBarrels(sf::Time elapsedTime) {
    framesSinceLastBarrel++;
    if (framesSinceLastBarrel % BARREL_SPAWN_RATE == 0) {
        drawBarrel();
        framesSinceLastBarrel = 0;
    }

    for (auto const &barrel: barrels) {
        if (barrel->m_position.x > SCREEN_WIDTH + 100 || barrel->m_position.x < -100 ||
            barrel->m_position.y > SCREEN_HEIGHT + 100 || barrel->m_position.y < -100) {
            barrels.remove(barrel);

        } else {
            sf::Vector2f movement(0.f, 0.f);
            if (barrel->isFalling) {
                barrel->fallingTime+=elapsedTime;
                barrel->currentAnimation = &barrel->falling;
                movement.y += MARIO_GRAVITY;
            } else {
                barrel->currentAnimation = &barrel->rolling;
                movement.x -= PlayerSpeed * barrel->direction;
            }


            auto nextXPosition = barrel->m_position.x + (movement.x * elapsedTime.asSeconds());

            if (nextXPosition <= 0 || nextXPosition >= SCREEN_WIDTH) {
                movement.x = 0;
            }

            barrel->isMoving = fabs(movement.x) > 0;
            barrel->m_position.x += movement.x * elapsedTime.asSeconds();
            barrel->m_position.y += movement.y * elapsedTime.asSeconds();

            barrel->animatedSprite.play(*barrel->currentAnimation);
            barrel->animatedSprite.setPosition(barrel->m_position);
            barrel->animatedSprite.update(elapsedTime);

            barrel->m_sprite.setPosition(barrel->m_position);
        }
    }
}

void Game::updateMario(sf::Time elapsedTime){
    sf::Vector2f movement(0.f, 0.f);
    mario->onTheFloor = mario->isOnTheFloor();
    mario->onBottomOfLadder = mario->isOnBottomOfLadder();
    mario->onTopOfLadder = mario->isOnTopOfLadder();

    if (mario->isJumping && mario->lastJump + jumpTime > clock.getElapsedTime()) {
        movement.y -= MARIO_GRAVITY;
    } else {
        mario->isJumping = false;
    }

    if (mario->isFalling && !mario->isClimbing && !mario->isJumping){
        movement.y += MARIO_GRAVITY;
    }

    if (mario->isMovingLeft) {
        movement.x -= PlayerSpeed;
        mario->currentAnimation = &mario->walkingAnimationLeft;
        mario->isFacingLeft = true;
        mario->isFacingRight = false;
    }

    if (mario->isMovingRight) {
        movement.x += PlayerSpeed;
        mario->currentAnimation = &mario->walkingAnimationRight;
        mario->isFacingRight = true;
        mario->isFacingLeft = false;
    }

    if (mario->onTopOfLadder) {
        mario->isClimbing = mario->isMovingDown;
    } else if (mario->onBottomOfLadder) {
        mario->isClimbing = mario->isMovingUp;
    }

    if (mario->isOnLadder && mario->isClimbing) {
        if (mario->isMovingUp) {
            if (mario->onTopOfLadder) {
                mario->isClimbing = false;
            } else {
                movement.y -= PlayerSpeed;
                movement.x = 0;
            }
        } else if (mario->isMovingDown) {
            if (mario->onBottomOfLadder) {
                movement.y = 0;
                mario->isClimbing = false;
            } else {
                movement.y += PlayerSpeed;
                movement.x = 0;
            }
        } else {
            movement.x = 0;
        }
    }

    auto nextXPosition = mario->m_position.x + (movement.x * elapsedTime.asSeconds());

    if (nextXPosition <= 0 || nextXPosition >= SCREEN_WIDTH) {
        movement.x = 0;
    }

    mario->isMoving = fabs(movement.x) > 0;
    mario->m_position.x += movement.x * elapsedTime.asSeconds();
    mario->m_position.y += movement.y * elapsedTime.asSeconds();

    mario->animatedSprite.play(*mario->currentAnimation);
    mario->animatedSprite.setPosition(mario->m_position);
    mario->animatedSprite.update(elapsedTime);

    donkey->animatedSprite.play(*donkey->currentAnimation);
    donkey->animatedSprite.setPosition(donkey->m_position);
    donkey->animatedSprite.update(elapsedTime);

    if (!mario->isMoving) {
        mario->animatedSprite.stop();

        if (mario->isFacingLeft) {
            mario->m_sprite = mario->standingLeftRect;
        } else {
            mario->m_sprite = mario->standingRightRect;
        }

        mario->m_sprite.setPosition(mario->m_position);
    }
}

void Game::render() {
    mWindow.clear();
    mWindow.draw(background);

    for (const std::shared_ptr<Entity> &entity : EntityManager::m_Entities) {
        if (entity->isAnimated && entity->isMoving) {
            mWindow.draw(entity->animatedSprite);
        } else {
            mWindow.draw(entity->m_sprite);
        }
    }

    for (const std::shared_ptr<Barrel> &barrel : barrels) {
        if (barrel->isAnimated && barrel->isMoving) {
            mWindow.draw(barrel->animatedSprite);
        } else {
            mWindow.draw(barrel->m_sprite);
        }
    }

    if (debug) {
        sf::RectangleShape marioBox;
        marioBox.setPosition(mario->m_position);
        marioBox.setSize(sf::Vector2f(mario->m_size.x, mario->m_size.y));
        marioBox.setOutlineColor(sf::Color::Red);
        marioBox.setFillColor(sf::Color::Transparent);
        marioBox.setOutlineThickness(2.f);
        mWindow.draw(marioBox);
    }

    mWindow.draw(scoreAnnouncementText);
    mWindow.draw(scoreText);
    mWindow.draw(mStatisticsText);
    mWindow.display();
}

void Game::updateScore() {
    scoreText.setString(std::to_string(score));
}

void Game::updateStatistics(sf::Time elapsedTime) {
    mStatisticsUpdateTime += elapsedTime;
    mStatisticsNumFrames += 1;

    if (mStatisticsUpdateTime >= sf::seconds(1.0f)) {
        mStatisticsText.setString(
                "Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
                "Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    switch (key) {
        case sf::Keyboard::Up:
            mario->isMovingUp = isPressed;
            break;
        case sf::Keyboard::Down:
            mario->isMovingDown = isPressed;
            break;
        case sf::Keyboard::Left:
            mario->isMovingLeft = isPressed;
            break;
        case sf::Keyboard::Right:
            mario->isMovingRight = isPressed;
            break;

        case sf::Keyboard::Space:
            if (mario->isFalling) {
                break;
            }

            mario->lastJump = clock.getElapsedTime();
            mario->isJumping = isPressed;
            break;
    }
}

void Game::handleCollisions() {
    handleLaddersCollisions();
    handleElevationCollisions();
    handleFloorsCollisions();
    handleCoinsCollisions();
    handleBarrelsCollisions();
}

void Game::handleBarrelsCollisions() {
    handleBarrelsPlayerCollisions();
    handleBarrelsFloorCollisions();
}

void Game::handleBarrelsPlayerCollisions() {
    const auto playerBounds  = mario->getBounds();

    for (auto const& barrel: barrels) {
        const auto coinGlobalBounds = barrel->getBounds();

        if (coinGlobalBounds.intersects(playerBounds)) {
            EntityManager::Remove(barrel);
            lost = true;
        }
    }
}

void Game::handleCoinsCollisions() {
    const auto coins = EntityManager::GetCoins();
    const auto playerBounds  = mario->getBounds();

    for (auto const& coin: coins) {
        const auto coinGlobalBounds = coin.get()->m_sprite.getGlobalBounds();

        if (coinGlobalBounds.intersects(playerBounds)) {
            EntityManager::Remove(coin);
            score += COIN_VALUE;
        }
    }
}

void Game::handleFloorsCollisions() {
    const auto floors = EntityManager::GetFloors();
    auto playerBounds = mario->getBounds();

    playerBounds.height += 1;
    mario->isFalling = true;

    for (auto const& floor: floors) {
        const auto floorGlobalBounds = floor.get()->m_sprite.getGlobalBounds();

        if (floorGlobalBounds.intersects(playerBounds)) {
            const auto isPlayerBeneathFloor = floorGlobalBounds.top - playerBounds.top < 0;
            mario->isFalling = isPlayerBeneathFloor;
            return;
        }
    }
}

void Game::handleBarrelsFloorCollisions() {
    const auto floors = EntityManager::GetFloors();

    for (auto const& barrel: barrels) {
        auto barrelBounds = barrel->getBounds();
        barrelBounds.height +=1;
        bool wasFalling = barrel->isFalling;
        barrel->isFalling = true;

        for (auto const& floor: floors) {
            const auto floorGlobalBounds = floor.get()->m_sprite.getGlobalBounds();
            if (floorGlobalBounds.intersects(barrelBounds)) {
                const auto isBarrelBeneathFloor = floorGlobalBounds.top - barrelBounds.top < 0;

                if(wasFalling && barrel->fallingTime > sf::seconds(0.5f)) {
                    barrel->direction *= -1;
                }
                barrel->fallingTime = sf::Time::Zero;
                barrel->isFalling = isBarrelBeneathFloor;
                break;
            }
        }

    }

}

void Game::handleLaddersCollisions() {
    const auto ladders = EntityManager::GetLadders();
    auto playerBounds = mario->getBounds();

    mario->isOnLadder = false;

    for (auto const& ladder: ladders) {
        auto ladderGlobalBounds = ladder.get()->m_sprite.getGlobalBounds();
        ladderGlobalBounds.top -= 10;
        ladderGlobalBounds.height += 10;

        if (ladderGlobalBounds.intersects(playerBounds)) {
            mario->isOnLadder = true;
            return;
        }
    }
}

void Game::handleElevationCollisions() {
    if (mario->isOnLadder) {
        return;
    }

    if (mario->isOnTheFloor()) {
        const auto floors = EntityManager::GetFloors();
        const auto playerBounds = mario->getBounds();

        for (auto const& floor : floors) {
            const auto floorGlobalBounds = floor.get()->m_sprite.getGlobalBounds();

            if (floorGlobalBounds.intersects(playerBounds)) {
                mario->m_position.y -= mario->m_position.y - floorGlobalBounds.top + MARIO_HEIGHT;
                break;
            }
        }
    }
}