//Place this script in any object that is to be an indicator light for the door
//Green means that any door can be opened
//Orange means a door is in unse (opening or closing)
//Red means a door is opened

default
{
    link_message(integer Sender, integer Num, string Message, key ID)
    {
        if (Message == "Opening" || Message == "Closing")
        {
            //turn orange
            llSetColor(<1.0,0.5,0.0>,ALL_SIDES);
            llSetPrimitiveParams([PRIM_POINT_LIGHT,1,<1.0,0.5,0.0>,1.0,0.1]);
        }
        
        if (Message == "Open")
        {
            //turn red
            llSetColor(<1.0,0.0,0.0>,ALL_SIDES);
            llSetPrimitiveParams([PRIM_POINT_LIGHT,1,<1.0,0.0,0.0>,1.0,0.1]);
        }
        
        if (Message == "Closed")
        {
            //turn green
            llSetColor(<0.0,1.0,0.0>,ALL_SIDES);
            llSetPrimitiveParams([PRIM_POINT_LIGHT,1,<0.0,1.0,0.0>,1.0,0.1]);
        }
    }
}
