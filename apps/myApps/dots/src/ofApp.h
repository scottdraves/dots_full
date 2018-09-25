#pragma once

#include "ofMain.h"
#include "ofxFft.h"
#include "fmod.h"
#include "GuiApp.h"
#include "appConstants.h"
#include "ofxSyphon.h"
#include "StateManager.h"

extern "C" {
#include "flam3.h"
}

typedef struct pointPair {
    int idx;
    ofVec3f pt1, pt2;
    float ptSize, lineWidth;
    ofFloatColor color;
    float cpPixelsPerUnit;
    ofVec2f cpCenter;
} pointPair;

typedef struct flameSeq {
    long frameCreated, frameUpdated;
    unsigned short *xform_distribution;
} flameSeq;

class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void audioIn(float * input, int bufferSize, int nChannels);

    void guiUpdate();

    void setFlameParameters(flam3_genome &renderCp, flam3_genome &cp, const int motionId);
    void renderFlame(flam3_genome *toRender, double *flameSamples, const int nSamples,
                     randctx *rc, unsigned short *xform_distribution, const int fuse = 20);

    void flameDraw();
    void flameWander();
    void flameRotate();

    void handleKey(int key);

    void handleTrackChanged(int & trackIdx);
    void handleSceneChanged(int & sceneIdx);
    void handleWanderingChanged(bool & wandering);

    // After interpolation and to render
    flam3_genome *cp, renderCp;
    int nsamples;
    double *prevFlameSamples, *currFlameSamples;

    // Interpolate between flam3 cps smoothly during wander
    int frame, swapFrame;
    vector<flameSeq> flameSequences;

    // Transition between non-wander cps
    flam3_genome *cp2, renderCp2;
    unsigned short *static_xform_distribution1, *static_xform_distribution2;

    // Dot shader
    ofShader billboardShader;

    // To draw
    ofVboMesh lines;
    vector<pointPair> pointPairs;
    float *pointRadii, *lineWidths, *cpPixelsPerUnit;
    ofVec2f *cpCenter;
    double totDotPixels, totLinePixels;

    // Display Paremters
    int fullscreen;
    float mpx, mpy, mpxSmoothingFactor, mpySmoothingFactor;
    float audioEffectSize1, audioEffectSize2, audioEffectSize3, audioEffectSize4;
    float mmpx, mmpy;
    float baseSpeed, rmsSpeedMult;
    float pointRadiusAudioScaleAmt;
    float frameClearSpeed;
    float particleAlpha, basePointRadius, pointRadiusAudioScale;
    float maxLineLength;
    float saturationPct;
    float overallScale;

    // Audio mode
    int audioMode;
    ofSoundPlayer mySound;
    int soundStreamDevice, nChannels;
    ofSoundStream soundStream;  // for input

    // Audio analysis
    int nFftBuckets;
    float *audioInput;
    float *fftOutput;
    float audioRMS, smoothedAudioRMS, rmsMultiple;
    float audioCentroid, centroidMaxBucket;
    ofxFft *fft;
    float fftDecayRate;

    // The GUI window
    shared_ptr<GuiApp> gui;
    ofFbo visualsFbo;
    shared_ptr<ofTexture> visuals;
    ofxSyphonServer syphonServer;

    // State manager
    shared_ptr<StateManager> stateManager;

    queue<int> keyPresses;
    bool cmdKeyDown;
};

