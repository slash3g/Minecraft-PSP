#include <math.h>
#include <psptypes.h>

#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Utils/Logger.h>
#include <Aurora/Utils/Timer.h>
#include <Aurora/System/SystemManager.h>
#include <Aurora/Graphics/Camera.h>
#include <Aurora/Utils/StateManager.h>

#include "LameCraft/StateMenu.h"
#include "LameCraft/SplashScreen.h"
#include "LameCraft/SoundManager.h"
#include "LameCraft/TextureHelper.h"

using namespace Aurora::Graphics;
using namespace Aurora::Utils;
using namespace Aurora::System;
using namespace Aurora;

PSP_MODULE_INFO("MineCraftPSP", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU|THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

int main()
{
    SetupCallbacks();

    scePowerSetClockFrequency(333, 333, 166);

    Logger *logger = Logger::Instance();
    logger->LogMessage("Initialize render manager...\n");

    //initialize render manager
    RenderManager::InstancePtr()->Init();
    RenderManager::InstancePtr()->CollectPerformance(true);
    RenderManager::InstancePtr()->InitDebugFont();

    logger->LogMessage("Set perspective...\n");
    //set perspectives
    RenderManager::InstancePtr()->SetOrtho(0,0,0,0,0,0);

    logger->LogMessage("Init and load textures...\n");
    TextureHelper::Instance()->Init();
    logger->LogMessage("Init and load sounds...\n");
    SoundManager::Instance()->Init();

    logger->LogMessage("Init RNG...\n");
    srand(time(NULL));

    logger->LogMessage("Create state manager...\n");
    //new state manager
    StateManager stateManager;
    stateManager.Init();

    RenderManager::InstancePtr()->SetClearColour(0xFF000000);

    logger->LogMessage("Show splash screen...\n");
    //splash screens
    SplashScreen *screen = new SplashScreen(TextureHelper::Instance()->GetTexture(TextureHelper::Genesis),0,0,480,272,3);
    screen->ShowSplash();
    delete screen;

    RenderManager::InstancePtr()->SetClearColour(0xFFFFFFFF);

    logger->LogMessage("New active state...\n");
    //new active state
    StateMenu *statePlay = new StateMenu();
    statePlay->Init();
    stateManager.ChangeState(statePlay);

    logger->LogMessage("RUN!\n");
    //trun
    while ( stateManager.Running() )
    {
        stateManager.HandleEvents();
        stateManager.Update();
        stateManager.Draw();
    }

    sceGuTerm();			// Terminating the Graphics System
    sceKernelExitGame();	// Quits Application

    return 0;
}
