#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include "appConstants.h"
#include "MidiController.h"
#include "StateManager.h"

class ReplaceablePanel : public ofxPanel {
public:
    ReplaceablePanel();
    ~ReplaceablePanel();

    ReplaceablePanel * setup(const ofParameterGroup & parameters);
};

class GuiApp: public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void handleKey(int key);
    void buttonPressed(const void * sender);

    // Patameters that don't need to be serialized/unserialized
    ofParameterGroup audioInputParameters;
    ofParameter<int> audioMode;
    vector<ofSoundDevice> soundDevices;
    int nSoundDevices;
    ofParameter<int> soundStreamDevice;

    ofParameterGroup debugParameters;
    ofParameter<int> maxPixels, drawMode, nFlameSequences;
    ofParameter<float> pctToAllowRandom;
    ofParameter<bool> useSavedParams;

    ofParameterGroup metaParams;
    ofParameter<bool> wandering;
    ofParameter<int> fadeFrames;

    ofxPanel trackControlGui;
    ofxGuiGroup trackControls;
    ofxButton prevTrackBtn, nextTrackBtn;
    ofxButton newTrackBtn, saveTrackBtn;
    ofxGuiGroup sceneControls;
    ofxButton deleteSceneBtn, reloadSceneBtn, duplicateSceneBtn;

    ofxPanel analysisGui, inputGui, debugGui, metaGui;
    ofxPanel trackParamsGui, displayGui;

    // To mirror from ofApp
    float frameRate, pctParticles;
    ofTexture* visuals;
    float *audioBuckets;
    int nAudioBuckets;
    float mpx, mpy;
    float audioCentroid, audioRMS;

    // State manager
    shared_ptr<StateManager> stateManager;

    queue<int> keyPresses;
    MidiController midi;
};
