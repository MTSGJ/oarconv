//  6 degrees for every minute or second
//  1440 minutes in a day

display_minutes()
{
    llRotateTexture(-(((llGetGMTclock() / 60)) * 6) * DEG_TO_RAD, ALL_SIDES);
}

default
{
    state_entry()
    {
        display_minutes();
        llSetTimerEvent(1);
    }
    
    timer()
    {
        display_minutes();
    }
}
