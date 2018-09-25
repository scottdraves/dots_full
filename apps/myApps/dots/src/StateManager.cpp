//
//  StateManager.cpp
//  dotsLocal
//
//  Created by Christopher Anderson on 4/9/16.
//
//

#include "StateManager.h"

void interpolateParameters(const ofParameterGroup &from, const ofParameterGroup &to,
                           const ofParameterGroup &ret, float t) {
    for(std::size_t i = 0; i < from.size(); i++){
        string type = from.getType(i);
        if(type == typeid(ofParameter <int32_t> ).name()){
            auto fromParam = from.getInt(i);
            auto toParam = to.getInt(i);
            auto retParam = ret.getInt(i);
            retParam.set((toParam.get() - fromParam.get()) * t + fromParam.get());
        }else if(type == typeid(ofParameter <uint32_t> ).name()){
            auto fromParam = from.get<uint32_t>(i);
            auto toParam = to.get<uint32_t>(i);
            auto retParam = ret.get<uint32_t>(i);
            retParam.set((toParam.get() - fromParam.get()) * t + fromParam.get());
        }else if(type == typeid(ofParameter <int64_t> ).name()){
            auto fromParam = from.get<int64_t>(i);
            auto toParam = to.get<int64_t>(i);
            auto retParam = ret.get<int64_t>(i);
            retParam.set((toParam.get() - fromParam.get()) * t + fromParam.get());
        }else if(type == typeid(ofParameter <uint64_t> ).name()){
            auto fromParam = from.get<uint64_t>(i);
            auto toParam = to.get<uint64_t>(i);
            auto retParam = ret.get<uint64_t>(i);
            retParam.set((toParam.get() - fromParam.get()) * t + fromParam.get());
        }else if(type == typeid(ofParameter <int8_t> ).name()){
            auto fromParam = from.get<int8_t>(i);
            auto toParam = to.get<int8_t>(i);
            auto retParam = ret.get<int8_t>(i);
            retParam.set((toParam.get() - fromParam.get()) * t + fromParam.get());
        }else if(type == typeid(ofParameter <uint8_t> ).name()){
            auto fromParam = from.get<uint8_t>(i);
            auto toParam = to.get<uint8_t>(i);
            auto retParam = ret.get<uint8_t>(i);
            retParam.set((toParam.get() - fromParam.get()) * t + fromParam.get());
        }else if(type == typeid(ofParameter <int16_t> ).name()){
            auto fromParam = from.get<int16_t>(i);
            auto toParam = to.get<int16_t>(i);
            auto retParam = ret.get<int16_t>(i);
            retParam.set((toParam.get() - fromParam.get()) * t + fromParam.get());
        }else if(type == typeid(ofParameter <uint16_t> ).name()){
            auto fromParam = from.get<uint16_t>(i);
            auto toParam = to.get<uint16_t>(i);
            auto retParam = ret.get<uint16_t>(i);
            retParam.set((toParam.get() - fromParam.get()) * t + fromParam.get());
        }else if(type == typeid(ofParameter <float> ).name()){
            auto fromParam = from.getFloat(i);
            auto toParam = to.getFloat(i);
            auto retParam = ret.getFloat(i);
            retParam.set((toParam.get() - fromParam.get()) * t + fromParam.get());
        }else if(type == typeid(ofParameter <double> ).name()){
            auto fromParam = from.get<double>(i);
            auto toParam = to.get<double>(i);
            auto retParam = ret.get<double>(i);
            retParam.set((toParam.get() - fromParam.get()) * t + fromParam.get());
        }else if(type == typeid(ofParameter <bool> ).name()){
            auto fromParam = from.getBool(i);
            auto toParam = to.getBool(i);
            auto retParam = ret.get<bool>(i);
            retParam.set(t > 0.5 ? toParam.get() : fromParam.get());
        }else if(type == typeid(ofParameter <ofVec2f> ).name()){
            auto fromParam = from.getVec2f(i);
            auto toParam = to.getVec2f(i);
            auto retParam = ret.getVec2f(i);
            retParam.set((toParam.get() - fromParam.get()) * t + fromParam.get());
        }else if(type == typeid(ofParameter <ofVec3f> ).name()){
            auto fromParam = from.getVec3f(i);
            auto toParam = to.getVec3f(i);
            auto retParam = ret.getVec3f(i);
            retParam.set((toParam.get() - fromParam.get()) * t + fromParam.get());
        }else if(type == typeid(ofParameter <ofVec4f> ).name()){
            auto fromParam = from.getVec4f(i);
            auto toParam = to.getVec4f(i);
            auto retParam = ret.getVec4f(i);
            retParam.set((toParam.get() - fromParam.get()) * t + fromParam.get());
        }else if(type == typeid(ofParameter <ofColor> ).name()){
            auto fromParam = from.getColor(i);
            auto toParam = to.getColor(i);
            auto retParam = ret.getColor(i);
            retParam.set(fromParam.get().getLerped(toParam.get(), t));
        }else if(type == typeid(ofParameter <ofShortColor> ).name()){
            auto fromParam = from.getShortColor(i);
            auto toParam = to.getShortColor(i);
            auto retParam = ret.getShortColor(i);
            retParam.set(fromParam.get().getLerped(toParam.get(), t));
        }else if(type == typeid(ofParameter <ofFloatColor> ).name()){
            auto fromParam = from.getFloatColor(i);
            auto toParam = to.getFloatColor(i);
            auto retParam = ret.getFloatColor(i);
            retParam.set(fromParam.get().getLerped(toParam.get(), t));
        }else if(type == typeid(ofParameter <string> ).name()){
            auto fromParam = from.getString(i);
            auto toParam = to.getString(i);
            auto retParam = ret.getString(i);
            retParam.set(t > 0.5 ? toParam.get() : fromParam.get());
        }else if(type == typeid(ofParameterGroup).name()){
            auto fromParam = from.getGroup(i);
            auto toParam = to.getGroup(i);
            auto retParam = ret.getGroup(i);
            interpolateParameters(fromParam, toParam, retParam, t);
        }else{
            ofLogWarning() << "ofxBaseGroup; no control for parameter of type " << type;
        }
    }
}

