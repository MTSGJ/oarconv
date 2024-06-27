//  Analog Wall Clock
//  Created by Water Rogers for IBM/Opensource

//  Purpose
//  --------------------------------------------------------------
//  This purpose of this script is to show llRotateTexture() in
//  action.  It also uses multiple scripts in different links of
//  the object.

//  Requirements
//  --------------------------------------------------------------
//  3 Prims are required for all 3 functions, as well as a texture
//  that looks like a clock hand where the rotation of the object's
//  Roll is 0 degrees.

//  Usage
//  --------------------------------------------------------------
//  Put this script into an object, and use the function desired.
//  Hours, minutes, and seconds all affect how and when the texture
//  will rotate.


//  EVENTS
//  --------------------------------------------------------------


display_seconds()
{
    //  Same as the other two, except the wallclock already returns how many
    //  seconds, so there is no need to do any other math other then to translate
    //  to radians and rotate the texture.
    llRotateTexture(-(llGetGMTclock() * 6) * DEG_TO_RAD, ALL_SIDES);
}

default
{
    state_entry()
    {
        //  Call the function that this object will represent.  This particular
        //  object happens to be the "seconds" hand of the clock.  The other 2
        //  will call the other 2 respectively.
        display_seconds();
        llSetTimerEvent(1);
    }
    
    timer()
    {
        display_seconds();
    }
}