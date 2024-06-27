//todo: Code to (in this and other scripts)
//1) Create a door handler object and script
//2) Send, recive and interperate link messages about status
//      Must be able to handel an arbitrary number of doors
//3) Only open when status indicates it should
//4) Set the lights to indicate status
//      Green = the door can be opened
//      Orange = A door is in use
//      Red = The door can not be opened

integer Steps = 10;

float OpenState = 0.6;
float ClosedState = 0.0;

float hollow = 99.0;                    // 0.0 to 0.95
vector cut = <0.0, 1.0, 0.0>;
vector twist = <0, 0.0, 0.0>;          // -1.0 to 1.0
vector holesize = <0.0, 0.50, 0.0>;    // max X:1.0 Y:0.5
vector topshear = <0.0, 0.0, 0.0>;     // -0.5 to 0.5
vector profilecut = <0.0, 0.75, 0.0>;   // 0.0 to 1.0
vector taper_a = <0.0, 0.0, 0.0>;      // 0.0 to 1.0
float revolutions = 1.0;               // 1.0 to 4.0
float radiusoffset = 0.0;              // -1.0 to 1.0
float skew = 0.0;

string DoorOpening = "Door1";
string DoorCloseing = "Door1";
string DoorOpened = "Door2";
string DoorClosed = "Door2";

default
{
    state_entry()
    {
        profilecut.x = ClosedState;
        
        llSetPrimitiveParams([  PRIM_TYPE,
                                PRIM_TYPE_TORUS,
                                PRIM_HOLE_DEFAULT,
                                cut,
                                hollow,
                                twist,
                                holesize,
                                topshear,
                                profilecut,
                                taper_a,
                                revolutions, 
                                radiusoffset,
                                skew
                            ]);    
        state Closed;
    }
}

state Closed
{
    state_entry()
    {
        llSetTimerEvent(0.0);
        llMessageLinked(LINK_ALL_OTHERS, 0, "Closed", "");
        llPlaySound(DoorClosed,1.0);
    }

    link_message(integer Sender, integer Num, string Message, key SenderID)
    {
        if (Message =="Confirmed")
        {
            //go to door opening
            state Opening;
        }
    }
    
    touch_end(integer User)
    {
        llMessageLinked(LINK_ALL_OTHERS, 0, "OpenRequest", "");
    }
}

state Opening
{
    state_entry()
    {
        llSetTimerEvent(0.2);
        llMessageLinked(LINK_ALL_OTHERS, 0, "Opening", "");
        llPlaySound(DoorOpening,1.0);
    }
    
    timer()
    {
        profilecut.x = profilecut.x + ((OpenState - ClosedState) / Steps);
        
        if (profilecut.x >= OpenState)
        {
            profilecut.x = OpenState;
        }
        
        llSetPrimitiveParams([  PRIM_TYPE,
                                PRIM_TYPE_TORUS,
                                PRIM_HOLE_DEFAULT,
                                cut,
                                hollow,
                                twist,
                                holesize,
                                topshear,
                                profilecut,
                                taper_a,
                                revolutions, 
                                radiusoffset,
                                skew
                            ]);
        if (profilecut.x >= OpenState)
        {
            state Open;
        }
    }
    
    link_message(integer Sender, integer Num, string Message, key ID)
    {
        if (Message == "ForceClose");
        {
            state Closing;
        }
    }
}

state Open
{
    state_entry()
    {
        llSetTimerEvent(10.0);
        llMessageLinked(LINK_ALL_OTHERS, 0, "Open", "");
        llPlaySound(DoorOpened,1.0);
    }
    
    touch_end(integer IUser)
    {
        state Closing;
    }
    
    timer()
    {
        state Closing;
    }
    
    link_message(integer Sender, integer Num, string Message, key ID)
    {
        if (Message == "ForceClose");
        {
            state Closing;
        }
    }
}

state Closing
{
    state_entry()
    {
        llSetTimerEvent(0.2);
        llMessageLinked(LINK_ALL_OTHERS, 0, "Closing", "");
        llPlaySound(DoorCloseing,1.0);
    }
    
    timer()
    {
        profilecut.x = profilecut.x - ((OpenState- ClosedState) / Steps);
        
        if (profilecut.x <= ClosedState)
        {
            profilecut.x = ClosedState;
        }
        
        llSetPrimitiveParams([  PRIM_TYPE,
                                PRIM_TYPE_TORUS,
                                PRIM_HOLE_DEFAULT,
                                cut,
                                hollow,
                                twist,
                                holesize,
                                topshear,
                                profilecut,
                                taper_a,
                                revolutions, 
                                radiusoffset,
                                skew
                            ]);
                            
        if (profilecut.x <= ClosedState)
        {
            state Closed;
        }
    }
}