template <typename ParameterType>
void setParam(ofParameter<ParameterType>& param, float t) {
    param.set(ofLerp(param.getMin(), param.getMax(), t));
}

void StateManager::setup() {
    loadGenebanks();

    seed = 0;
    initrc(seed);
    fading = false;
    wasFading = false;
    fadeFrames = 100;

    // No genome in activeScene yet
    activeScene.setupParams();
    activeTrack.setupParams();

    // Build default scene
    DotsTrack defaultTrack;
    defaultTrack.trackId = 0;
    defaultTrack.setupParams();

    for (int cpIdx = 0; cpIdx < ncps; ++cpIdx) {
        DotsScene *scene = new DotsScene;
        scene->setupParams();
        scene->motionId = cpIdx;
        scene->setupGenome(&cps[cpIdx]);

        defaultTrack.scenes.push_back(scene);
    }
    defaultTrack.setGenomesFromScenes();
    defaultTrack.copyParamsTo(activeTrack);

    memset(&wanderGenome, 0, sizeof(flam3_genome));

    tracks.push_back(std::move(defaultTrack));

    maxTrackId = 0;

    // Now load tracks from file
    loadAllParamsFromFile();

    wandering = false;
    trackIdx = 0;
    sceneIdx = -1;
    advanceScene();
}

void StateManager::loadGenebanks() {
    {
        FILE *in;
        char *inf = "../../../data/live.flam3";
        in = fopen(inf, "rb");
        if (!in) {
            cout << "couldn't open flame3, exiting" << endl;
            std::exit(1);
        }
        cps = flam3_parse_from_file(in, inf, flam3_defaults_on, &ncps);
        for (int i = 0; i < ncps; i++) {
            cps[i].time = (double) i;
        }
        if (!cps) {
            cout << "couldn't read flame3, exiting" << endl;
            std::exit(1);
        }
    }
    {
        FILE *in;
        char *inf = "../../../data/genebank2.flam3";
        in = fopen("../../../data/genebank2.flam3", "rb");
        if (!in) {
            cout << "couldn't open genebank, exiting" << endl;
            std::exit(1);
        }
        genebank = flam3_parse_from_file(in, inf, flam3_defaults_on, &ngenebank);
    }
}

void StateManager::genomeModified(int & newGenomeIdx) {
//    if (newGenomeIdx != currScene->genomeIdx)
//        trackDirty = true;
}

