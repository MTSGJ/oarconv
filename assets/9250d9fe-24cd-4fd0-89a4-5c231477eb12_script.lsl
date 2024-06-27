// I-Room Helper - Inactive 
// See http://www.aiai.ed.ac.uk/project/i-room/

default
{
    state_entry()
    {
        // Show that we're off.
        llSetText("I-Room Helper\n(Inactive)", <0.5,0.5,0.5>, 1);      
    }

    on_rez(integer start_param) {
        // every time we're rezzed, reset the script
        // this ensures that all local variables are set afresh
        llResetScript();
    }
    
    // Start robot behaviour by touching it, 
    // if one of the touches is in same group as object.
    touch_start(integer num)
    {
       llSay(0,"I-Room Helper is not connected to an I-X Server.\nSee http://www.aiai.ed.ac.uk/project/i-room/ for more details.");
    }
    
}

