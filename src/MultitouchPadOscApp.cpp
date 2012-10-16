//
// MultitouchPadOscApp.cpp
// MultitouchPadOsc is released under the MIT License.
//
// Copyright (c) 2011 - 2012, Paul Vollmer http://www.wrong-entertainment.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "MultitouchPadOscApp.h"



void MultitouchPadOscApp::setup() {
	/* Load the font
	 */
	//ofTrueTypeFont::setGlobalDpi(96);
	vera.loadFont(ofFilePath::getCurrentWorkingDirectory() + "/Font/Vera.ttf", 9, true, false);
	if (vera.isLoaded()) {
		ofLog() << "FONT: Is loaded";
	} else {
		ofLog() << "FONT: Not loaded";
	}
	
	
	/* Multitouch Trackpad
	 * Add the listeners
	 */
    ofAddListener(pad.update, this, &MultitouchPadOscApp::padUpdates);
    ofAddListener(pad.touchAdded, this, &MultitouchPadOscApp::newTouch);
    ofAddListener(pad.touchRemoved, this, &MultitouchPadOscApp::removedTouch);
	
	
	/* Viewer
	 */
	toolbarMVC.init();
	settingsMVC.init(vera);
	consoleMVC.init();
	
	
	/* Set the xml tag names and root-version, -url attributes.
	 */
	XML.changeSyntax(XML.ROOT, "multitouchPadOscApp");
	XML.changeSyntax(XML.ROOT_VERSION, PROJECTVERSION);
	XML.changeSyntax(XML.ROOT_URL, "http://www.wng.cc");
	XML.changeSyntax(XML.CORE, "appCore");
	
	/* Set the default Settings parameter.
	 */
	XML.defaultSettings.frameRate = 60;
	XML.defaultSettings.windowX = 100;
	XML.defaultSettings.windowY = 100;
	XML.defaultSettings.windowWidth = 500;
	XML.defaultSettings.windowHeight = 400;
	XML.defaultSettings.windowTitle = PROJECTNAME;
	XML.defaultSettings.cursor = false;
	XML.defaultSettings.fullscreen = false;
	XML.defaultSettings.escapeQuitsApp = true;
	XML.defaultSettings.log = true;
	
	/* ofxXmlDefaultSettings
	 * Load the xml file from default path.
	 */
	XML.load();
	
	/* Add custom settings to the xml default file.
	 * To read/write content from the root directory, you can use
	 * the pushRoot/popRoot methods.
	 */
	XML.pushRoot();
	if (XML.fileExist) {
		toolbarMVC.getXml(XML);
		touchpointsMVC.getXml(XML);
		settingsMVC.getXml(XML);
	}
	/* If no xml file exist, create the xml tag/attributes and add some parameter.
	 */
	else {
		toolbarMVC.addXml(XML);
		touchpointsMVC.addXml(XML);
		settingsMVC.addXml(XML);
	}
	XML.popRoot();
	
	/* Set the openFrameworks app settings.
	 */
	XML.setSettings();
	ofLog() << "XML: " << XML.getStatusMessage();
	setWindowTitle();
	
	/* Save the XML file if no file existed
	 */
	if (XML.fileExist == false) {
		XML.saveFile();
	}
	
	/* Log the MVC parameter
	 */
	toolbarMVC.log();
	touchpointsMVC.log();
	settingsMVC.log();
	
	/* OSC
	 * Open an outgoing connection to oscHost, oscPort
	 */
	oscSender.setup(settingsMVC.oscHost, settingsMVC.oscPort);
	ofLog() << "OSC: setup host \"" << settingsMVC.oscHost << "\", port " << "\"" << settingsMVC.oscPort << "\"";
	
	/* GUI
	 * set the status to osc:out settings value
	 */
	gui = new ofxUICanvas();
	gui->setFont(ofFilePath::getCurrentWorkingDirectory() + "/Font/Vera.ttf"); //This loads a new font and sets the GUI font
    gui->setFontSize(OFX_UI_FONT_LARGE, 12);                                   //These call are optional, but if you want to resize the LARGE, MEDIUM, and SMALL fonts, here is how to do it. 
    gui->setFontSize(OFX_UI_FONT_MEDIUM, 8);           
    gui->setFontSize(OFX_UI_FONT_SMALL, 6);                                    //SUPER IMPORTANT NOTE: CALL THESE FUNTIONS BEFORE ADDING ANY WIDGETS, THIS AFFECTS THE SPACING OF THE GUI
	gui->addWidget(new ofxUITextInput("TEXT HOST", settingsMVC.oscHost, 130,20, 50,77, OFX_UI_FONT_SMALL));
	gui->addWidget(new ofxUITextInput("TEXT PORT", ofToString(settingsMVC.oscPort), 130,20, 50,97, OFX_UI_FONT_SMALL));
	gui->addWidget(new ofxUITextInput("TEXT DEVICENAME", ofToString(settingsMVC.oscTouchpadDevicename), 150, 20, 100,113, OFX_UI_FONT_SMALL));
	ofAddListener(gui->newGUIEvent, this, &MultitouchPadOscApp::guiEvent);
	gui->setVisible(false);
	
	
	ofLog() << "Number of Devices: "<< pad.getNumDevices();
	ofLog() << "SETUP: ready";
}



