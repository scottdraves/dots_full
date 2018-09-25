#include "GuiApp.h"

void GuiApp::setup() {
    ofSetEscapeQuitsApp(false);
    
    midi.setup();

    ofSoundStream ss;
    soundDevices = ss.getDeviceList();
    nSoundDevices = soundDevices.size();

    audioInputParameters.setName("Audio Input");
    audioInputParameters.add(audioMode.set("audioMode", AUDIO_MODE_MIC, 0, N_AUDIO_MODES-1));
    audioInputParameters.add(soundStreamDevice.set("soundStreamDevice", 0, 0, nSoundDevices-1));
    inputGui.setup(audioInputParameters);

    // Auto select loopback if available
    for (int i = 0; i < soundDevices.size(); ++i) {
        ofSoundDevice &sd = soundDevices[i];
        if (sd.name.find("Soundflower") != string::npos && sd.name.find("2ch") != string::npos) {
            soundStreamDevice.set(i);
        }
    }

    // Debug
    debugParameters.setName("Debug");
    debugParameters.add(drawMode.set("drawMode", 0, 0, 1));
    debugParameters.add(nFlameSequences.set("nFlameSequences", 5, 1, 10));
    debugParameters.add(maxPixels.set("maxPixels", 5000000, 10000, 100000000));
    debugParameters.add(pctToAllowRandom.set("pctToAllowRandom", 0.02, 0, 0.1));
    debugParameters.add(useSavedParams.set("Use saved params", true));
    debugGui.setup(debugParameters);

    // Meta
    metaParams.setName("Meta");
    metaParams.add(wandering.set("wandering", false));
    metaParams.add(fadeFrames.set("fadeFrames", 100, 0, 200));
    metaGui.setup(metaParams);

    trackControlGui.setup("Tracks");

    trackControls.setup();
    trackControls.setName("< >");
    trackControls.add(prevTrackBtn.setup("Prev track"));
    trackControls.add(nextTrackBtn.setup("Next track"));
    trackControls.add(saveTrackBtn.setup("Save track"));
    trackControls.add(newTrackBtn.setup("New track"));
    trackControlGui.add(&trackControls);

    prevTrackBtn.addListener(stateManager.get(), &StateManager::regressTrack);
    nextTrackBtn.addListener(stateManager.get(), &StateManager::advanceTrack);
    saveTrackBtn.addListener(stateManager.get(), &StateManager::saveTrack);
    newTrackBtn.addListener(stateManager.get(),  &StateManager::createTrack);

    sceneControls.setup();
    sceneControls.setName("Scenes");
    sceneControls.add(duplicateSceneBtn.setup("Duplicate scene"));
    sceneControls.add(reloadSceneBtn.setup("Reload scene"));
    sceneControls.add(deleteSceneBtn.setup("Remove scene"));
    trackControlGui.add(&sceneControls);

    duplicateSceneBtn.addListener(stateManager.get(), &StateManager::duplicateScene);
    reloadSceneBtn.addListener(stateManager.get(), &StateManager::reloadScene);
    deleteSceneBtn.addListener(stateManager.get(), &StateManager::deleteScene);

    // State manager must be set up first
    displayGui.setup(stateManager->activeScene.displayParameters);
    trackParamsGui.setup(stateManager->activeTrack.displayParameters);

    inputGui.setPosition(10, 450);
    debugGui.setPosition(inputGui.getPosition().x + inputGui.getWidth() + 10, inputGui.getPosition().y);
    metaGui.setPosition(debugGui.getPosition().x, debugGui.getPosition().y + debugGui.getHeight() + 10);
    trackParamsGui.setPosition(510, 75);
    trackControlGui.setPosition(trackParamsGui.getPosition().x, trackParamsGui.getPosition().y + trackParamsGui.getHeight() + 10);
    displayGui.setPosition(trackControlGui.getPosition().x + trackControlGui.getWidth() + 10, 15);


    float height = MAX(MAX(MAX(MAX(inputGui.getPosition().y + inputGui.getHeight(),
                       debugGui.getPosition().y + inputGui.getHeight()),
                       metaGui.getPosition().y + metaGui.getHeight()),
                       displayGui.getPosition().y + displayGui.getHeight()),
                       trackControlGui.getPosition().y + trackControlGui.getHeight());

    ofSetWindowShape(displayGui.getPosition().x + displayGui.getWidth() + 10,
                     height + 10);

    visuals = NULL;
    audioBuckets = NULL;
    nAudioBuckets = 0;
    frameRate = 0;
    pctParticles = 0;
    mpx = 0;
    mpy = 0;

    ofBackground(0);
    ofSetVerticalSync(false);
}

void GuiApp::update(){
    if (ofGetFrameNum() % 30 == 0) {
        // Every 30 frames, refresh some data...
        ofSoundStream ss;
        soundDevices = ss.getDeviceList();
        if (soundDevices.size() != nSoundDevices) {
            if (soundStreamDevice.get() >= soundDevices.size()) {
                soundStreamDevice.set(0);
            }
            nSoundDevices = soundDevices.size();
            soundStreamDevice.setMax(nSoundDevices);
        }
    }

    stateManager->wandering = wandering.get();
    stateManager->fadeFrames = fadeFrames.get();
}

