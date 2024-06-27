// I-Room Display - 28-Jul-2008
// SP Pizzicato updated by Ai Austin
// Based on SP Pizzicato - 19-Jun-2008

integer channel = 10;           //llDialog listen channel *** USE DIFFERENT CHANNEL FOR MULTIPLE SCREENS


integer listenHandle = -1;      //Dialog menu listen handler
integer listenUrl = -1;         //listen handler for channel 1 for when a URL is being added
integer listenTimer = -1;       //Timer variable for removing all listeners after 2 minutes of listener inactivity


// the verbs:
string DISPLAY = "display";
string SET = "set";


// the following modify the verb:
string ON = "on";
string OFF = "off";
string HELP = "help";
string RESET = "reset";
string LABEL = "label";
string NOLABEL = "nolabel";


//string DEFAULT_TEXTURE = "12788ca0-f20e-9af3-53f6-4be79fa19252";
//integer DISPLAY_ON_SIDE = ALL_SIDES; //Change this to change where the image will be displayed
integer DISPLAY_ON_SIDE = 1; // this is the +x face


string DEFAULT_TEXTURE = "5748decc-f629-461c-9a36-a35a221fe21f"; //Blank texture - Used when there are no textures to display in Picture mode 
string MEDIA_TEXTURE = "8b5fec65-8d8d-9dc5-cda8-8fdf2716e361";


string currentLabel = "";
string resetLabel = "";


string mediatype(string ext)    //Returns a string stating the filetype of a file based on file extension
{
    ext = llToLower(ext);
    if(ext == "swf")
        return "Flash";
    if(ext == "mov" || ext == "avi" || ext == "mpg" || ext == "mpeg" || ext == "smil")
        return "Video";
    if(ext == "jpg" || ext == "mpeg" || ext == "gif" || ext == "png" || ext == "pict" || ext == "tga" || ext == "tiff" || ext == "sgi" || ext == "bmp")
        return "Image";
    if(ext == "txt")
        return "Text";
    if(ext == "mp3" || ext == "wav")
        return "Audio";
    return "Unknown";
}



report(string str)
{
    llSetObjectDesc(str);
}



menu()
{
    string title="I-Room Display";
    list buttons=[DISPLAY+" "+ON,DISPLAY+" "+OFF,RESET];
    llDialog(llDetectedKey(0),title,buttons,channel);
}



set_label(string text)
{
    llSetText(text, <1.0,1.0,1.0>, 1);
}


no_label()
{
    llSetText("", <1.0,1.0,1.0>, 1);
}


on()
{
        //llSay(0, "Switching display on");
        listenHandle = llListen(channel,"","","");
        //llListenRemove(listenHandle);
        //listenHandle = -1;
        llSetTexture(DEFAULT_TEXTURE,DISPLAY_ON_SIDE);
        llSetTexture(MEDIA_TEXTURE,DISPLAY_ON_SIDE);
        llSetPrimitiveParams([PRIM_FULLBRIGHT, DISPLAY_ON_SIDE, TRUE]);
        llSetPrimitiveParams([PRIM_FULLBRIGHT, DISPLAY_ON_SIDE, TRUE, PRIM_BUMP_SHINY, DISPLAY_ON_SIDE, PRIM_SHINY_NONE, PRIM_BUMP_NONE, PRIM_COLOR, DISPLAY_ON_SIDE, <1,1,1>, 1.0, PRIM_MATERIAL, PRIM_MATERIAL_PLASTIC]);
 
        set_label(currentLabel);
}


off()
{
        //llSay(0, "Switching display off");
        // want to set to default texture:
        // at 2008-06-19 was llSetTexture(MEDIA_TEXTURE,DISPLAY_ON_SIDE);
        llSetTexture(DEFAULT_TEXTURE,DISPLAY_ON_SIDE);
        llSetPrimitiveParams([PRIM_FULLBRIGHT, DISPLAY_ON_SIDE, FALSE, PRIM_BUMP_SHINY, DISPLAY_ON_SIDE, PRIM_SHINY_LOW, PRIM_BUMP_NONE, PRIM_COLOR, DISPLAY_ON_SIDE, <0,0,0>, 1.0, PRIM_MATERIAL, PRIM_MATERIAL_PLASTIC]);
        //llListenRemove(listenHandle);
        
        reset();
        no_label();
    
}