void MultitouchPadOscApp::update() {
	
}



void MultitouchPadOscApp::draw(){
	/* background
	 */
	ofBackground(111, 112, 114);
	ofFill();
	ofSetColor(88, 88, 90);
	ofRect(10, 30, ofGetWidth()-20, ofGetHeight()-40);
	
	/* The Window Mode button display method isn't  includet at the Toolbar MVC.
	 * Because we trigger on/off the MVC, the button must be draw seperate.
	 */
	ofSetColor(255);
	ofFill();
	toolbarMVC.buttonWindowMode.display();
	if (toolbarMVC.buttonWindowMode.status == false) {
		/* MVC draw methods
		 */
		toolbarMVC.draw(vera, oscIsSending);
		touchpointsMVC.draw(vera, pad);
		
		/* Settings Viewer
		 */
		
		if(toolbarMVC.buttonSettings.status == true) {
			settingsMVC.draw(vera);
		}
		
		/* Console button
		 */
		if(toolbarMVC.buttonConsole.status == true) {
			consoleMVC.draw(vera);
		}
	}
	
	/* Reset oscIsSending variable to false
	 */
	oscIsSending = false;
}



void MultitouchPadOscApp::exit() {
	/* Set the XML file
	 */
	settingsMVC.setXml(XML);
	toolbarMVC.setXml(XML);
	
	/* Save the current settings to xml.
	 */
	XML.save();
	
	/* GUI
	 */
	delete gui;
}



void MultitouchPadOscApp::keyPressed(int key) {
	/* MVC key pressed events
	 */
	toolbarMVC.keyPressed(key);
	settingsMVC.keyPressed(key);
	consoleMVC.keyPressed(key);
}



void MultitouchPadOscApp::keyReleased(int key) {
}



void MultitouchPadOscApp::mouseMoved(int x, int y) {
}



void MultitouchPadOscApp::mouseDragged(int x, int y, int button) {
}



void MultitouchPadOscApp::mousePressed(int x, int y, int button) {
	
	toolbarMVC.mousePressed(x, y);
	
	/* hide / show GUI textfield
	 */
	if (toolbarMVC.buttonTouchpoints.status == true) {
		gui->setVisible(false);
	}
	
	if (toolbarMVC.buttonSettings.status == true) {
		settingsMVC.mousePressed(x, y);
		gui->setVisible(true);
	}
	
	if (toolbarMVC.buttonConsole.status == true) {
		gui->setVisible(false);
	}
}



void MultitouchPadOscApp::mouseReleased(int x, int y, int button) {
}



void MultitouchPadOscApp::windowResized(int w, int h) {
	toolbarMVC.windowResized(w, h);
}



void MultitouchPadOscApp::gotMessage(ofMessage msg) {
}



void MultitouchPadOscApp::dragEvent(ofDragInfo dragInfo) {
}



