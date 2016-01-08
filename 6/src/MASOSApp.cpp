#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Projectile {
public:
    void setup();
    void update(float initialVelocity, float angle, float time, float terminalVelocity);
    void draw();

    vec2 mPosition;
    vec2 mInitialPosition;
    float mCircleRadius;

private:
    float mG;
    float mPi;
    Color mColor;
    vec2 mFixedPos;
};

void Projectile::setup() {
    mPosition = vec2(0.0f, 0.0f);
    mInitialPosition = getWindowCenter();
    mG = 9.81f;
    mPi = 3.14159265358979323846f;
    mCircleRadius = 5.0f;
    mColor = Color(1.0f, 1.0f, 1.0f);
    mFixedPos = vec2(0, 0);
}

void Projectile::update(float initialVelocity, float angle, float time, float terminalVelocity) {
    mPosition.x = mInitialPosition.x + ((initialVelocity * terminalVelocity / mG) * cosf(angle * mPi / 180.0f) * (1.0f - exp(-mG * time / terminalVelocity)));
    mPosition.y = mInitialPosition.y - ((initialVelocity * terminalVelocity / mG) * sinf(angle * mPi / 180.0f) * (1.0f- exp(-mG * time / terminalVelocity)) - terminalVelocity * time);
    mFixedPos = mPosition;
    if (mPosition.x >= getWindowWidth() * 0.9)
    {
        mFixedPos.x = getWindowWidth() * 0.9;
    }
    if (mPosition.y <= getWindowHeight() * 0.1)
    {
        mFixedPos.y = getWindowHeight() * 0.1;
    }
}

void Projectile::draw() {
    gl::color(mColor);
    gl::drawSolidCircle(mFixedPos, mCircleRadius);
}

class MASOSApp : public App {
  public:
    MASOSApp::~MASOSApp();
    void setup() override;
    void mouseDown( MouseEvent event ) override;
    void update() override;
    void draw() override;
    void buttonStart();
    void buttonPause();
    void buttonReset();
    void playLoop();
    void reset();
    void drawScale();

private:
    params::InterfaceGlRef mParams;
    float mInitialVelocity;
    float mAngle;
    float mTerminalVelocity;
    float mCurrentTime;
    float mDeltaTime;
    float mStartTime;
    float mEndTime;
    Projectile mProjectileUnderTest;
    shared_ptr<thread> mThreadPlay;
    bool mIsPlaying;
    bool mShouldQuit;
    float currX;
    float currY;
    gl::Texture2dRef mTextTexture;
};

void MASOSApp::setup()
{
    mShouldQuit = false;
    reset();

    mParams = params::InterfaceGl::create(getWindow(), "Parameters", toPixels(ivec2(250, 300)));

    mParams->addParam("Initial Velocity [m/s]", &mInitialVelocity, "min=0");
    mParams->addParam("Angle [']", &mAngle);
    mParams->addParam("Terminal Velocity [m/s]", &mTerminalVelocity);
    mParams->addParam("Current time [s]", &mCurrentTime, "min=0");
    mParams->addParam("Delta time [ms]", &mDeltaTime, "step=25");
    mParams->addParam("Start time [s]", &mStartTime, "min=0");
    mParams->addParam("End time [s]", &mEndTime, "min=0");
    mParams->addButton("Start", bind(&MASOSApp::buttonStart, this));
    mParams->addButton("Pause", bind(&MASOSApp::buttonPause, this));
    mParams->addButton("Reset", bind(&MASOSApp::buttonReset, this));

    mParams->addParam("Pos X", &currX);
    mParams->addParam("Pos Y", &currY);

    mIsPlaying = false;
    mThreadPlay = shared_ptr<thread>(new thread(bind(&MASOSApp::playLoop, this)));
}

void MASOSApp::mouseDown( MouseEvent event )
{
}

