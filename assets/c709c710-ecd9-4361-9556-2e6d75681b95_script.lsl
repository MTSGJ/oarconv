//Place this script in the control box prim

integer DOORCLOSED = 0;
integer DOORINUSE = 1;
integer DOOROPEN = 2;

integer DoorState = 0;
default
{
    state_entry()
    {
        DoorState = DOORCLOSED;
    }
    
    link_message(integer Sender, integer Num, string Message, key ID)
    {
        //if sent a request to open
        if (Message == "OpenRequest")
        {
            //if no other door is open or in use
            if (DoorState == DOORCLOSED)
            {
                //set to door use state
                DoorState = DOORINUSE;
                llMessageLinked(Sender, 0, "Confirmed", "");
            }
        }
        
        //if reset request
        if (Message == "Reset")
        {
            //for this nothing at the moment.
            llMessageLinked(LINK_ALL_OTHERS, 0, "ForceClose", "");
        }
        
        //if a door closes
        if (Message == "Closed")
        {
            //set to door closed state
            DoorState = DOORCLOSED;
        }
        
        //if a door is opened
        if (Message == "Opened")
        {
            //set to door open state
            DoorState = DOOROPEN;
        }
    }
}
