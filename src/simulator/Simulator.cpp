#include "Simulator.hpp"
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include <OgrePlane.h>
#include <OgreRenderWindow.h>
#include <OgreSceneNode.h>
#include <OgreViewport.h>
#include <iostream>
#include "../controller/DummyParamController.hpp"
#include "../controller/LightController.hpp"
#include "../controller/ParamController.hpp"
#include "../controller/SolidColorEffect.hpp"
#include "../controller/StrobeEffect.hpp"
#include "SimulatorLightController.hpp"

Simulator::Simulator()
    : OgreBites::ApplicationContext("ColorDance Simulator") {}

// This is pretty jank - we manually maintain of map of whether the up and down
// arrow keys are pressed down, and handle repeating when keys are held down
// that way.
bool Simulator::keyPressed(const OgreBites::KeyboardEvent &evt) {
  if (evt.keysym.sym == SDLK_ESCAPE) {
    getRoot()->queueEndRendering();
  } else if (evt.keysym.sym == SDLK_DOWN) {
    keyDownMap[ControlKeys::kDown] = true;
  } else if (evt.keysym.sym == SDLK_UP) {
    keyDownMap[ControlKeys::kUp] = true;
  } else if (evt.keysym.sym >= SDLK_0 && evt.keysym.sym <= SDLK_9) {
    numberInputBuffer += '0' + (evt.keysym.sym - SDLK_0);
  } else if (evt.keysym.sym == SDLK_RETURN) {
    paramController->Set(Params::kTempo, std::stoi(numberInputBuffer));
    numberInputBuffer.clear();
  } else if (evt.keysym.sym == SDLK_SPACE) {
    effect->BeatDetected();
  }
  return true;
}

bool Simulator::keyReleased(const OgreBites::KeyboardEvent &evt) {
  if (evt.keysym.sym == SDLK_DOWN) {
    keyDownMap[ControlKeys::kDown] = false;
  } else if (evt.keysym.sym == SDLK_UP) {
    keyDownMap[ControlKeys::kUp] = false;
  }
  return true;
}

void Simulator::setup() {
  OgreBites::ApplicationContext::setup();

  // register for input events
  addInputListener(this);

  // get a pointer to the already created root
  Ogre::Root *root = getRoot();
  Ogre::SceneManager *scnMgr = root->createSceneManager();
  // register our scene with the RTSS
  Ogre::RTShader::ShaderGenerator *shadergen =
      Ogre::RTShader::ShaderGenerator::getSingletonPtr();
  shadergen->addSceneManager(scnMgr);

  Ogre::SceneNode *camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  camNode->setPosition(1000, 250, 1000);
  camNode->lookAt(Ogre::Vector3(0, 250, 0),
                  Ogre::Node::TransformSpace::TS_WORLD);

  Ogre::Camera *cam = scnMgr->createCamera("cam1");
  cam->setNearClipDistance(5);
  camNode->attachObject(cam);

  Ogre::Viewport *vp = getRenderWindow()->addViewport(cam);
  vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
  cam->setAspectRatio(Ogre::Real(vp->getActualWidth()) /
                      Ogre::Real(vp->getActualHeight()));

  Ogre::Plane ground(Ogre::Vector3::UNIT_Y, 0);
  Ogre::MeshManager::getSingleton().createPlane(
      "ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, ground,
      1000, 1000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
  Ogre::Entity *groundEntity = scnMgr->createEntity("ground");
  groundEntity->setCastShadows(false);
  groundEntity->setMaterialName("Template/OffWhite");
  scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(
      groundEntity);

  Ogre::Plane wall(Ogre::Vector3::UNIT_Z, 0);
  Ogre::MeshManager::getSingleton().createPlane(
      "wall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, wall,
      1000, 500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
  Ogre::Entity *wallEntity = scnMgr->createEntity("wall");
  wallEntity->setCastShadows(false);
  wallEntity->setMaterialName("Template/White");
  Ogre::SceneNode *wallNode =
      scnMgr->getRootSceneNode()->createChildSceneNode();
  wallNode->setPosition(0, 250, 0);
  wallNode->attachObject(wallEntity);

  Ogre::Entity *obstacleEntity = scnMgr->createEntity("ninja.mesh");
  obstacleEntity->setCastShadows(true);
  obstacleEntity->setMaterialName("Template/OffBlack");
  Ogre::SceneNode *obstacleNode =
      scnMgr->getRootSceneNode()->createChildSceneNode();
  obstacleNode->setPosition(100, 0, 300);
  obstacleNode->attachObject(obstacleEntity);

  scnMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
  scnMgr->setShadowTechnique(
      Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);

  controller = new SimulatorLightController(scnMgr);
  paramController = new DummyParamController();
  paramController->Set(Params::kHue0, 120);
  paramController->Set(Params::kTempo, 200);
  paramController->Set(Params::kWidth, 10);
  effect = new StrobeEffect(controller, paramController);
  effect->Run();
}

bool Simulator::frameEnded(const Ogre::FrameEvent &evt) {
  effect->Run();

  if (keyDownDebounce % 3 == 0) {
    uint16_t tempo = paramController->Get(Params::kTempo);

    if (keyDownMap[ControlKeys::kDown]) {
      tempo--;
    } else if (keyDownMap[ControlKeys::kUp]) {
      tempo++;
    }

    if (tempo > 1000) {
      // Wrap around from going "below zero"
      tempo = 255;
    } else if (tempo > 255) {
      tempo = 0;
    }

    paramController->Set(Params::kTempo, tempo);

    std::cout << "Tempo: " << paramController->Get(Params::kTempo) << std::endl;
    keyDownDebounce = 0;
  }

  keyDownDebounce++;

  return true;
}

int main(int argc, char **argv) {
  Simulator app;
  app.initApp();
  app.getRoot()->startRendering();
  app.closeApp();
  return 0;
}
