// GNU GPL Licence - Original modified slightly to show blank screen on preview
// as well as currently showing slide on Laptop screen
// 2-Aug-2011, Austin Tate, AIAI, University of Edinburgh

///////////////////////////////////////////////////////////////////////////////////
///////
///////            presentation
///////            initially created by Anthony Bundy, i3D Inc.
///////             
///////             Date Jan 12, 2008
///////             
///////             This script shows texture slides based on the alphabetical order
///////             they appear in inventory. It is meant that the current slide is 
///////             shown on a detached screen with a listener and the previous and
///////             next slides will be displayed locally.
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
//////////////////////////////////////////////////////////////////////////////////////    

// Inputs

// Output

// Chat Channels used
    integer debugChannel = DEBUG_CHANNEL;

    integer chatOffset = -225847536; // chat channel the display should listen to.
    integer chat2Display;

// Link Channels used
    integer link2All = -4873920;

// Global declarations
    integer debug = 0;               // for debugging
    list myMessage;
    integer currentChannel = 0;
    integer powerState;
    integer videoState;
    
    integer currentSlide;            // current slide index
    integer currDisplayLinkNum;      // link number that displays the current texture slide
    integer nextDisplayLinkNum;      // link number that displays the next texture slide
    integer powerButtonLinkNum;      // link number of the power button
    integer videoButtonLinkNum;      //link number of the video button
    integer channelButtonLinkNum;
      
    // Original SL Textures
    // key noSlide = "c38bab1a-6304-f8eb-f6e3-42c96a454f7c"; // texture key for an X indicating there is no slide for that spot
    // key videoButtonTexture = "4e3a538e-8287-e224-b1d5-28a95fdaa4ab";    // texture key Video/Slide selection
    // key powerButtonTexture = "87694dec-38a9-a6bc-d2ec-287259cc6d72";    // texture key Power On/Off
    // key channelButtonTexture = "587e7e4f-0485-72cc-809a-ccc40e88aded";  // texture key for channel selections
    
    // Texture UUIDs for your own grid
    key noSlide = "794e7aded-b1c8-4201-9fac-6fcde6b10116"; // texture key for an X indicating there is no slide for that spot
    key videoButtonTexture = "ccd17310-05cd-430d-ae37-e95dc3d396ba";   // texture key Video/Slide selection
    key powerButtonTexture = "3707c965-1155-46a5-8722-163287c9b90f";   // texture key Power On/Off
    key channelButtonTexture = "a3b60eff-99d4-404e-a1fa-672e6c944bae"; // texture key for channel selections  

    key mediaTexture;
    
    integer displayFace = 2;        // display face 2 for some linked objects
    //integer face5 = 5;            // display face 5 for some linked objects

// Global functions
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////
//////////      Function:   updateDisplays
//////////
//////////      Input:      
//////////                    
//////////      Output:     
//////////                    
//////////      Purpose:    This function updates all the displays that can show the various textures
//////////                    
//////////                    
//////////                    
//////////      Issues:        
//////////                                      
//////////////////////////////////////////////////////////////////////////////////////////////////
updateDisplays()
{
    if (debug > 0) llSay(DEBUG_CHANNEL, "currentSlide = " + (string)currentSlide + " : "+
        llGetInventoryName(INVENTORY_TEXTURE, currentSlide) + 
        " key= " + (string)llGetInventoryKey( llGetInventoryName(INVENTORY_TEXTURE, currentSlide) ));
    if (llGetInventoryNumber(INVENTORY_TEXTURE) > 0) 
    {
        // there are slides in inventory
        // set main display to current slide
        llSay( chat2Display, "SETTEXTURE::"+
            (string)llGetInventoryKey( llGetInventoryName(INVENTORY_TEXTURE, currentSlide) ) );
        llSetLinkTexture(currDisplayLinkNum, 
            llGetInventoryKey( llGetInventoryName(INVENTORY_TEXTURE, currentSlide) ), 
            displayFace);
        
        // set next display
        if (currentSlide < llGetInventoryNumber(INVENTORY_TEXTURE) - 1) 
        {
            // we are not at the end
            // set the next display to the next texture
            if (llGetInventoryName(INVENTORY_TEXTURE, currentSlide + 1) == "logo")
            {
                if (currentSlide < llGetInventoryNumber(INVENTORY_TEXTURE) - 2) 
                {
                    llSetLinkTexture(nextDisplayLinkNum, 
                        llGetInventoryKey( llGetInventoryName(INVENTORY_TEXTURE, currentSlide + 2) ), 
                        displayFace);
                }else
                {
                    // we are actually on last slide (because all that's left is the logo.
                    llSetLinkTexture(nextDisplayLinkNum, noSlide, displayFace);
                }
                
            }else
            {
                llSetLinkTexture(nextDisplayLinkNum, 
                    llGetInventoryKey( llGetInventoryName(INVENTORY_TEXTURE, currentSlide + 1) ), 
                    displayFace);
            }
            
        }else
        {
            // We are at the end
            llSetLinkTexture(nextDisplayLinkNum, noSlide, displayFace);
        }
    }

}
power(integer pwrState)
{
    if (pwrState) 
    {
        powerState = TRUE;
        // Set power button texture to showpower on
        llSetLinkPrimitiveParams(powerButtonLinkNum, [PRIM_TEXTURE, displayFace,
            powerButtonTexture, <1.0,0.5,0.0>,<0.0,-0.25,0.0>,0.0]);
    }else
    {
        powerState = FALSE;
        llSetLinkPrimitiveParams(powerButtonLinkNum, [PRIM_TEXTURE, displayFace,
            powerButtonTexture, <1.0,0.5,0.0>,<0.0,0.25,0.0>,0.0]);
    }
    
}
video(integer vidState)
{
    if(debug > 0) llSay(debugChannel, "change video state " + (string)videoState);
    if (vidState) 
    {
        videoState = TRUE;
        // Set video button texture to show video on
        llSetLinkPrimitiveParams(videoButtonLinkNum, [PRIM_TEXTURE, displayFace,
            videoButtonTexture, <1.0,0.5,0.0>,<0.0,-0.25,0.0>,0.0]);
    }else
    {
        videoState = FALSE;
        llSetLinkPrimitiveParams(videoButtonLinkNum, [PRIM_TEXTURE, displayFace,
            videoButtonTexture, <1.0,0.5,0.0>,<0.0,0.25,0.0>,0.0]);
    }
}


