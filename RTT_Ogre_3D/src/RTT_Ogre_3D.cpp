//============================================================================
// Name        : RTT_Ogre_3D.cpp
// Author      : Nysomin
// Copyright   : 2011, GNU GPLv3
// Description : Ogre 3D rendering engine for RealTimeTactics
//============================================================================

#include "RTT_Ogre_3D.h"
#include <OgreException.h>
#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>

using namespace RTT;
using namespace Ogre;

//-------------------------------------------------------------------------------------
RTT_Ogre_3D::RTT_Ogre_3D(void)
	: rttRoot(0),
	rttResourcesCfg(StringUtil::BLANK),
	rttPluginsCfg(StringUtil::BLANK)
{
}
//-------------------------------------------------------------------------------------
RTT_Ogre_3D::~RTT_Ogre_3D(void)
{
	delete rttRoot;
}

bool RTT_Ogre_3D::go(void)
{
#ifdef _DEBUG
	rttResourcesCfg = "resources_d.cfg";
    rttPluginsCfg = "plugins_d.cfg";
#else
    rttResourcesCfg = "resources.cfg";
    rttPluginsCfg = "plugins.cfg";
#endif

    // construct Ogre::Root root node
    rttRoot = new Root(rttPluginsCfg);


    // set up resources
    // Load resource paths from config file
    ConfigFile cf;
    cf.load(rttResourcesCfg);
    // Parse resource config file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }

    // configure
    // Show the configuration dialog and initialise the system
    if(!(rttRoot->restoreConfig() || rttRoot->showConfigDialog()))  // Test to see if the render window can/should be launched
    {
        return false;
    }
    rttWindow = rttRoot->initialise(true, "Real Time Tactics Render Window");

    // Set default mipmap level (note: some APIs ignore this)
    TextureManager::getSingleton().setDefaultNumMipmaps(5);
    // initialise all resource groups
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    rttSceneManager = rttRoot->createSceneManager("DefaultSceneManager"); //Create a generic screen manager

    rttCamera = rttSceneManager->createCamera("PrimaryCamera"); //Create our primary camera in our screen manager
    //The camera needs positioning
    rttCamera->setPosition(Vector3(0,7.5,8.5)); // Position it at 80 in Z direction
    rttCamera->lookAt(Vector3(0,0,-5)); // Look back along -Z
    rttCamera->setNearClipDistance(5); // This is how close an object can be to the camera before it is "clipped", or not rendered

    // Create one viewport, entire window, this is where the camera view is rendered
    Viewport* vp = rttWindow->addViewport(rttCamera);
    vp->setBackgroundColour(ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    rttCamera->setAspectRatio(
    Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

    //START ADDING OBJECTS TO RENDER          Time for some objects for testing ***************************************

    // Set ambient light and shadows
    rttSceneManager->setAmbientLight(ColourValue(0, 0, 0));
    rttSceneManager->setShadowTechnique(SHADOWTYPE_TEXTURE_ADDITIVE);
    //Shadowmaps	EXPERIMENTAL  **********BROKEN************
    /*
    rttSceneManager->setShadowTexturePixelFormat(Ogre::PF_FLOAT16_R);
    rttSceneManager->setShadowTextureSelfShadow(true);
    rttSceneManager->setShadowTextureCasterMaterial("ShadowCaster");
    rttSceneManager->setShadowTextureReceiverMaterial("ShadowReceiver");
     */

    Entity* dirtTile = rttSceneManager->createEntity("DirtTile1", "DirtTile.mesh");
    dirtTile->setCastShadows(true);
    SceneNode* dirtTileNode = rttSceneManager->getRootSceneNode()->createChildSceneNode("DirtTile1");
    dirtTileNode->attachObject(dirtTile);

    Entity* blueMarine = rttSceneManager->createEntity("BlueMarine", "BlueMarine.mesh");
    blueMarine->setCastShadows(true);
    SceneNode* blueMarineNode = rttSceneManager->getRootSceneNode()->createChildSceneNode("BlueMarine");
    blueMarineNode->attachObject(blueMarine);

    Entity* groundPlane = rttSceneManager->createEntity("Ground", "Plane.mesh");
    groundPlane->setMaterialName("Claygreen");
    groundPlane->setCastShadows(false);
    SceneNode* groundPlaneNode = rttSceneManager->getRootSceneNode()->createChildSceneNode("Ground");
    groundPlaneNode->attachObject(groundPlane);
    groundPlaneNode->scale(25,25,25);


    // Create a light
    Light* mainLight = rttSceneManager->createLight("MainLight");
    mainLight->setType(Light::LT_POINT);
    //mainLight->mCastShadows=true;
    mainLight->setPosition(20,30,15);
    //mainLight->setCastShadows(true);
    mainLight->setCastShadows(true);

    rttSceneManager->setSkyDome(true, "Examples/CloudySky2", 4, 5, 3000);

    //END OBJECTS       ************************************************************************************************

    //where the magic happens:  Render loop!!!
    while(true)
    {
        // Pump window messages for nice behaviour
        WindowEventUtilities::messagePump();

        if(rttWindow->isClosed())
        {
            return false;
        }

        // Render a frame
        if(!rttRoot->renderOneFrame()) return false;
    }

    return true;
}


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])//*************************************************************************FOUND MAIN!!!************************************************************************
#endif
    {
        // Create application object
        RTT_Ogre_3D app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