void StateManager::update() {
    if (currScene && nextScene) {
        if (wandering) {
            applyParameterInterpolation(activeTrack.interpAmt);
        } else {
            float t = getGenomeInterpolation();

            if (fading && fadeForward && t > 0.999) {
                advanceScene();
                fading = false;
            } else if (fading && !fadeForward && t < 0.001) {
                fading = false;
            } else if (t > 0.001) {
                applyParameterInterpolation(t);
            }
        }
    }

//    if (!wandering) {
//        midiToSceneParams();
//    } else {
//        // TODO: apply to midi adjust
//    }
}

void StateManager::flameWanderUpdate() {
    DotsTrack &t = getTrack();

    activeTrack.wanderPos += activeTrack.getWanderSpeed();

    if (activeTrack.wanderPos >= t.nGenomes)
        activeTrack.wanderPos = 0;

    int currCP = (int)activeTrack.wanderPos;

    if (lastCP != currCP) {
        advanceScene();
        lastCP = currCP;
    }

    activeTrack.interpAmt = activeTrack.wanderPos - currCP;

    // cpTrackOrder has n+1 - last is a copy of first
    flam3_interpolate(t.genomes, t.nGenomes + 1, activeTrack.wanderPos, 0, &wanderGenome);
}

void StateManager::flameUpdate(float audioRMS) {
    // Rotate both the same amount
    float speed = activeScene.baseSpeed + audioRMS * activeScene.rmsSpeedMult;
    flam3_rotate(currScene->genome, speed, flam3_inttype_log);
    flam3_rotate(nextScene->genome, speed, flam3_inttype_log);
}

int StateManager::randomi(int n) {
    int i = flam3_random_isaac_01(&rc) * ncps;
    return i % n;
}

float StateManager::getGenomeInterpolation() {
//    return activeTrack.wanderSpeed;

    if (!fading) return 0;

    long currFrame = ofGetFrameNum();
    long delta = currFrame - fadeStartFrame;

    // TODO: make this a param
    float split = (float)delta / fadeFrames;

    // We might be fading backwards
    if (fadeForward) {
        return ofClamp(split, 0, 1);
    } else {
        return ofClamp(1.0f - split, 0, 1);
    }
}

void StateManager::loadGenome(flam3_genome *dest) {
    if (!currScene) return;

    flam3_copy(dest, currScene->genome);
}

void StateManager::killCurrent() {
    flam3_genome result;
    memset(&result, 0, sizeof(flam3_genome));
    initrc(time(NULL));
    if (randomi(2)) {
        cout << "Crossing random with random" << endl;
        int parent0 = randomi(ncps);
        int parent1 = randomi(ncps);
        flam3_cross(&cps[parent0], &cps[parent1], &result, CROSS_NOT_SPECIFIED, &rc, NULL);
    } else {
        cout << "Crossing random with genebank" << endl;
        int parent0 = randomi(ncps);
        int parent1 = randomi(ngenebank);
        flam3_cross(&cps[parent0], &genebank[parent1], &result, CROSS_NOT_SPECIFIED, &rc, NULL);
    }

    flam3_copy(getScene().genome, &result);

    getTrack().setGenomesFromScenes();
    ofNotifyEvent(onTrackUpdate, trackIdx);
}

void StateManager::mateCurrent() {
    flam3_genome result;
    memset(&result, 0, sizeof(flam3_genome));
    initrc(time(NULL));
    int parent1 = random()%ncps;
    flam3_cross(getScene().genome, &cps[parent1], &result, CROSS_NOT_SPECIFIED, &rc, NULL);
    flam3_copy(getScene().genome, &result);

    getTrack().setGenomesFromScenes();
    ofNotifyEvent(onTrackUpdate, trackIdx);
}

void StateManager::mutateCurrent() {
    flam3_genome result;
    memset(&result, 0, sizeof(flam3_genome));
    initrc(time(NULL));
    int ivars[flam3_nvariations];
    for (int i = 0; i < flam3_nvariations; i++)
        ivars[i] = i;

    flam3_mutate(getScene().genome, MUTATE_NOT_SPECIFIED, ivars, flam3_nvariations, 0, 0.2, &rc, NULL);

    getTrack().setGenomesFromScenes();
    ofNotifyEvent(onTrackUpdate, trackIdx);
}

void StateManager::initrc(long sed) {
    /* Set up the isaac rng */
    for (int lp = 0; lp < RANDSIZ; lp++)
        rc.randrsl[lp] = sed;
    irandinit(&rc,1);
}