void MultitouchPadOscApp::padUpdates(int & t) {
	printf("pad updates & has %i touches\n",t);
	
	
	for(int i=0; i<pad.getTouchCount(); i++) {
		// get a single touch as MTouch struct....
		MTouch touch;
		if(!pad.getTouchAt(i,&touch)) {
			continue; // guard..
		}

		
		/* OSC
		 */
		if(toolbarMVC.buttonOscActive.status == true) {
			
			
			// Check if padFrame is active
			if (settingsMVC.checkboxFrame.status == true) {
				// Send osc message, integer value with the current frame.
				// e.g. /mt/1/frame/23
				string sFrame = ofToString("/") + ofToString(settingsMVC.oscTouchpadDevicename) + ofToString("/") + ofToString(i) + "/frame";
				
				oscIntMessage(sFrame, touch.frame);
				
				consoleMVC.addString(ofToString("OSC ") + ofToString(sFrame) + ofToString("/") + ofToString(touch.frame));
				
				oscIsSending = true;
			}
			
			// check if padTimestamp is active
			/*if (settings.padTimestamp == 0) {
			 Finger finger;
			 cout << "padTimestamp " << finger.timestamp << endl;
			 }*/
			
			// check if padPosition is active
			if (settingsMVC.checkboxPosition.status == true) {
				// Send osc message, float value between 0 and 1.
				// e.g. /mt/1/x/0.5
				string sX = ofToString("/") + ofToString(settingsMVC.oscTouchpadDevicename) + ofToString("/") + ofToString(i) + "/x";
				string sY = ofToString("/") + ofToString(settingsMVC.oscTouchpadDevicename) + ofToString("/") + ofToString(i) + "/y";
				oscFloatMessage(sX, touch.x);
				oscFloatMessage(sY, touch.y);
				consoleMVC.addString(ofToString("OSC ") + ofToString(sX) + ofToString("/") + ofToString(touch.x));
				consoleMVC.addString(ofToString("OSC ") + ofToString(sY) + ofToString("/") + ofToString(touch.y));
				
				oscIsSending = true;
			}
			
			// check if padVelocity is active
			/*if (settings.padVelocity == 0) {
			 Finger finger;
			 cout << "padVelocity " << finger.mm.vel.x << endl;
			 }*/
			
			// check if padSize is active
			if (settingsMVC.checkboxSize.status == true) {
				// Send osc message, float value between 0 and 1.
				// e.g. /mt/1/size/0.5
				string sSize = ofToString("/") + ofToString(settingsMVC.oscTouchpadDevicename) + ofToString("/") + ofToString(i) + "/size";
				oscFloatMessage(sSize, touch.size);
				consoleMVC.addString(ofToString("OSC ") + ofToString(sSize) + ofToString("/") + ofToString(touch.size));
				
				oscIsSending = true;
			}
			
			// check if padAngle is active
			if (settingsMVC.checkboxAngle.status == true) {
				// Send osc message, float value between 0 and 1.
				// e.g. /mt/1/angle/0.5
				string sAngle = ofToString("/") + ofToString(settingsMVC.oscTouchpadDevicename) + ofToString("/") + ofToString(i) + "/angle";
				oscFloatMessage(sAngle, touch.angle);
				consoleMVC.addString(ofToString("OSC ") + ofToString(sAngle) + ofToString("/") + ofToString(touch.angle));
				
				oscIsSending = true;
			}
		}
	} // End for
		
}



void MultitouchPadOscApp::newTouch(int & n) {
    //printf("+ a new touch\n", n);
	
	/* Send an osc message if the touchpoint is added.
	 */
	ofxOscMessage m;
	m.setAddress("/" + settingsMVC.oscTouchpadDevicename + "/" + ofToString(n) + "/added");
	oscSender.sendMessage(m);
}



void MultitouchPadOscApp::removedTouch(int & r) {
    //printf("- a removed touch\n",r);
	
	/* Send an osc message if the touchpoint is removed.
	 */
	ofxOscMessage m;
	m.setAddress("/" + settingsMVC.oscTouchpadDevicename + "/" + ofToString(r) + "/removed");
	oscSender.sendMessage(m);
}



void MultitouchPadOscApp::guiEvent(ofxUIEventArgs &e) {
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	
	if (name == "TEXT HOST") {
		ofxUITextInput *textInput = (ofxUITextInput *) e.widget;
		settingsMVC.oscHost = textInput->getTextString();
		oscSender.setup(settingsMVC.oscHost, settingsMVC.oscPort);
		setWindowTitle();
		consoleMVC.addString("Change host to " + textInput->getTextString(), true);
	}
	else if (name == "TEXT PORT") {
		ofxUITextInput *textInput = (ofxUITextInput *) e.widget;
		settingsMVC.oscPort = ofToInt(textInput->getTextString());
		oscSender.setup(settingsMVC.oscHost, settingsMVC.oscPort);
		setWindowTitle();
		consoleMVC.addString("Change port to " + textInput->getTextString(), true);
	}
	else if (name == "TEXT DEVICENAME") {
		ofxUITextInput *textInput = (ofxUITextInput *) e.widget;
		settingsMVC.oscTouchpadDevicename = textInput->getTextString();
		consoleMVC.addString("Change devicename to " + textInput->getTextString(), true);
	}
}



void MultitouchPadOscApp::setWindowTitle(){
	ofSetWindowTitle(ofToString(PROJECTNAME) + " - Host: " + settingsMVC.oscHost + " Port: " + ofToString(settingsMVC.oscPort));
}



void MultitouchPadOscApp::oscIntMessage(string s, int val) {
	ofxOscMessage m;
	m.setAddress(s);
	m.addIntArg(val);
	oscSender.sendMessage(m);
}



void MultitouchPadOscApp::oscFloatMessage(string s, float val) {
	ofxOscMessage m;
	m.setAddress(s);
	m.addFloatArg(val);
	oscSender.sendMessage(m);
}
