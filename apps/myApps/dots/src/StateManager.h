//
//  StateManager.hpp
//  dotsLocal
//
//  Created by Christopher Anderson on 4/9/16.
//
//

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include "appConstants.h"
#include "MidiController.h"

extern "C" {
#include "flam3.h"
}

static void copyParameters(const ofParameterGroup &from, const ofParameterGroup &to) {
    for(std::size_t i = 0; i < from.size(); i++){
        string type = from.getType(i);
        if(type == typeid(ofParameter <int32_t> ).name()){
            auto fromParam = from.getInt(i);
            auto toParam = to.getInt(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameter <uint32_t> ).name()){
            auto fromParam = from.get<uint32_t>(i);
            auto toParam = to.get<uint32_t>(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameter <int64_t> ).name()){
            auto fromParam = from.get<int64_t>(i);
            auto toParam = to.get<int64_t>(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameter <uint64_t> ).name()){
            auto fromParam = from.get<uint64_t>(i);
            auto toParam = to.get<uint64_t>(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameter <int8_t> ).name()){
            auto fromParam = from.get<int8_t>(i);
            auto toParam = to.get<int8_t>(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameter <uint8_t> ).name()){
            auto fromParam = from.get<uint8_t>(i);
            auto toParam = to.get<uint8_t>(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameter <int16_t> ).name()){
            auto fromParam = from.get<int16_t>(i);
            auto toParam = to.get<int16_t>(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameter <uint16_t> ).name()){
            auto fromParam = from.get<uint16_t>(i);
            auto toParam = to.get<uint16_t>(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameter <float> ).name()){
            auto fromParam = from.getFloat(i);
            auto toParam = to.getFloat(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameter <double> ).name()){
            auto fromParam = from.get<double>(i);
            auto toParam = to.get<double>(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameter <bool> ).name()){
            auto fromParam = from.getBool(i);
            auto toParam = to.getBool(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameter <ofVec2f> ).name()){
            auto fromParam = from.getVec2f(i);
            auto toParam = to.getVec2f(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameter <ofVec3f> ).name()){
            auto fromParam = from.getVec3f(i);
            auto toParam = to.getVec3f(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameter <ofVec4f> ).name()){
            auto fromParam = from.getVec4f(i);
            auto toParam = to.getVec4f(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameter <ofColor> ).name()){
            auto fromParam = from.getColor(i);
            auto toParam = to.getColor(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameter <ofShortColor> ).name()){
            auto fromParam = from.getShortColor(i);
            auto toParam = to.getShortColor(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameter <ofFloatColor> ).name()){
            auto fromParam = from.getFloatColor(i);
            auto toParam = to.getFloatColor(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameter <string> ).name()){
            auto fromParam = from.getString(i);
            auto toParam = to.getString(i);
            toParam.set(fromParam.get());
        }else if(type == typeid(ofParameterGroup).name()){
            auto fromParam = from.getGroup(i);
            auto toParam = to.getGroup(i);
            copyParameters(fromParam, toParam);
        }else{
            ofLogWarning() << "ofxBaseGroup; no control for parameter of type " << type;
        }
    }
}

typedef struct DotsScene {
    flam3_genome *genome = NULL;

    ofParameterGroup displayParameters;

    ofParameterGroup audioAnalysisParameters;
    ofParameter<float> fftDecayRate;
    ofParameter<float> centroidMaxBucket;
    ofParameter<float> rmsMultiple;
    ofParameter<float> mpxSmoothingFactor, mpySmoothingFactor;

    ofParameterGroup drawingParams;
    ofParameter<float> clearSpeed;
    ofParameter<float> particleAlpha;
    ofParameter<float> overallScale;
    ofParameter<float> saturationPct;

    ofParameterGroup speedParams;
    ofParameter<float> baseSpeed, rmsSpeedMult;

    ofParameterGroup dotParams;
    ofParameter<float> pointRadiusAudioScaleAmt;
    ofParameter<float> basePointRadius, pointRadiusAudioScale;

    ofParameterGroup lineParams;
    ofParameter<float> maxLineLength;

    ofParameterGroup audioEffectParams;
    ofParameter<int> motionId;
    ofParameter<float> audioEffectSize1, audioEffectSize2, audioEffectSize3, audioEffectSize4;

    void setupGenome(flam3_genome *src) {
        if (genome) free(genome);

        genome = (flam3_genome *)malloc(sizeof(flam3_genome));
        memset(genome, 0, sizeof(flam3_genome));
        flam3_copy(genome, src);
    }

    void setupParams() {
        displayParameters.clear();
        audioAnalysisParameters.clear();
        drawingParams.clear();
        speedParams.clear();
        dotParams.clear();
        lineParams.clear();
        audioEffectParams.clear();

        // Audio analysis
        audioAnalysisParameters.setName("Audio Analysis");
        audioAnalysisParameters.add(fftDecayRate.set("fftDecayRate", 0.9, 0, 1));
        audioAnalysisParameters.add(centroidMaxBucket.set("centroidMax (mpx)", 0.35, 0, 1));
        audioAnalysisParameters.add(rmsMultiple.set("rmsMult (mpy)", 5, 0, 15));
        audioAnalysisParameters.add(mpxSmoothingFactor.set("mpxSmoothingFactor", 0.4, 0, 1));
        audioAnalysisParameters.add(mpySmoothingFactor.set("mpySmoothingFactor", 0.1, 0, 1));
        displayParameters.add(audioAnalysisParameters);

        // Drawing
        drawingParams.setName("Drawing");
        drawingParams.add(clearSpeed.set("clearSpeed", 50, 0, 255));
        drawingParams.add(particleAlpha.set("particleAlpha", 50, 0, 255));
        drawingParams.add(overallScale.set("overallScale", 1, 0.1, 3.0));
        drawingParams.add(saturationPct.set("saturationPct", 1, 0, 1));
        displayParameters.add(drawingParams);

        // Rotation / Interpolation speed
        speedParams.setName("Speed");
        speedParams.add(baseSpeed.set("baseSpeed", 0, -5, 10));
        speedParams.add(rmsSpeedMult.set("rmsSpeedMult", 30, 0, 100));
        displayParameters.add(speedParams);

        // Dot size
        dotParams.setName("Dots");
        dotParams.add(pointRadiusAudioScaleAmt.set("pointRadiusAudioScaleAmt", 1, 0, 1));
        dotParams.add(pointRadiusAudioScale.set("dotAudioScale", 10, 0, 50));
        dotParams.add(basePointRadius.set("baseDotRadius", 10, 0, 50));
        displayParameters.add(dotParams);

        // Line size
        lineParams.setName("Lines");
        lineParams.add(maxLineLength.set("maxLineLength", 100, 0, 3000));
        displayParameters.add(lineParams);

        // Audio effects
        audioEffectParams.setName("Audio Effects");
        audioEffectParams.add(motionId.set("motionId", 0, 0, NUM_MOTION_IDS));
        audioEffectParams.add(audioEffectSize1.set("effectSize1", 1, 0, 1));
        audioEffectParams.add(audioEffectSize2.set("effectSize2", 1, 0, 1));
        audioEffectParams.add(audioEffectSize3.set("effectSize3", 1, 0, 1));
        audioEffectParams.add(audioEffectSize4.set("effectSize4", 1, 0, 1));
        displayParameters.add(audioEffectParams);
        
        displayParameters.setName("Scene");
    };

    void copyParamsTo(DotsScene &dest) {
        copyParameters(this->displayParameters, dest.displayParameters);
    }

    void copyGenomeTo(DotsScene &dest) {
        dest.setupGenome(genome);
    }

    ~DotsScene() {
        free(genome);
    }
} DotsScene;

