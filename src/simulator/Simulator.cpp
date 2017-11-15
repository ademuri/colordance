#include "Simulator.hpp"
#include "../controller/LightController.hpp"
#include "../controller/SolidColorEffect.hpp"
#include "SimulatorLightController.hpp"
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include <OgrePlane.h>
#include <OgreRenderWindow.h>
#include <OgreSceneNode.h>
#include <OgreViewport.h>

Simulator::Simulator()
    : OgreBites::ApplicationContext("ColorDance Simulator") {}

bool Simulator::keyPressed(const OgreBites::KeyboardEvent &evt) {
  if (evt.keysym.sym == SDLK_ESCAPE) {
    getRoot()->queueEndRendering();
  }
  if (evt.keysym.sym == SDLK_LEFT) {
    light1->setDiffuseColour(0, 0, 0);
  } else {
    light1->setDiffuseColour(0, 1.0, 0);
  }
  return true;
}

Ogre::Light *createLight(Ogre::SceneManager *scnMgr,
                         Ogre::Vector3 const position, double red, double green,
                         double blue) {
  Ogre::Light *spotLight = scnMgr->createLight();
  // spotLight->setDiffuseColour(red, green, blue);
  // spotLight->setSpecularColour(red, green, blue);
  spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
  spotLight->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z);

  Ogre::SceneNode *spotLightNode =
      scnMgr->getRootSceneNode()->createChildSceneNode();
  spotLightNode->attachObject(spotLight);
  spotLightNode->setDirection(0, 0, -1);
  spotLightNode->setPosition(position);
  spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));

  return spotLight;
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

  light1 = createLight(scnMgr, Ogre::Vector3(-100, 100, 800), 0, 1.0, 0);
  light2 = createLight(scnMgr, Ogre::Vector3(0, 100, 800), 0, 0, 1.0);
  light3 = createLight(scnMgr, Ogre::Vector3(100, 100, 800), 1.0, 0, 0);

  this->controller = new SimulatorLightController(light1, light2, light3);
  this->solidColorEffect = new SolidColorEffect(controller);
  solidColorEffect->run();
}

bool Simulator::frameEnded(const Ogre::FrameEvent &evt) {
  solidColorEffect->run();
}

int main(int argc, char **argv) {
  Simulator app;
  app.initApp();
  app.getRoot()->startRendering();
  app.closeApp();
  return 0;
}
