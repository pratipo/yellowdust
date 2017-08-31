#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define HOST "yellowdustB.local" //receiver address
//#define HOST "121.129.70.155"
#define RPORT 15001
#define SPORT 15000

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

		// graphics
		//ofGstVideoUtils gst;
		//ofTexture tex;
		//int w,h,x,y;

		// OSC
		ofxOscReceiver receiver;
		ofxOscSender sender;

		// SERIAL
		unsigned char prevByte;
		unsigned char inByte;
		int			bpos;
		unsigned char dustbuffer[8];			// a string needs a null terminator, so we need 3 + 1 bytes
		ofSerial serial;
};
