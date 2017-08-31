#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	cout << "yellow dust CAMERA node\n";
	ofSetLogLevel(OF_LOG_VERBOSE);

	//OSC setup
	sender.setup(HOST, SPORT);
	receiver.setup(RPORT);

	prevByte = (unsigned char) 0;
	inByte =  (unsigned char) 0;

	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	serial.setup(0, 9600); // "ttyUSB0"

	bpos = -1;
}

//--------------------------------------------------------------
void ofApp::update(){

	// check INCOMING OSC
	while(receiver.hasWaitingMessages()){
		cout <<  "incoming OSC" << endl;

		ofxOscMessage m;
		receiver.getNextMessage(m);
		if (m.getAddress()=="/light"){
			serial.writeByte( (unsigned char)m.getArgAsInt32(0) );
			cout <<  "/light : " << (char)m.getArgAsInt32(0) << endl;
		}
	}

	if(serial.available()>0) {
		cout <<  "incoming serial" << endl;

		prevByte = inByte;
		inByte = (unsigned char)serial.readByte();
		if (prevByte == 255 && inByte == 255)
			bpos = 0;
		else if (bpos == -1 && inByte == 255)
			bpos = -1;
		else
			dustbuffer[bpos++] = inByte;

		if (bpos==8){
			ofxOscMessage m;
			m.setAddress("/dust");
			m.addIntArg(int( (unsigned char)(dustbuffer[0]) << 8 | (unsigned char)(dustbuffer[1]) ));
			m.addIntArg(int( (unsigned char)(dustbuffer[2]) << 8 | (unsigned char)(dustbuffer[3]) ));
			m.addIntArg(int( (unsigned char)(dustbuffer[4]) << 8 | (unsigned char)(dustbuffer[5]) ));
			m.addIntArg(int( (unsigned char)(dustbuffer[6]) << 8 | (unsigned char)(dustbuffer[7]) ));
			sender.sendMessage(m, false);
			cout <<  "send OSC" << endl;
			bpos = -1;
		}

	}
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key == 'a'){
		serial.writeByte( 0 );
	}
	else if (key == 'b'){
		serial.writeByte( 1 );
	}
	else{
		ofxOscMessage m;
		m.setAddress("/test");
		m.addIntArg(key);
		sender.sendMessage(m, false);
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