reset()
{   
    currentLabel = resetLabel;
    set_label(currentLabel);
    llScaleTexture(1.0,1.0,DISPLAY_ON_SIDE);
    llOffsetTexture(0.0,0.0,DISPLAY_ON_SIDE);
}


default
{
    state_entry()
    {
        off();
    }
    
    on_rez(integer i)
    {
        llSay(0,"I-X Display Channel "+(string)channel);
        llResetScript();
    }


    touch_start(integer total_number)
    {
        menu();
        listenHandle = llListen(channel,"","","");
    }
    
    changed(integer change)
    {
    }


    listen(integer channel, string name, key id, string message)
    {
        //llWhisper(0, "heard "+message);
        
        list messagel = llParseString2List(message, [" "],[]);
        string verb = llList2String(messagel,0);
        
        if(verb == DISPLAY)
        {
             // now decide what the next command is:
            if(llGetListLength(messagel)>4){
                 // this is an offset-thingy message!
                 // 1st = verb, 2nd hrepeat 3rd vrepeat 4th hoffset 5th voffset 
                 llScaleTexture(llList2Float(messagel,1),llList2Float(messagel,2),DISPLAY_ON_SIDE);
                 llOffsetTexture(llList2Float(messagel,3),llList2Float(messagel,4),DISPLAY_ON_SIDE);
            }
            else{
                string mod = llList2String(messagel,1);
                if(llStringLength(mod)<6){                 
                 
                    if(mod == ON){
                         on();
                    }
                    else if(mod == OFF){
                        off();
                    }
                    else if(mod == RESET){
                        reset();
                    }
                    else if(mod == HELP){
                        llSay(0, "I-Room display: chat on channel "+(string)channel+": "+DISPLAY+" "+ON+"|"+OFF+"|"+HELP+"|<url>");
                    }
                }
                else 
                {    
                    string prot = llGetSubString(mod,0,3);
                    if(prot=="http" || prot=="rtsp"){ 
                        on();
                        // want to display the url:
                        //string url = llList2String(messagel,1);
                        //llSay(0, "request to show "+mod);
                        llParcelMediaCommandList([PARCEL_MEDIA_COMMAND_URL, mod, PARCEL_MEDIA_COMMAND_AUTO_ALIGN, TRUE, PARCEL_MEDIA_COMMAND_PLAY]);
                        llScaleTexture(1.0,1.0,DISPLAY_ON_SIDE);
                        llOffsetTexture(0.0,0.0,DISPLAY_ON_SIDE);
                        integer slashpos = llSubStringIndex(mod,"/");
                        string tempst = mod;
                        while(slashpos!=-1){
                            tempst =  llGetSubString(tempst,slashpos+1,-1);
                            slashpos = llSubStringIndex(tempst,"/");
                        }
                        
                        resetLabel = tempst;
                    }
                }   
            }
        }
        else if(verb == SET)
        {
            string mod = llList2String(messagel,1);
            if(mod == LABEL)
            {
                string tl = llList2String(messagel,2);
                if(tl==ON || tl==OFF || tl==RESET || tl==HELP){
                    // ignore
                }
                else {
                    integer slashpos = llSubStringIndex(tl,"/");
                    string tempst = tl;
                    
                    while(slashpos!=-1){
                       //lastpos = slashpos;
                        tempst =  llGetSubString(tempst,slashpos+1,-1);
                        slashpos = llSubStringIndex(tempst,"/");
                            //llSay(0,tempst);
                    }
                    currentLabel = tempst;
                    set_label(currentLabel);
                }
            }
            else if(mod == NOLABEL)
            {
                currentLabel = "";
                no_label();
            }
        }
        else if(verb == RESET)
        {
            reset();


        }
    }


}