void MASOSApp::update()
{
    mProjectileUnderTest.update(mInitialVelocity, mAngle, mCurrentTime, mTerminalVelocity);
    if (mProjectileUnderTest.mPosition.y >= getWindowCenter().y)
    {
        mProjectileUnderTest.mPosition.y = getWindowCenter().y;
        mIsPlaying = false;
    }
    currX = mProjectileUnderTest.mPosition.x - mProjectileUnderTest.mInitialPosition.x;
    currY = -(mProjectileUnderTest.mPosition.y - mProjectileUnderTest.mInitialPosition.y);
    if (currY == -0) currY = 0;
    mParams->setPosition(ivec2(0,0));
}

void MASOSApp::draw()
{
    gl::clear(Color(0.0f, 0.0f, 0.0f));
    mProjectileUnderTest.draw();
    drawScale();
    mParams->draw();
}

void MASOSApp::drawScale()
{
    gl::drawLine(getWindowCenter() + vec2(0, mProjectileUnderTest.mCircleRadius), vec2(getWindowWidth() * 0.95, getWindowCenter().y + mProjectileUnderTest.mCircleRadius));
    gl::drawLine(vec2(getWindowWidth() * 0.95, getWindowCenter().y + mProjectileUnderTest.mCircleRadius), vec2(getWindowWidth() * 0.95, 0));

    Surface8u rendered;
    for (int i = getWindowCenter().x; i < getWindowWidth(); i += 50)
    {
        TextLayout mTextLayout;
        mTextLayout.clear(ColorA(0.8f, 0.2f, 0.2f, 0.2f));
        mTextLayout.setFont(Font("Arial", 18));
        mTextLayout.setColor(Color(1, 1, 1));
        if (currX + getWindowCenter().x < getWindowWidth() * 0.9)
        {
            mTextLayout.addLine(std::to_string(int(i - getWindowCenter().x)));
        }
        else
        {
            mTextLayout.addLine(std::to_string(int((currX + getWindowCenter().x - getWindowWidth() * 0.9) + i - getWindowCenter().x)));
        }
        rendered = mTextLayout.render(true, false);
        mTextTexture = gl::Texture2d::create(rendered);
        gl::draw(mTextTexture, vec2(i - mProjectileUnderTest.mCircleRadius, getWindowCenter().y + 10));
    }
    for (int i = getWindowCenter().y; i > 0; i -= 30)
    {
        TextLayout mTextLayout;
        mTextLayout.clear(ColorA(0.8f, 0.2f, 0.2f, 0.2f));
        mTextLayout.setFont(Font("Arial", 18));
        mTextLayout.setColor(Color(1, 1, 1));
        if (getWindowCenter().y - currY > getWindowHeight() * 0.1)
        {
            mTextLayout.addLine(std::to_string(int(getWindowCenter().y - i)));
        }
        else
        {
            mTextLayout.addLine(std::to_string(int(currY - i + getWindowHeight() * 0.1)));
        }
        rendered = mTextLayout.render(true, false);
        mTextTexture = gl::Texture2d::create(rendered);
        gl::draw(mTextTexture, vec2(getWindowWidth() * 0.95 + 2, i - 14));
    }
}

void MASOSApp::buttonStart()
{
    mIsPlaying = true;
}

void MASOSApp::buttonPause()
{
    mIsPlaying = false;
}

void MASOSApp::buttonReset()
{
    mIsPlaying = false;
    reset();
}

void MASOSApp::reset()
{
    mInitialVelocity = 100.0f;
    mAngle = 70.0f;
    mTerminalVelocity = 50.0f;
    mDeltaTime = 25;
    mCurrentTime = mStartTime = 0.f;
    mEndTime = 7.f;
    mProjectileUnderTest.setup();
}

void MASOSApp::playLoop()
{
    while(!mShouldQuit)
    {
        if (mIsPlaying)
        {
            mCurrentTime += mDeltaTime/1000;
            if (mCurrentTime > mEndTime)
            {
                mIsPlaying = false;
                mCurrentTime = mEndTime;
            } else if (mCurrentTime < mStartTime)
            {
                mIsPlaying = false;
                mCurrentTime = mStartTime;
            }
            _sleep(25);
        }
    }
}

MASOSApp::~MASOSApp()
{
    mShouldQuit = true;
    mThreadPlay->join();
}


CINDER_APP( MASOSApp, RendererGl )
