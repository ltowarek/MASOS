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
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

private:
    params::InterfaceGlRef mParams;
    float mInitialVelocity;
    float mAngle;
    float mTerminalVelocity;
    float mTime;
    Projectile mProjectileUnderTest;
    Projectile mProjectileReference;
};

void MASOSApp::setup()
{
    mInitialVelocity = 0.0f;
    mAngle = 0.0f;
    mTerminalVelocity = 0.0f;
    mTime = 0.0f;
    mProjectileUnderTest.setup();
    mProjectileReference.setup();

    mParams = params::InterfaceGl::create(getWindow(), "Parameters", toPixels(ivec2(200, 300)));

    mParams->addParam("Initial Velocity", &mInitialVelocity);
    mParams->addParam("Angle", &mAngle);
    mParams->addParam("Terminal Velocity", &mTerminalVelocity);
    mParams->addParam("Time", &mTime);
}

void MASOSApp::mouseDown( MouseEvent event )
{
}

void MASOSApp::update()
{
    mProjectileUnderTest.update(mInitialVelocity, mAngle, mTime, mTerminalVelocity);
    mProjectileReference.update(mInitialVelocity, mAngle, mTime);
}

void MASOSApp::draw()
{
	gl::clear(Color(0.0f, 0.0f, 0.0f)); 
    mProjectileUnderTest.draw();
    mProjectileReference.draw();
    mParams->draw();
}

CINDER_APP( MASOSApp, RendererGl )
