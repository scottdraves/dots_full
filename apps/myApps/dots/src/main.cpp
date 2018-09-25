#include "ofMain.h"
#include "GuiApp.h"
#include "ofAppGLFWWindow.h"
#include "ofApp.h"
#include "StateManager.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    
    settings.width = 1080;
    settings.height = 720;
    settings.setPosition(ofVec2f(300,0));
    settings.resizable = true;
    settings.setGLVersion(3, 3);
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    
    settings.width = 650;
    settings.height = 720;
    settings.setPosition(ofVec2f(0,0));
    settings.resizable = true;
    settings.shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);

    shared_ptr<StateManager> stateManager(new StateManager());
    stateManager->setup();

    shared_ptr<GuiApp> guiApp(new GuiApp());
    guiApp->stateManager = stateManager;

    shared_ptr<ofApp> mainApp(new ofApp());
    mainApp->gui = guiApp;
    mainApp->stateManager = stateManager;
    
    ofRunApp(guiWindow, guiApp);
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
}