init()
{
    // All the initialization items
    
    chat2Display = chatOffset+currentChannel;
    
    currentSlide = 0;
    
    currDisplayLinkNum = 0;
    nextDisplayLinkNum = 0;
    powerButtonLinkNum = 0;
    videoButtonLinkNum = 0;
        
    integer totalLinked = llGetNumberOfPrims();
    integer idx;
    if (totalLinked > 1) 
    {
        // at least some laptop are linked together
        // Find the link numbers of the items that will show the slides on the 'laptop' screen.
        for (idx=1; idx < totalLinked; ++idx)
        {
            if (llToLower( llGetLinkName(idx) ) == "currdisplay") 
            {
                currDisplayLinkNum = idx;
            }else
            if (llToLower( llGetLinkName(idx) ) == "nextdisplay") 
            {
                nextDisplayLinkNum = idx;
            }else
            if (llToLower( llGetLinkName(idx) ) == "power")
            {
                powerButtonLinkNum = idx;
            }else
            if (llToLower( llGetLinkName(idx) ) == "video")
            {
                videoButtonLinkNum = idx;
            }else
            if (llToLower( llGetLinkName(idx) ) == "channel")
            {
                channelButtonLinkNum = idx;
            }
        }
    }
    
    // check if we found all the displays
    if (currDisplayLinkNum < 1 || nextDisplayLinkNum < 1 ||
        powerButtonLinkNum < 1 || videoButtonLinkNum < 1 || channelButtonLinkNum < 1) 
    {
        llSay(0,"problem, control panel not linked properly");
    }else
    if (llGetInventoryNumber(INVENTORY_TEXTURE) < 2) 
    {
        llSay(0,"no slides detected, please place slides in the contents.");
    }else
    {
        if (llGetInventoryName(INVENTORY_TEXTURE, 0) == "logo") currentSlide = 1;
        else currentSlide = 0;
        updateDisplays(); 
        power(TRUE);
        video(FALSE);
    }
    
}

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
    // When we enter this state do this stuff
    state_entry() 
    {
            init();
    }
    touch_start(integer total_number)
    {
        // **** user permissions, change the following check based on your desired permissions requirements
        if ( llDetectedGroup(0) ) // agent is in same active group as this object
        {
            // next button pressed
            if (llGetLinkName( llDetectedLinkNumber(0) ) == "next") 
            {
                if(!powerState) power(TRUE);
                if (videoState) video(FALSE);
                if (currentSlide < llGetInventoryNumber(INVENTORY_TEXTURE) - 1) 
                {
                    ++currentSlide;
                    if (llGetInventoryName(INVENTORY_TEXTURE, currentSlide) == "logo")
                    {
                        if (currentSlide < llGetInventoryNumber(INVENTORY_TEXTURE) - 1) 
                        {
                            ++currentSlide;
                        }else
                        {
                            // we are on last texture, and it is the logo, go to second to last texture
                            currentSlide = llGetInventoryNumber(INVENTORY_TEXTURE) - 2;
                        }
                        
                    }
                    updateDisplays();
                }    // else we are at the end, don't do anything
                
            }else
            // previous button pressed
            if (llGetLinkName( llDetectedLinkNumber(0) ) == "prev") 
            {
                if(!powerState) power(TRUE);
                if (videoState) video(FALSE);
                if (currentSlide > 1) 
                {
                    --currentSlide;
                    
                    if (llGetInventoryName(INVENTORY_TEXTURE, currentSlide) == "logo")
                    {
                        if (currentSlide > 0) 
                        {
                            --currentSlide;
                        }else
                        {
                            // we are on texture 0 and it is the logo, set current texture to 1
                            currentSlide = 1;
                        }
                        
                    }
                    updateDisplays();
                }    // else we are at the beggining, don't do anything            
            }else
            // begin button pressed
            if (llGetLinkName( llDetectedLinkNumber(0) ) == "begin") 
            {
                if(!powerState) power(TRUE);
                if (videoState) video(FALSE);
                // if texture 0 is the logo, go to texture 1 to start.
                if (llGetInventoryName(INVENTORY_TEXTURE, 0) == "logo") currentSlide = 1;
                else currentSlide = 0;
                updateDisplays();            
            }else
            // end button pressed
            if (llGetLinkName( llDetectedLinkNumber(0) ) == "end") 
            {
                if(!powerState) power(TRUE);
                if (videoState) video(FALSE);
                // go to end
                currentSlide = llGetInventoryNumber(INVENTORY_TEXTURE) - 1;
                updateDisplays();
            }else
            // end button pressed
            if (llGetLinkName( llDetectedLinkNumber(0) ) == "video") 
            {
                if ( llGetLandOwnerAt(llGetPos()) == llGetOwner() ) 
                {
                    // we have proper permissions
                    
                    if(!powerState) power(TRUE);
                    videoButtonLinkNum = llDetectedLinkNumber(0);

                    if (videoState) 
                    {
                        // Change texture back to current texture.
                        video(FALSE);
                        updateDisplays();
                    }else
                    {
                        mediaTexture = (key)llList2String(llParcelMediaQuery([PARCEL_MEDIA_COMMAND_TEXTURE]) ,0);
                        if (mediaTexture) 
                        {
                            // mediaTexture is valid
                            // set the video texture 
                            // Change texture to media texture
                            
                            video(TRUE);
                            llSay( chat2Display, "SETTEXTURE::"+ (string)mediaTexture);
                            llSetLinkTexture(currDisplayLinkNum, mediaTexture, displayFace);
                        }else
                        {
                            llSay(0,"No valid media texture set for parcel");
                        }
                    }


                }else
                {
                    // notify user we can't do this.
                    llSay(0,"Cannot set media texture. Laptop not owned by landowner");
                }

                
            }else
            // end button pressed
            if (llGetLinkName( llDetectedLinkNumber(0) ) == "power") 
            {
                if (videoState) video(FALSE);
                powerButtonLinkNum = llDetectedLinkNumber(0);
                if (powerState) 
                {
                    // power is on, so we must be turning off, find and display the logo texture
                    integer idx;
                    for (idx=0; idx < llGetInventoryNumber(INVENTORY_TEXTURE); ++idx)
                    {
                        if (llGetInventoryName(INVENTORY_TEXTURE, idx)== "logo") 
                        {
                            currentSlide = idx;
                            llSay( chat2Display, "SETTEXTURE::"+
                                (string)llGetInventoryKey( llGetInventoryName(INVENTORY_TEXTURE, currentSlide) ) );
                            llSetLinkTexture(currDisplayLinkNum, llGetInventoryKey( 
                                llGetInventoryName(INVENTORY_TEXTURE, currentSlide) ), displayFace);

                            // ***** Added 2011-08-02 also set preview screen to logo
                            llSetLinkTexture(nextDisplayLinkNum, llGetInventoryKey( 
                                llGetInventoryName(INVENTORY_TEXTURE, currentSlide) ), displayFace);
                            
                        }
                    }
                    power(FALSE);
                    
                }else
                {
                    // power must be off, lets turn on and go to first slide
                    
                    // if texture 0 is the logo, go to texture 1 to start, otherwise go to 0.
                    if (llGetInventoryName(INVENTORY_TEXTURE, 0) == "logo") currentSlide = 1;
                    else currentSlide = 0;
                    updateDisplays();
                    power(TRUE);
                    
                }

            }else
            if (llGetLinkName( llDetectedLinkNumber(0) ) == "channel") 
            {
                channelButtonLinkNum = llDetectedLinkNumber(0);
                currentChannel = (++currentChannel)%4;
                chat2Display = chatOffset+currentChannel;
                
                // set the channel texture offset
                llSetLinkPrimitiveParams(channelButtonLinkNum, [PRIM_TEXTURE, displayFace,
                    channelButtonTexture, <1.0,0.25,0.0>,<0.0, .375 - (.25*currentChannel) ,0.0>,0.0]);
                // llWhisper(0,"Channel set to " + (string)chat2Display);
                    
            }
        } // else not an approved key
        
    }
    link_message(integer sender_number, integer number, string message, key id) 
    {
        if (number == link2All) 
        {
            // channel to all scripts, parse the message
            myMessage = llParseString2List(message, ["::"], [""]);
            if (llList2String(myMessage, 0) == "channel") 
            {
                // change communication channel
                currentChannel = (integer)llList2String(myMessage, 1);
                chat2Display = chatOffset+currentChannel;
            }
            
            myMessage = [];
            
        }
    }
}
