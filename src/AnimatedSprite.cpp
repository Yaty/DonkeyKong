#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(
    sf::Time frameTime,
    bool paused,
    bool looped
) :
    mAnimation(NULL),
    mFrameTime(frameTime),
    mCurrentFrame(0),
    mIsPaused(paused),
    mIsLooped(looped),
    mTexture(NULL)
{}

void AnimatedSprite::setAnimation(const Animation& animation) {
    mAnimation = &animation;
    mTexture = mAnimation->getSpriteSheet();
    mCurrentFrame = 0;
    setFrame(mCurrentFrame);
}


void AnimatedSprite::play() {
    mIsPaused = false;
}

void AnimatedSprite::play(const Animation& animation) {
    if (getAnimation() != &animation) {
        setAnimation(animation);
    }

    play();
}

void AnimatedSprite::pause() {
    mIsPaused = true;
}

void AnimatedSprite::stop() {
    mIsPaused = true;
    mCurrentFrame = 0;
    setFrame(mCurrentFrame);
}

void AnimatedSprite::setLooped(bool looped) {
    mIsLooped = looped;
}

void AnimatedSprite::setColor(const sf::Color& color) {
    // Update the vertices' color
    mVertices[0].color = color;
    mVertices[1].color = color;
    mVertices[2].color = color;
    mVertices[3].color = color;
}

const Animation* AnimatedSprite::getAnimation() const {
    return mAnimation;
}

sf::FloatRect AnimatedSprite::getLocalBounds() const {
    sf::IntRect rect = mAnimation->getFrame(mCurrentFrame);

    float width = static_cast<float>(std::abs(rect.width));
    float height = static_cast<float>(std::abs(rect.height));

    return sf::FloatRect(0.f, 0.f, width, height);
}

sf::FloatRect AnimatedSprite::getGlobalBounds() const {
    return getTransform().transformRect(getLocalBounds());
}

bool AnimatedSprite::isLooped() const {
    return mIsLooped;
}

bool AnimatedSprite::isPlaying() const {
    return !mIsPaused;
}

sf::Time AnimatedSprite::getFrameTime() const {
    return mFrameTime;
}

void AnimatedSprite::setFrame(std::size_t newFrame, bool resetTime) {
    if (mAnimation) {
        //calculate new vertex positions and texture coordiantes
        sf::IntRect rect = mAnimation->getFrame(newFrame);

        mVertices[0].position = sf::Vector2f(0.f, 0.f);
        mVertices[1].position = sf::Vector2f(0.f, static_cast<float>(rect.height));
        mVertices[2].position = sf::Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
        mVertices[3].position = sf::Vector2f(static_cast<float>(rect.width), 0.f);

        float left = static_cast<float>(rect.left) + 0.0001f;
        float right = left + static_cast<float>(rect.width);
        float top = static_cast<float>(rect.top);
        float bottom = top + static_cast<float>(rect.height);

        mVertices[0].texCoords = sf::Vector2f(left, top);
        mVertices[1].texCoords = sf::Vector2f(left, bottom);
        mVertices[2].texCoords = sf::Vector2f(right, bottom);
        mVertices[3].texCoords = sf::Vector2f(right, top);
    }

    if (resetTime) {
        mCurrentTime = sf::Time::Zero;
    }
}

void AnimatedSprite::update(sf::Time deltaTime) {
    // if not paused and we have a valid animation
    if (!mIsPaused && mAnimation) {
        // add delta time
        mCurrentTime += deltaTime;

        // if current time is bigger then the frame time advance one frame
        if (mCurrentTime >= mFrameTime) {
            // reset time, but keep the remainder
            mCurrentTime = sf::microseconds(mCurrentTime.asMicroseconds() % mFrameTime.asMicroseconds());

            // get next Frame index
            if (mCurrentFrame + 1 < mAnimation->getSize()) {
                mCurrentFrame++;
            } else {
                // animation has ended
                mCurrentFrame = 0; // reset to start

                if (!mIsLooped) {
                    mIsPaused = true;
                }

            }

            // set the current frame, not reseting the time
            setFrame(mCurrentFrame, false);
        }
    }
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (mAnimation && mTexture) {
        states.transform *= getTransform();
        states.texture = mTexture;
        target.draw(mVertices, 4, sf::Quads, states);
    }
}
