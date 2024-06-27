integer hours_offset = 8;

    display_hours()
{
    // llRotateTexture(((((llGetWallclock()-(hours_offset*60*60))/ 60) / 60) * 30) * DEG_TO_RAD, ALL_SIDES);
    llRotateTexture(-((((llGetGMTclock())/ 60) / 60) * 30) * DEG_TO_RAD, ALL_SIDES);
}

default
{
    state_entry()
    {
        display_hours();
        llSetTimerEvent(1);
    }
    
    timer()
    {
        display_hours();
    }
}