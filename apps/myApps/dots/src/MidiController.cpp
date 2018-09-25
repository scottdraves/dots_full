//
//  MidiController.cpp
//  dotsLocal
//
//  Created by Christopher Anderson on 4/5/16.
//
//

#include "MidiController.h"

MidiController::~MidiController() {
    midiIn.closePort();
    midiIn.removeListener(this);
}

void MidiController::setup() {
    vector<string> &ports = midiIn.getPortList();

    // Auto select nanoKontrol if available
    int port = 0;
    for (int i = 0; i < ports.size(); ++i) {
        if (ports[i].find("nanoKONTROL2") != string::npos) {
            port = i;
            break;
        }
    }
    midiIn.openPort(port);

    midiIn.addListener(this);

    for (int i = 0; i < 8; ++i) {
        sliders[i] = -1;
        knobs[i] = -1;
        buttons[i] = -1;
    }
}

void MidiController::newMidiMessage(ofxMidiMessage& msg) {
    if (msg.control >= 0 && msg.control < 8) {
        sliders[msg.control] = (float)msg.value/127.0;
    } else if (msg.control >= 16 && msg.control < 23) {
        knobs[msg.control-16] = (float)msg.value/127.0;
    }

    int buttonIdx = -1, buttonBit = -1;
    if (msg.control >= 32 && msg.control < 40) {
        buttonIdx = msg.control - 32;
        buttonBit = 0;
    } else if (msg.control >= 48 && msg.control < 56) {
        buttonIdx = msg.control - 48;
        buttonBit = 1;
    } else if (msg.control >= 64 && msg.control < 72) {
        buttonIdx = msg.control - 64;
        buttonBit = 2;
    }

    if (buttonIdx >= 0 && buttonBit >= 0) {
        if (msg.value == 127) {
            buttons[buttonIdx] |= 1 << buttonBit;
        } else {
            buttons[buttonIdx] &= ~(1 << buttonBit);
        }
    }
}