void StateManager::midiToSceneParams(MidiController &midi) {
    if (midi.sliders[0] > 0) setParam(activeScene.clearSpeed, midi.sliders[0]);
    if (midi.sliders[1] > 0) setParam(activeScene.overallScale, midi.sliders[1]);
    if (midi.sliders[2] > 0) setParam(activeScene.basePointRadius, midi.sliders[2]);
    if (midi.sliders[3] > 0) setParam(activeScene.maxLineLength, midi.sliders[3]);
    if (midi.sliders[4] > 0) setParam(activeScene.audioEffectSize1, midi.sliders[4]);
    if (midi.sliders[5] > 0) setParam(activeScene.audioEffectSize2, midi.sliders[5]);
    if (midi.sliders[6] > 0) setParam(activeScene.audioEffectSize3, midi.sliders[6]);
    if (midi.sliders[7] > 0) setParam(activeScene.audioEffectSize4, midi.sliders[7]);
}

DotsTrack& StateManager::getTrack() {
    return tracks[trackIdx];
}

DotsScene& StateManager::getScene() {
    return *currScene;
}

void StateManager::regressTrack() {
    cout << "Regress track" << endl;
    if (trackIdx < 0) return;

    activeTrack.copyParamsTo(getTrack());

    trackIdx--;
    if (trackIdx < 0) {
        trackIdx = tracks.size() - 1;
    }

    getTrack().copyParamsTo(activeTrack);

    sceneIdx = -1;
    advanceScene();

    ofNotifyEvent(onTrackChange, trackIdx);
}

void StateManager::advanceTrack() {
    cout << "Advance track" << endl;
    if (trackIdx < 0) return;

    activeTrack.copyParamsTo(getTrack());

    trackIdx++;
    if (trackIdx >= tracks.size()) {
        trackIdx = 0;
    }

    getTrack().copyParamsTo(activeTrack);

    sceneIdx = -1;
    advanceScene();

    ofNotifyEvent(onTrackChange, trackIdx);
}

void StateManager::saveTrack() {
    cout << "Save track" << endl;
    activeScene.copyParamsTo(*currScene);
    activeTrack.copyParamsTo(getTrack());
    getTrack().setGenomesFromScenes();
    serializeCurrentTrackToFile();
}

void StateManager::fadeSceneFwd() {
    if (wandering) return;

    if (!fading) {
        activeScene.copyParamsTo(*currScene);

        fading = true;
        fadeStartFrame = ofGetFrameNum();
        fadeForward = true;
    } else {
        advanceScene();
        fading = false;
    }
}

void StateManager::fadeSceneRev() {
    if (wandering) return;

    if (!fading) {
        regressScene();

        fading = true;
        fadeStartFrame = ofGetFrameNum();
        fadeForward = false;
    } else {
        currScene->copyParamsTo(activeScene);
        fading = false;
    }
}

void StateManager::regressScene() {
    if (trackIdx < 0) return;

    sceneIdx--;
    int nextIdx = sceneIdx + 1;

    int nTracks = getTrack().scenes.size();
    if (sceneIdx < 0) {
        sceneIdx = nTracks-1;
        nextIdx = 0;
    }

    if (nextIdx >= nTracks) {
        nextIdx = 0;
    }

    // Save what we're coming from
    if (currScene && !wandering && !fading) {
        activeScene.copyParamsTo(*currScene);
    }

    currScene = tracks[trackIdx].scenes[sceneIdx];
    nextScene = tracks[trackIdx].scenes[nextIdx];

    // Load what we're going to
    currScene->copyParamsTo(activeScene);

    ofNotifyEvent(onSceneChange, sceneIdx);
}

void StateManager::advanceScene() {
    if (trackIdx < 0) return;

    sceneIdx++;
    int nextIdx = sceneIdx + 1;

    int nTracks = getTrack().scenes.size();
    if (sceneIdx >= nTracks) {
        sceneIdx = 0;
        nextIdx = 1;
    }

    if (nextIdx >= nTracks) {
        nextIdx = 0;
    }

    // Save what we're coming from
    if (currScene && !wandering && !fading) {
        activeScene.copyParamsTo(*currScene);
    }

    currScene = tracks[trackIdx].scenes[sceneIdx];
    nextScene = tracks[trackIdx].scenes[nextIdx];

    // Load what we're going to
    currScene->copyParamsTo(activeScene);

    ofNotifyEvent(onSceneChange, sceneIdx);
}

