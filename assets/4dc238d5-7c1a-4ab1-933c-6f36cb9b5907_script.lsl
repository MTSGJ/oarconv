///////////////////////////////////////////////////////////////////////////////////
///////
///////
///////
///////            screen display
///////            initially created by Anthony Bundy, i3D Inc.
///////             
///////             Date Jan 12, 2008
///////             
///////             This script listens on a certain channel for a message
///////             SETTEXTURE::(key) where (key) is the texture key to show on its display face
///////             
///////             Copyright (C) <2008>  <i3D Inc.>
///////             Contact: team@i3dnow.com
///////             
///////                 This program is free software: you can redistribute it and/or modify
///////                 it under the terms of the GNU General Public License as published by
///////                 the Free Software Foundation, either version 3 of the License, or
///////                 (at your option) any later version.
///////             
///////                 This program is distributed in the hope that it will be useful,
///////                 but WITHOUT ANY WARRANTY; without even the implied warranty of
///////                 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///////                 GNU General Public License for more details.
///////             
///////                 You should have received a copy of the GNU General Public License
///////                 along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////                 
///////                 Contact: team@i3dnow.com
///////                 
///////              
//////////////////////////////////////////////////////////////////////////////////////    

// Inputs

// Output

// Chat Channels used
    integer debugChannel = DEBUG_CHANNEL;

    integer chatOffset = -225847536;        // chat channel the display should listen to.
    integer chat2Display;

// Link Channels used
    integer link2All = -4873920;

// Global declarations
    integer debug = 0;                // for debugging
    integer myListener;
    list myMessage;
    integer currentChannel = 0;
    integer channelButtonLinkNum;
    key channelButtonTexture = "587e7e4f-0485-72cc-809a-ccc40e88aded";

    key slide; // texture key for an X indicating there is no slide for that spot

    integer displayFace = 2;
    // integer displayFace = 2;

///////////////////////////////////////////////////////////////////////////////////////
//STATE STATE STATE STATE STATE STATE STATE STATE STATE STATE STATE STATE STATE STATE//
///////////////////////////////////////////////////////////////////////////////////////
//                                                                                   //
//                                                                                   //
//                          DEFAULT STATE                                            //
//                                                                                   //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
default 
{
    state_entry() 
    {
        chat2Display = chatOffset+currentChannel;
        llListenRemove(myListener);
        myListener = llListen(chat2Display, "", NULL_KEY, "");
    }
    listen(integer channel, string name, key id, string message) 
    {
        if (llSubStringIndex(message, "SETTEXTURE::") > -1) 
        {
            // message is to set the texture key, -1 is for 0 based indexing on the characters
            slide = (key)llDeleteSubString(message, 0, llStringLength("SETTEXTURE::") - 1);
            if(debug > 0) llSay(DEBUG_CHANNEL,"slide: " + (string)slide);
            if (slide) 
            {
                // slide is a valid key
                llSetTexture(slide, displayFace);
            }
        }
    }
    touch_start(integer total_number)
    {
        if ( llDetectedGroup(0) ) // agent is in same active group as this object
        {
            if (llGetLinkName( llDetectedLinkNumber(0) ) == "channel") 
            {
                channelButtonLinkNum = llDetectedLinkNumber(0);
                currentChannel = (++currentChannel)%4;
                chat2Display = chatOffset+currentChannel;
                
                // set the channel texture offset
                llSetLinkPrimitiveParams(channelButtonLinkNum, [PRIM_TEXTURE, displayFace,
                    channelButtonTexture, <1.0,0.25,0.0>,<0.0, .375 - (.25*currentChannel) ,0.0>,0.0]);
                llListenRemove(myListener);
                myListener = llListen(chat2Display, "", NULL_KEY, "");
                // llWhisper(0,"Channel set to " + (string)chat2Display);
            }
        }
        
    }
    link_message(integer sender_number, integer number, string message, key id) 
    {
        if (number == link2All) 
        {
            myMessage = llParseString2List(message, ["::"], [""]);
            if (llList2String(myMessage, 0) == "channel") 
            {
                currentChannel = (integer)llList2String(myMessage, 1);
                chat2Display = chatOffset+currentChannel;
                llListenRemove(myListener);
                myListener = llListen(chat2Display, "", NULL_KEY, "");
                
            }
            
            myMessage=[];
            
        }
    }
    
}
