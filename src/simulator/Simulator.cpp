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
#include "../controller/ColorShiftEffect.hpp"
#include "../controller/StrobeEffect.hpp"
#include "SimulatorLightController.hpp"

#ifdef USE_BOOST

void Simulator::read_handler(const boost::system::error_code &error,
                             std::size_t bytes_transferred) {
  if (error) {
    std::cout << "Error reading serial input: " << error << std::endl;
  } else {
    std::string inputString;
    inputString.append(serialBuf, bytes_transferred);

    // Find the last full number (i.e. the one that ends with a newline).
    std::vector<std::string> inputLines;
    boost::algorithm::split(inputLines, inputString, boost::is_any_of("\n"));

    int lastFullString = inputLines.size() - 1;
    if (serialBuf[bytes_transferred - 1] != '\n') {
      lastFullString--;
    }

    uint16_t val;
    for (int i = lastFullString; i >= 0; i--) {
      std::string str = inputLines[i];
      boost::trim(str);
      if (!str.empty()) {
        val = stoul(str);
        break;
      }
    }

    paramController->Set(Params::kTempo, val);
  }
}
#endif

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
  } else if (evt.keysym.sym == SDLK_LEFT) {
    currentParamIndex--;
  } else if (evt.keysym.sym == SDLK_RIGHT) {
    currentParamIndex++;
  } else if (evt.keysym.sym >= SDLK_0 && evt.keysym.sym <= SDLK_9) {
    numberInputBuffer += '0' + (evt.keysym.sym - SDLK_0);
  } else if (evt.keysym.sym == SDLK_RETURN) {
    paramController->Set(adjustableParams[currentParamIndex],
                         std::stoi(numberInputBuffer));
    numberInputBuffer.clear();
    effect->ChooseLights();
  } else if (evt.keysym.sym == SDLK_SPACE) {
    effect->BeatDetected();
  }

  if (currentParamIndex < 0) {
    currentParamIndex = adjustableParams.size() - 1;
  } else if (currentParamIndex >= adjustableParams.size()) {
    currentParamIndex = 0;
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
  paramController->Set(Params::kWidth, 5);
  paramController->Set(Params::kPan, ParamController::kPanNeutral);
  paramController->Set(Params::kTilt, ParamController::kTiltNeutral);
  effect = new ColorShiftEffect(controller, paramController);
  effect->Run();

#ifdef USE_BOOST
  try {
    // TODO: make this a flag
    serialPort = new boost::asio::serial_port(io_service_, "/dev/ttyACM0");
  } catch (const boost::system::system_error &ex) {
    std::cerr << "Unable to open serial port: " << ex.what() << std::endl;
  }
#endif
}

bool Simulator::frameEnded(const Ogre::FrameEvent &evt) {
  effect->Run();

  if ((keyDownDebounce % 3 == 0) && (keyDownMap[ControlKeys::kDown] ||
      keyDownMap[ControlKeys::kUp])) {
    const Params currentParam = adjustableParams[currentParamIndex];
    uint16_t val = paramController->Get(currentParam);

    if (keyDownMap[ControlKeys::kDown]) {
      val--;
    } else if (keyDownMap[ControlKeys::kUp]) {
      val++;
    }

    val = paramController->WrapParam(currentParam, val);

    paramController->Set(currentParam, val);

    printf("Param %d: %u\n", currentParamIndex,
           paramController->Get(currentParam));
    keyDownDebounce = 0;

    // TODO: only run this once the key is released?
    effect->ChooseLights();
  }

  keyDownDebounce++;

#ifdef USE_BOOST
  if (serialPort != nullptr && serialPort->is_open()) {
    serialPort->async_read_some(
        boost::asio::buffer(serialBuf, kSerialBufSize),
        boost::bind(&Simulator::read_handler, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    io_service_.poll_one();
  }
#endif

  return true;
}

int main(int argc, char **argv) {
  Simulator app;
  app.initApp();
  app.getRoot()->startRendering();
  app.closeApp();
  return 0;
}