void StateManager::reloadScene() {
    cout << "Reload scene" << endl;
    currScene->copyParamsTo(activeScene);
    getTrack().setGenomesFromScenes();

    ofNotifyEvent(onSceneChange, sceneIdx);
}

void StateManager::deleteScene() {
    cout << "Delete scene" << endl;

    vector<DotsScene *> &scenes = tracks[trackIdx].scenes;
    scenes.erase(scenes.begin() + sceneIdx);
    getTrack().setGenomesFromScenes();

    // TODO: less ham-fisted way to do this
    regressScene();
    advanceScene();

    ofNotifyEvent(onSceneChange, sceneIdx);
}

// TODO: unify with the below
void StateManager::duplicateScene() {
    cout << "Duplicate scene" << endl;

    if (!currScene) return;

    DotsScene *cpy = new DotsScene;
    cpy->setupParams();
    activeScene.copyParamsTo(*cpy);
    currScene->copyGenomeTo(*cpy);

    vector<DotsScene *> &destScenes = tracks[trackIdx].scenes;
    destScenes.insert(destScenes.begin() + sceneIdx + 1, cpy);
    getTrack().setGenomesFromScenes();

    ofNotifyEvent(onTrackUpdate, trackIdx);
    ofNotifyEvent(onSceneChange, sceneIdx);
}

void StateManager::createTrack() {
    DotsTrack track;
    track.trackId = nextTrackId();
    track.setupParams();
    activeTrack.copyParamsTo(getTrack());

    DotsScene *scene = new DotsScene;
    scene->setupParams();
    activeScene.copyParamsTo(*scene);
    getScene().copyGenomeTo(*scene);

    track.scenes.push_back(scene);
    track.setGenomesFromScenes();

    tracks.push_back(std::move(track));
}

// TODO: clean up if possible
void StateManager::applyParameterInterpolation(float t) {
    if (t < 0.001) return;
    
    activeScene.pointRadiusAudioScaleAmt.set(ofLerp(currScene->pointRadiusAudioScaleAmt.get(), nextScene->pointRadiusAudioScaleAmt.get(), t));
    activeScene.pointRadiusAudioScale.set(ofLerp(currScene->pointRadiusAudioScale.get(), nextScene->pointRadiusAudioScale.get(), t));
    activeScene.fftDecayRate.set(ofLerp(currScene->fftDecayRate.get(), nextScene->fftDecayRate.get(), t));
    activeScene.rmsMultiple.set(ofLerp(currScene->rmsMultiple.get(), nextScene->rmsMultiple.get(), t));
    activeScene.centroidMaxBucket.set(ofLerp(currScene->centroidMaxBucket.get(), nextScene->centroidMaxBucket.get(), t));
    activeScene.mpxSmoothingFactor.set(ofLerp(currScene->mpxSmoothingFactor.get(), nextScene->mpxSmoothingFactor.get(), t));
    activeScene.mpySmoothingFactor.set(ofLerp(currScene->mpySmoothingFactor.get(), nextScene->mpySmoothingFactor.get(), t));
    activeScene.baseSpeed.set(ofLerp(currScene->baseSpeed.get(), nextScene->baseSpeed.get(), t));
    activeScene.rmsSpeedMult.set(ofLerp(currScene->rmsSpeedMult.get(), nextScene->rmsSpeedMult.get(), t));
    activeScene.clearSpeed.set(ofLerp(currScene->clearSpeed.get(), nextScene->clearSpeed.get(), t));
    activeScene.particleAlpha.set(ofLerp(currScene->particleAlpha.get(), nextScene->particleAlpha.get(), t));
    activeScene.overallScale.set(ofLerp(currScene->overallScale.get(), nextScene->overallScale.get(), t));
    activeScene.saturationPct.set(ofLerp(currScene->saturationPct.get(), nextScene->saturationPct.get(), t));
    activeScene.basePointRadius.set(ofLerp(currScene->basePointRadius.get(), nextScene->basePointRadius.get(), t));
    activeScene.maxLineLength.set(ofLerp(currScene->maxLineLength.get(), nextScene->maxLineLength.get(), t));
    activeScene.audioEffectSize1.set(ofLerp(currScene->audioEffectSize1.get(), nextScene->audioEffectSize1.get(), t));
    activeScene.audioEffectSize2.set(ofLerp(currScene->audioEffectSize2.get(), nextScene->audioEffectSize2.get(), t));
    activeScene.audioEffectSize3.set(ofLerp(currScene->audioEffectSize3.get(), nextScene->audioEffectSize3.get(), t));
    activeScene.audioEffectSize4.set(ofLerp(currScene->audioEffectSize4.get(), nextScene->audioEffectSize4.get(), t));
}

