integer count = 0;
integer refreshRate = 0;
string url = "http://www.aiai.ed.ac.uk/img/logo/aiai-186x186.jpg";
string dynamicID="";
string contentType="image";

refresh_texture()
{
    // refresh rate is not yet respected here, which is why we need the timer
    osSetDynamicTextureURL(dynamicID, contentType ,url , "", refreshRate );
}


default
{
    state_entry()
    {
        llSetText("osShowImageURL - refreshes every "+(string)refreshRate+" secs.\nOr touch to refresh now", <1.0, 0.5, 0.0>, 1);
        refresh_texture();
        // If refresh rate = 0 don't set timer.
        if (refreshRate != 0) llSetTimerEvent(refreshRate); // create a "timer event" every "refreshRate" seconds
    }


    timer()
    {
        refresh_texture();
    }


    touch_start(integer times)
    {
        llSay(0,"Refresh url: "+url);
        refresh_texture();
    }
}