void GuiApp::draw() {
    char s[512];
    float margin = 10, padding = 10;
    
    ofPushMatrix();
    
    if (frameRate) {
        sprintf(s, "%.2f fps", frameRate);
        ofDrawBitmapString(s, 10, 15);
    }

    if (pctParticles) {
        sprintf(s, "%d%% particles", (int)(pctParticles*100));
        ofDrawBitmapString(s, 150, 15);
    }

    if (stateManager->trackIdx >= 0) {
        sprintf(s, "track: %d / %lu", stateManager->trackIdx, stateManager->tracks.size()-1);
        ofDrawBitmapString(s, 510, 25);
        sprintf(s, "scene: %d / %lu", stateManager->sceneIdx, stateManager->getTrack().scenes.size()-1);
        ofDrawBitmapString(s, 510, 40);

        if (wandering) {
            ofNoFill();
            ofDrawRectangle(510, 50, 100, 15);
            ofFill();
            ofDrawRectangle(510, 50, 100 * stateManager->activeTrack.interpAmt, 15);
        } else if (stateManager->getGenomeInterpolation() > 0.001) {
            ofNoFill();
            ofDrawRectangle(510, 50, 100, 15);
            ofFill();
            ofDrawRectangle(510, 50, 100 * stateManager->getGenomeInterpolation(), 15);
        } else {
            ofDrawBitmapString("Not wandering or fading", 510, 60);
        }
    }



    ofTranslate(0, 17);

    if (visuals) {
        ofTranslate(0, margin);
        // TODO fit visuals to a bounding box
        
        float scale = min(480.0 / visuals->getWidth(), 270.0 / visuals->getHeight());
        float width = visuals->getWidth() * scale;
        float height = visuals->getHeight() * scale;

        ofFill();
        ofSetColor(255, 255);
        ofDrawRectangle(margin, 0, width + padding * 2, height + padding * 2);
        ofSetColor(0, 255);
        ofDrawRectangle(margin + padding, padding, width, height);
        ofSetColor(255, 255);
        visuals->draw(margin + padding, padding, width, height);

        ofTranslate(0, 270 + padding * 2);
    }
    
    if (audioBuckets) {
        ofTranslate(0, 100 + margin);

        ofPushStyle();

        ofSetColor(255);
        ofNoFill();
        ofSetLineWidth(1);
        
        float fftWidth = 415.0, fftHeight = 100;
        float fftBucketWidth = fftWidth / nAudioBuckets;
        
        ofPushMatrix();
        
        ofTranslate(margin, 0);
        ofDrawRectangle(0, 0, fftWidth, -fftHeight);
        ofBeginShape();
        for (int i = 0; i < nAudioBuckets; i++) {
            ofVertex(i * fftBucketWidth, -audioBuckets[i] * fftHeight);
        }
        ofEndShape();
        
        string audioModeDesc;
        if (audioMode == AUDIO_MODE_MIC) {
            audioModeDesc = "Microphone Input";
        } else if (audioMode == AUDIO_MODE_MP3) {
            audioModeDesc = "MP3 Playing";
        } else if (audioMode == AUDIO_MODE_NOISE) {
            audioModeDesc = "White Noise";
        } else {
            audioModeDesc = "No Audio";
        }
        ofDrawBitmapString(audioModeDesc, 5, 15-fftHeight);

        if (soundStreamDevice < soundDevices.size()) {
            ofDrawBitmapString(soundDevices[soundStreamDevice].name, 5, 30-fftHeight);
        }
        
        // Draw audio centroid
        float mappedCentroid = fftWidth * audioCentroid;
        ofSetColor(200, 0, 0);
        ofDrawLine(mappedCentroid, 0, mappedCentroid, -fftHeight);
        ofDrawBitmapString("centroid", mappedCentroid + 4, -30);
        float mappedCentroidMax = fftWidth * stateManager->activeScene.centroidMaxBucket;
        ofSetColor(125, 0, 0);
        ofDrawLine(mappedCentroidMax, 0, mappedCentroidMax, -fftHeight);
        ofDrawBitmapString("centroidMax", mappedCentroidMax + 4, -30);
        
        // Draw MPX, MPY
        float mpxHeight = 90 * mpx;
        float mpyHeight = 90 * mpy;
        ofFill();
        ofSetColor(255);
        ofDrawBitmapString("mpx", fftWidth + 10, -90);
        ofDrawRectangle(fftWidth + 10, 0, 25, -mpxHeight);
        ofDrawBitmapString("mpy", fftWidth + 40, -90);
        ofDrawRectangle(fftWidth + 40, 0, 25, -mpyHeight);
        
        ofPopMatrix();
        ofPopStyle();
    }
    
    ofPopMatrix();

    inputGui.draw();
    debugGui.draw();
    displayGui.draw();
    metaGui.draw();
    trackParamsGui.draw();
    trackControlGui.draw();
}

void GuiApp::handleKey(int key) {
    // None for now
}

void GuiApp::keyPressed(int key) {
    keyPresses.push(key);
}

void GuiApp::keyReleased(int key) {
    if (key == OF_KEY_COMMAND) {
        keyPresses.push(DOTS_KEY_COMMAND_UP);
    }
}