void StateManager::serializeCurrentTrackToFile() {
    cout << "Saving track idx " << trackIdx << " / id " << getTrack().trackId << " to file." << endl;

    char paramFilename[255];
    sprintf(paramFilename, "params/track-%03d.xml", getTrack().trackId);

    char flameFilename[255];
    sprintf(flameFilename, "genomes/track-%03d.xml", getTrack().trackId);

    ofFile flameFile(ofToDataPath(flameFilename), ofFile::WriteOnly);
    flameFile << "<genomes>" << endl;

    settings.clear();
    settings.addTag("track");
    settings.pushTag("track");

    settings.serialize(getTrack().displayParameters);

    settings.addTag("scenes");
    settings.pushTag("scenes");
    int nScenes = getTrack().scenes.size();
    for (int i = 0; i < nScenes; ++i) {
        DotsScene *scene = getTrack().scenes[i];
        settings.addTag("scene");
        settings.pushTag("scene", i);
        settings.serialize(scene->displayParameters);
        settings.popTag();

        flameFile << flam3_print_to_string(scene->genome) << endl;
    }
    settings.popTag();

    settings.popTag();
    settings.save(paramFilename);

    flameFile << "</genomes>" << endl;
    flameFile.close();
}

void StateManager::loadAllParamsFromFile() {
    map<int, DotsTrack> trackMap;

    ofDirectory dir("params");
    for (auto &file : dir.getFiles()) {
        settings.clear();
        settings.load(file.path());

        string filename = file.getFileName();
        if (filename.find("track") != 0) continue;

        // Load track id from filename
        int trackId = stoi(filename.substr(6, 9));

        // Skip 0th, even if it's saved.
        if (trackId == 0) continue;

        DotsTrack track;
        track.trackId = trackId;
        track.setupParams();

        settings.clear();
        settings.loadFile(file.getAbsolutePath());
        settings.pushTag("track");

        settings.deserialize(track.displayParameters);

        settings.pushTag("scenes");
        int nScenes = settings.getNumTags("scene");
        for (int i = 0; i < nScenes; ++i) {
            DotsScene *scene = new DotsScene;
            scene->setupParams();

            settings.pushTag("scene", i);
            settings.deserialize(scene->displayParameters);
            settings.popTag();

            track.scenes.push_back(scene);
        }

        settings.popTag();
        settings.popTag();

        if (track.scenes.size() == 0) continue;

        cout << "Loading track id " << trackId << " from file." << endl;
        trackMap[trackId] = track;
    }

    dir.open("genomes");
    for (auto &file : dir.getFiles()) {
        string filename = file.getFileName();

        if (filename.find("track") != 0) continue;

        // Load track idx from filename
        int trackId = stoi(filename.substr(6, 9));

        // Skip 0th, even if it's saved.
        if (trackId == 0) continue;

        // Somehow we're missing genomes!
        if (trackMap.count(trackId) == 0) continue;

        DotsTrack &track = trackMap.at(trackId);

        char *fn = strdup(file.getAbsolutePath().c_str());
        FILE *f = fopen(fn, "rb");
        track.genomes = flam3_parse_from_file(f, fn, flam3_defaults_on, &track.nGenomes);

        int i = 0;
        for (auto &scene : track.scenes) {
            scene->genome = (flam3_genome *)malloc(sizeof(flam3_genome));
            memset(scene->genome, 0, sizeof(flam3_genome));
            flam3_copy(scene->genome, track.genomes + i);
            i++;
        }
        track.setGenomesFromScenes();
    }

    for (auto &pair : trackMap) {
        tracks.push_back(std::move(pair.second));
        if (pair.second.trackId > maxTrackId)
            maxTrackId = pair.second.trackId;
    }

    // Sort tracks by idx
    std::sort(tracks.begin(), tracks.end(),
              [](const DotsTrack & a, const DotsTrack & b) -> bool
              {
                  return a.trackId < b.trackId;
              });
}