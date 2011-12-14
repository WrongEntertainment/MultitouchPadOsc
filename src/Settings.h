/**
 * MagicTrackpadOsc is developed by Paul Vollmer (wrong-entertainment.com)
 * 
 * 
 * Copyright (c) 2011 Paul Vollmer
 *
 * MagicTrackpadOsc is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * MagicTrackpadOsc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General
 * Public License along with MagicTrackpadOsc; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 * 
 * @author      Paul Vollmer
 * @modified    2011.12.09
 * @version     0.1.1
 */



#pragma once

#include "ofxXmlSettings.h"





class Settings {
	
	public:
		/* Constructor */
		Settings();
	
	
		/* Methods */
		void init(string _filepath);
		void save();
	
	
		/* Variables */
		// XML
		string filepath;           // Path to settings XML file.
		
		// Application variables
		int      appFramerate;     // Application framerate
		int      appWindowx;       // Application window x-position
		int      appWindowy;       // Application window y-position
		int      appCount;         // Application counter
		
		// OSC variables
		int      oscOut;           // OSC active variable, if 0 = true, 1 = false
		string   oscHost;          // OSC host variable
		int      oscPort;          // OSC port variable
	
		// Trackpad variables
		string   padDevicename;    // Trackpad devicename
		int      padFrame;         // Trackpad frame
		int      padTimestamp;     // Trackpad timestamp
		int      padPosition;      // Trackpad finger x-, y-position
		int      padVelocity;      // Trackpad finger x-, y-velocity
		int      padSize;          // Trackpad finger size
		int      padMaxis;         // Trackpad finger minor-, major-axis
		int      padAngle;         // Trackpad finger angle
	
		
	
	
	
	private:
		// openFrameworks xml addon
		ofxXmlSettings   xmlSettings;
	
};