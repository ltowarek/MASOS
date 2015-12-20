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
    void update(float initialVelocity, float angle, float time);
    void draw();
private:
    vec2 mPosition;
    vec2 mInitialPosition;
    float mG;
    float mPi;
    float mCircleRadius;
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
    mPosition.x = mInitialPosition.x + ((initialVelocity * terminalVelocity / mG) * cosf(angle * mPi / 180.0f) * (1.0f - exp(-mG * time / terminalVelocity)));
    mPosition.y = mInitialPosition.y - ((initialVelocity * terminalVelocity / mG) * sinf(angle * mPi / 180.0f) * (1.0f- exp(-mG * time / terminalVelocity)) - terminalVelocity * time);
}

void Projectile::update(float initialVelocity, float angle, float time) {
    mPosition.x = mInitialPosition.x + (initialVelocity * time * cosf(angle * mPi / 180.0f));
    mPosition.y = mInitialPosition.y - (initialVelocity * time * sinf(angle * mPi / 180.0f) - 0.5f * mG * time * time);
}

void Projectile::draw() {
    gl::color(mColor);
    gl::drawSolidCircle(mPosition, mCircleRadius);
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
    Projectile mProjectileReference;
    shared_ptr<thread> mThreadPlay;
    bool mIsPlaying;
    bool mShouldQuit;
};

void MASOSApp::setup()
{
    mShouldQuit = false;
    reset();

    mParams = params::InterfaceGl::create(getWindow(), "Parameters", toPixels(ivec2(200, 300)));

    mParams->addParam("Initial Velocity", &mInitialVelocity, "min=0");
    mParams->addParam("Angle", &mAngle);
    mParams->addParam("Terminal Velocity", &mTerminalVelocity);
    mParams->addParam("Current time", &mCurrentTime, "min=0");
    mParams->addParam("Delta time", &mDeltaTime, "step=25");
    mParams->addParam("Start time", &mStartTime, "min=0");
    mParams->addParam("End time", &mEndTime, "min=0");
    mParams->addButton("Start", bind(&MASOSApp::buttonStart, this));
    mParams->addButton("Pause", bind(&MASOSApp::buttonPause, this));
    mParams->addButton("Reset", bind(&MASOSApp::buttonReset, this));

    mIsPlaying = false;
    mThreadPlay = shared_ptr<thread>(new thread(bind(&MASOSApp::playLoop, this)));
}

void MASOSApp::mouseDown( MouseEvent event )
{
}

void MASOSApp::update()
{
    mProjectileUnderTest.update(mInitialVelocity, mAngle, mCurrentTime, mTerminalVelocity);
    mProjectileReference.update(mInitialVelocity, mAngle, mCurrentTime);
}

void MASOSApp::draw()
{
    gl::clear(Color(0.0f, 0.0f, 0.0f));
    mProjectileUnderTest.draw();
    mProjectileReference.draw();
    mParams->draw();
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
    mInitialVelocity = 20.0f;
    mAngle = 45.0f;
    mTerminalVelocity = 0.0f;
    mDeltaTime = 0.1f;
    mCurrentTime = mStartTime = 0.f;
    mEndTime = 60.f;
    mProjectileUnderTest.setup();
    mProjectileReference.setup();
}

void MASOSApp::playLoop()
{
    while(!mShouldQuit)
    {
        if (mIsPlaying) {
            mCurrentTime += mDeltaTime;
            if (mCurrentTime > mEndTime)
            {
                mIsPlaying = false;
            }
        }
        _sleep(1000 * mDeltaTime);
    }
}

MASOSApp::~MASOSApp()
{
    mShouldQuit = true;
    mThreadPlay->join();
}


CINDER_APP( MASOSApp, RendererGl )
