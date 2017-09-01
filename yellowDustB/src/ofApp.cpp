#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	cout << "yellow dust BEAMER node\n";
	// ofSetFrameRate(30);

	// setup OSC
	sender.setup(HOST, SPORT);
	receiver.setup(RPORT);

	// setup serial
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	serial.setup(0, 9600);

	prevByte = 0;
	inByte = 0;
	bpos = -1;

	// setup rpi cam stream
	w=1024,h=768;
	x=0,y=0;

	gst.setPipeline("tcpclientsrc host=yellowdustC.local port=5000 ! gdpdepay ! rtph264depay ! avdec_h264 ! videoconvert ", OF_PIXELS_RGB, true, w, h);
	gst.startPipeline();
	tex.allocate(w,h,GL_RGB);
	gst.play();

	c[0] = ofColor(230,255,0);
	c[1] = ofColor(230,255,0);
	c[2] = ofColor(255,230,0);
	c[3] = ofColor(255,230,0);

	dy[0] = ofGetHeight()-105*3 - 5;
	dy[1] = ofGetHeight()-105*2 - 5;
	dy[2] = ofGetHeight()-105*1 - 5;
	dy[3] = ofGetHeight()-105*0 - 5;

	for (int p = 0; p<4; p++){
		dust[p].setFilled(0);
		dust[p].setStrokeColor(c[p]);
		dust[p].setStrokeWidth(0.5);

		for (int i=0; i<ofGetWidth(); ++i){
			dps[p].push_back( 0.0 ); // start with zeros
		}
	}
	ofEnableAntiAliasing();

	lapse = 5000; // 30s to get some data ?
	systemON = false;
	startTime = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
void ofApp::update(){

	// REFRESH STREAM
	gst.update();
	if(gst.isFrameNew()) {
		tex.loadData(gst.getPixels();
	}

	// check INCOMING OSC
	if(receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(m);
		cout <<  "incoming OSC: " << m.getAddress() << endl;
		if (m.getAddress()=="/dust"){
			for (int i=0; i<4; i++){
				dustdata[i] = m.getArgAsInt32(i);
				cout << dustdata[i] << " ";
			}
			cout << endl;

			for (int p = 0; p<4; p++){
				rotate(dps[p].begin(), dps[p].begin()+1, dps[p].end()); // roll the path

				dps[p].at(ofGetWidth()-1) = -(dustdata[p]*0.0125) ; // update last values

				dust[p].clear();
				dust[p].moveTo(0, dps[p].front(), 0);
				for (int i=1; i<ofGetWidth(); ++i){
					dust[p].lineTo(i, dps[p].at(i), 0);
				}
			}
		}
	}

	if(serial.available()>=6) {
		cout <<  "incoming serial" << endl;
    int result = serial.readBytes( sensorbuffer, 6 );
    // check for error code
    if ( result == OF_SERIAL_ERROR )
    {
      cout << "unrecoverable error reading from serial" << endl;
    }
    else if ( result == OF_SERIAL_NO_DATA )
    {
      cout << "no data" << endl;
    }
		else if (sensorbuffer[0] == 255 && sensorbuffer[1] == 255){
			current = int( (unsigned char)( (unsigned char)(sensorbuffer[2]) << 8 | (unsigned char)(sensorbuffer[3]) ) );
			flow = int( (unsigned char)( (unsigned char)(sensorbuffer[4]) << 8 | (unsigned char)(sensorbuffer[5]) ) );
			cout << "current read: " << current << endl;
			cout << "flow read: " << flow << endl;
		}
	}

	// TRIGGER LOGIC!
  if(ofGetElapsedTimeMillis() - startTime >= lapse) {
		cout <<  "time! lapse: " << ofToString(lapse) << endl;
		if (systemON == false){ // TURN ON!
			cout <<  "ON" << endl;
			systemON = true;
			float dust_average = (dustdata[0] + dustdata[1] + 1)/2;
			dust_average = ofRandom(600.0); // EXHIBITION MODE
			// RELAY
			serial.drain();
			serial.writeByte(1);
			// LIGHTS
			ofxOscMessage m;
			m.setAddress("/light");

			if (dust_average > 400.0){
				m.addIntArg(2);
				lapse = 60000; // 60s
			}
			else if (dust_average > 200.0){
				m.addIntArg(2);
				lapse = 40000; // 40s
			}
			else{
				m.addIntArg(1);
				lapse = 20000; // 40s
			}
			sender.sendMessage(m, false); //cout <<  "send OSC" << endl;
		}
		else{ // TURN OFF
			cout <<  "OFF" << endl;
			systemON = false;
			// RELAY OFF
			serial.drain();
			serial.writeByte(0);
			// LIGHTS OFF
			ofxOscMessage m;
			m.setAddress("/light");
			m.addIntArg(0);
			sender.sendMessage(m, false);

			// load timer to get back ON
			if (lapse == 60000){
				lapse = 10000; // 10s OFF
			}
			else if (lapse == 40000){
				lapse = 30000; // 10s OFF
			}
			else if (lapse == 20000){
				lapse = 50000; // 10s OFF
			}
		}
		startTime = ofGetElapsedTimeMillis();
  }

}

//--------------------------------------------------------------
void ofApp::draw(){
	// draw stream frame
	tex.draw(x,y,w,h);

	ofPushStyle();
	// draw graphics
	for (int p = 0; p<4; p++){
		ofPushMatrix();
			ofTranslate(0, dy[p]);
			dust[p].draw();
		ofPopMatrix();
	}

	// text ...

	ofSetColor(0,0,0);
	ofDrawBitmapString("YELLOW DUST", 10,250);

	ofSetColor(255,255,255);
	ofDrawBitmapString("power " + ofToString(current), 10,ofGetHeight()/2-15);
	ofSetColor(255,255,255);
	ofDrawBitmapString("water " + ofToString(flow), 10,ofGetHeight()/2+15);

	ofSetColor(c[0]);
	ofDrawBitmapString("top sensors: " + ofToString(dustdata[0]) + " - " + ofToString(dustdata[1]) , 10, ofGetHeight()-200);
	ofSetColor(c[2]);
	ofDrawBitmapString("bottom sensors: " + ofToString(dustdata[2]) + " - " + ofToString(dustdata[3]) , 10, ofGetHeight()-95);

	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	ofxOscMessage m;
	m.setAddress("/light");
	if (key=='o'){
		m.addIntArg(1);
	}
	else if (key=='p'){
		m.addIntArg(2);
	}
	else{
		m.addIntArg(0);
	}
	sender.sendMessage(m, false);

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
