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
    void draw(float scale);

    float calculateScale(float initialVelocity, float angle, float terminalVelocity, float maxWidth, float maxHeight);

    vec2 mPosition;
    vec2 mInitialPosition;
    float mCircleRadius;

private:
    float mG;
    float mPi;
    Color mColor;
};

void Projectile::setup() {
    mPosition = vec2(0.0f, 0.0f);
    mInitialPosition = getWindowCenter();
    mG = 9.81f;
    mPi = 3.14159265358979323846f;
    mCircleRadius = 5.0f;
    mColor = Color(1.0f, 1.0f, 1.0f);
}

void Projectile::update(float initialVelocity, float angle, float time, float terminalVelocity) {
    mPosition.x = ((initialVelocity * terminalVelocity / mG) * cosf(angle * mPi / 180.0f) * (1.0f - exp(-mG * time / terminalVelocity)));
    mPosition.y = - ((initialVelocity * terminalVelocity / mG) * sinf(angle * mPi / 180.0f) * (1.0f- exp(-mG * time / terminalVelocity)) - terminalVelocity * time);
}

void Projectile::draw(float scale) {
    gl::color(mColor);
    gl::drawSolidCircle(mInitialPosition + (mPosition * scale), mCircleRadius);
}

float Projectile::calculateScale(float initialVelocity, float angle, float terminalVelocity, float maxWidth, float maxHeight)
{
    float scale = 1;
    float posX = 0;
    float posY = 0;
    for (float time = 0; posY >= 0; time += 0.25f) {
        posX = ((initialVelocity * terminalVelocity / mG) * cosf(angle * mPi / 180.0f) * (1.0f - exp(-mG * time / terminalVelocity)));
        posY = ((initialVelocity * terminalVelocity / mG) * sinf(angle * mPi / 180.0f) * (1.0f - exp(-mG * time / terminalVelocity)) - terminalVelocity * time);
        if (posX * scale > maxWidth || posY * scale > maxHeight)
        {
            scale *= 0.9;
        }
    }
    return scale;
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
    float scale;
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

    mParams->addParam("Scale", &scale);
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
    if (mProjectileUnderTest.mPosition.y > 0)
    {
        mProjectileUnderTest.mPosition.y = 0;
        mIsPlaying = false;
    }
    currX = mProjectileUnderTest.mPosition.x;
    currY = -(mProjectileUnderTest.mPosition.y);
    if (currY == -0) currY = 0;
    mParams->setPosition(ivec2(0,0));
}

void MASOSApp::draw()
{
    gl::clear(Color(0.0f, 0.0f, 0.0f));
    mProjectileUnderTest.draw(scale);
    mParams->draw();
}

void MASOSApp::buttonStart()
{
    scale = mProjectileUnderTest.calculateScale(mInitialVelocity, mAngle, mTerminalVelocity, getWindowCenter().x, getWindowCenter().y);
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
