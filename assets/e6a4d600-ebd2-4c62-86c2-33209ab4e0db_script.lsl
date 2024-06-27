// Display URL Dialog

string title = "";
string url="";

vector label_colour = <0.6,0.6,0.6>;
float  label_alpha = 1.0;

default
{
    state_entry()
    {
        
        // if (title == "") title = llGetObjectName();         // unless manually specified use object name
        // llSetText(title, label_colour, label_alpha);        // for floating text
    }
    
    touch_start(integer num_detected)
    {
        key gAvatarKey = llDetectedKey(0);
        if (url == "") {
            title= llGetObjectName();
            url = llGetObjectDesc();  
        };   
        llLoadURL(gAvatarKey, title, url);
    }
}
