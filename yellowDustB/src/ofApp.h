#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define HOST "yellowdustC.local" //receiver address
//#define HOST "pratipoT420" //receiver address
#define RPORT 15000
#define SPORT 15001

class ofApp : public ofBaseApp{
	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		bool systemON;
		uint64_t startTime;
		uint64_t lapse;

		// graphs
		ofGstVideoUtils gst;
		ofTexture tex;
		int w,h,x,y;

		ofColor c[4];  // color of each dust graph
		int dy[4]; // y offset of each dust graph

		// OSC
		ofxOscReceiver receiver;
		ofxOscSender sender;

		// SERIAL
		unsigned char prevByte;
		unsigned char inByte;
		int			bpos;
		unsigned char sensorbuffer[6];			// a string needs a null terminator, so we need 3 + 1 bytes
		ofSerial serial;

		// data structures and logic
		ofPath dust[4]; // dust graph
		vector<float> dps[4]; // dust graph data

		int current;
		int flow;
		int dustdata[4];			// a string needs a null terminator, so we need 3 + 1 bytes
};
