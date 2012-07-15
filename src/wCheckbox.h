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

#include "ofMain.h"
#include "wInteraction.h"





class wCheckbox {
	
	public:
		/* Constructor */
		wCheckbox();
	
		/* Methods */
		void init(ofTrueTypeFont &_f, string _message, int _x, int _y, int _status);
		void init(ofTrueTypeFont &_f, string _message, int _x, int _y);
		void display();
		void pressed(int _mx, int _my);
	
		/* Variables */
		int status;
	
	
	private:
		ofTrueTypeFont font;
		
		wInteraction interaction;

		/* Variables */
		int    x;
		int    y;
		int    size;
		string message;
	
};