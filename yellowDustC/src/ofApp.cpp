#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	cout << "yellow dust CAMERA node\n";
	ofSetLogLevel(OF_LOG_VERBOSE);

	//OSC setup
	sender.setup(HOST, SPORT);
	receiver.setup(RPORT);

	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	serial.setup("ttyUSB0", 9600);

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

		if (bpos>=0){
			dustbuffer[bpos++] = (unsigned char)serial.readByte();
		}
		else{
			if ((unsigned char)serial.readByte()==0)
				bpos = 0;
		}
		if (bpos>7){ // ready to send dust readings
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
