#include "Ogre.h"
#include "Bites/OgreApplicationContext.h"
#include <iostream>

using namespace Ogre;
using namespace OgreBites;

class NinjaSlash : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
private:
    Entity* ninjaEntity = NULL;
    AnimationState* mAnimationState = NULL;
    InputListener* mInputDevice = NULL;
public:
    NinjaSlash();
    bool mousePressed(const MouseButtonEvent& evt);
    void setup();
    void frameRendered(const Ogre::FrameEvent& evt);
};

NinjaSlash::NinjaSlash() : OgreBites::ApplicationContext()
{
}

bool NinjaSlash::mousePressed(const MouseButtonEvent& evt)
{
    if (evt.button == OgreBites::BUTTON_LEFT)
    {
        mAnimationState->setLoop(false);
        mAnimationState->setEnabled(true);
    }
    return true;
}

void NinjaSlash::setup()
{
    // do not forget to call the base first
    OgreBites::ApplicationContext::setup();

    Root* root = getRoot();

    SceneManager* scnMgr = root->createSceneManager();
    scnMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

    Light* light = scnMgr->createLight("MainLight");
    SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(light);
    lightNode->setPosition(20, 80, 50);

    SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    Camera* cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(5);
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);
    camNode->setPosition(0, 100, 400);

    getRenderWindow()->addViewport(cam);

    ninjaEntity = scnMgr->createEntity("ninja.mesh");
    SceneNode* ogreNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    ogreNode->attachObject(ninjaEntity);
    ogreNode->yaw(Degree(180));

    mAnimationState = ninjaEntity->getAnimationState("Attack3");
    addInputListener(this);
}

void NinjaSlash::frameRendered(const Ogre::FrameEvent& evt)
{
    if (mAnimationState->getEnabled())
    {
        mAnimationState->addTime(evt.timeSinceLastFrame);
        if (mAnimationState->getLength() == mAnimationState->getTimePosition())
        {
            mAnimationState->setTimePosition(0);
            mAnimationState->setEnabled(false);
        }
    }
}

int main()
{
    NinjaSlash app;
    app.initApp();
    app.getRoot()->startRendering();
    app.closeApp();
}