typedef struct DotsTrack {
    int trackId;

    float wanderPos = 0;
    float interpAmt;

    vector<DotsScene *> scenes;

    ofParameterGroup displayParameters;
    ofParameterGroup wanderParameters;
    ofParameter<float> wanderSpeed;

    int nGenomes;
    flam3_genome *genomes = NULL;

    void setupParams() {
        displayParameters.clear();
        wanderParameters.clear();

        displayParameters.setName("Track");
        wanderParameters.setName("Wander");
        wanderParameters.add(wanderSpeed.set("speed", 0.1, 0, 1));
        displayParameters.add(wanderParameters);
    };

    void copyParamsTo(DotsTrack &dest) {
        copyParameters(this->displayParameters, dest.displayParameters);
    }

    float getWanderSpeed() {
        return (exp(wanderSpeed) - 1.0f) / 100.0f;
    }

    void setGenomesFromScenes() {
        nGenomes = scenes.size();

        flam3_genome *newGenomes = (flam3_genome*)malloc(sizeof(flam3_genome) * (nGenomes + 1));
        memset(newGenomes, 0, sizeof(flam3_genome) * (nGenomes + 1));

        for (int i = 0; i < nGenomes; ++i) {
            flam3_copy(&newGenomes[i], scenes[i]->genome);
            newGenomes[i].time = i;
        }

        // Last element is so we loop properly
        flam3_copy(&newGenomes[nGenomes], scenes[0]->genome);
        newGenomes[nGenomes].time = nGenomes;

        if (genomes) free(genomes);
        genomes = newGenomes;
    }
} DotsTrack;

class StateManager {
public:
    void setup();
    void update();

    void setAudioRMS(float audioRMS);

    void flameWanderUpdate();
    void flameUpdate(float audioRMS);

    void loadGenebanks();
    void initrc(long sed);
    int randomi(int n);

    void loadGenome(flam3_genome *dest);
    void getGenomes(flam3_genome **d1, flam3_genome **d2, float &t);

    void killCurrent();
    void mateCurrent();
    void mutateCurrent();

    void midiToSceneParams(MidiController &midi);

    void genomeModified(int & genome);

    void loadAllParamsFromFile();
    void serializeCurrentTrackToFile();

    float getGenomeInterpolation();
    void applyParameterInterpolation(float t);

    void advanceTrack();
    void regressTrack();
    void saveTrack();
    void createTrack();
    DotsTrack& getTrack();

    void fadeSceneFwd();
    void fadeSceneRev();
    void regressScene();
    void advanceScene();
    void reloadScene();
    void deleteScene();
    void duplicateScene();
    DotsScene& getScene();

    DotsScene defaultScene;
    DotsScene activeScene;
    DotsScene *currScene;
    DotsScene *nextScene;

    DotsTrack activeTrack;

    // Are we wandering or standing still
    ofParameter<bool> wandering;
    int lastCP;
    flam3_genome wanderGenome;

    // Fading
    bool fading, wasFading;
    bool fadeForward;
    long fadeStartFrame;
    int fadeFrames;

    // Where are we
    int trackIdx, sceneIdx;
    vector<DotsTrack> tracks;

    // Geneback for mutation
    int ngenebank;
    flam3_genome *genebank;

    // Base control points
    int ncps;
    flam3_genome *cps;

    // Fixed randomness for flam3
    int seed;
    randctx rc;

    // For serialization
    ofxXmlSettings settings;
    int maxTrackId;

    // Events
    ofEvent<int> onTrackChange;
    ofEvent<int> onSceneChange;

    ofEvent<int> onTrackUpdate;
    ofEvent<int> onSceneUpdate;

protected:
    int nextTrackId() { return ++maxTrackId; };
};