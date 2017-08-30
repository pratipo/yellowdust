#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	cout << "yellow dust BEAMER node\n";

	// setup rpi cam stream
	w=1920,h=1080;
	x=0,y=0;
	gst.setPipeline("tcpclientsrc host=yellowdustC.local port=5000 ! gdpdepay ! rtph264depay ! avdec_h264 ! videoconvert ", OF_PIXELS_RGB, true, w, h);
	gst.startPipeline();
	tex.allocate(w,h,GL_RGB);
	gst.play();

	// setup OSC
	sender.setup(HOST, SPORT);
	receiver.setup(RPORT);

	// setup serial
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	serial.setup(0, 9600);

	bpos = -1;
}

//--------------------------------------------------------------
void ofApp::update(){

	// REFRESH STREAM
	gst.update();
	if(gst.isFrameNew()) {
		tex.loadData(gst.getPixels());
	}

	// check INCOMING OSC
	while(receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(m);
		cout <<  "incoming OSC: " << m.getAddress() << endl;
		if (m.getAddress()=="/dust"){
			//serial.writeByte( (unsigned char)'b' ); //m.getArgAsInt32(0)
			for (int i=0; i<4; i++){
				dustdata[i] = m.getArgAsInt32(i);
				cout << dustdata[i] << " ";
			}
			cout << endl;
		}
	}

	if(serial.available()>0) {
		cout <<  "incoming serial" << endl;
		if (bpos>=0){
			sensorbuffer[bpos++] = (unsigned char)serial.readByte();
		}
		else if ((unsigned char)serial.readByte()==0) {
				bpos = 0;
		}
		if (bpos>3){ // ready to send dust readings

			int( (unsigned char)( (unsigned char)(dustbuffer[0]) << 8 | (unsigned char)(dustbuffer[1]) ) );
			int( (unsigned char)( (unsigned char)(dustbuffer[2]) << 8 | (unsigned char)(dustbuffer[3]) ) );

		}
	}

	// TRIGGERING LOGIC

	if ()
	ofxOscMessage m;
	m.setAddress("/light");
	m.addIntArg(lightstate);
	sender.sendMessage(m, false);
	cout <<  "send OSC" << endl;

	/*
	// INCOMING OSC
	if (receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);

		// unrecognized message: display on the bottom of the screen
		string msg_string;
		msg_string = m.getAddress();
		msg_string += ": ";
		for(int i = 0; i < m.getNumArgs(); i++){
			// get the argument type
			msg_string += m.getArgTypeName(i);
			msg_string += ":";
			// display the argument - make sure we get the right type
			if(m.getArgType(i) == OFXOSC_TYPE_INT32){
				msg_string += ofToString(m.getArgAsInt32(i));
			}
			else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
				msg_string += ofToString(m.getArgAsFloat(i));
			}
			else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
				msg_string += m.getArgAsString(i);
			}
			else{
				msg_string += "unknown";
			}
		}
		cout << msg_string << "\n" ;
	}
	*/
}

//--------------------------------------------------------------
void ofApp::draw(){
	// draw stream frame
	tex.draw(x,y,w,h);

	// draw graphics

	// text ...

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key=='o'){
		ofxOscMessage m;
		m.setAddress("/test");
		m.addIntArg(key);
		m.addFloatArg(ofGetElapsedTimef());
		sender.sendMessage(m, false);
	}

	// push data to thingspeak
	if (key=='d'){
		ofSystem("sudo curl -X GET https://api.thingspeak.com/update?api_key=UUBSOUB5TY6QIP3Y -d field2=" + ofToString(key));
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
