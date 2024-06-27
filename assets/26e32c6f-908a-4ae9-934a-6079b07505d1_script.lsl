//http://www.hypergridbusiness.com/2014/10/easier-google-spreadsheet-visitor-logger/

// Script Name: Google Spreadsheet Visitor Recorder v2.0
// By Maria Korolov of Hypergrid Business
// Updated by Zuza Ritt in August 2013
// Adapted from Visitor_List_Maker.lsl 
// And from Hyperica Lookup HG Teleport Script (with code contributed by Paul Emery)

// This program is free software; you can redistribute it and/or modify it.

// Creative Commons licenses apply to all scripts from the Second Life
// wiki and script library and are Copyrighted by Linden Lab. See
// http://creativecommons.org/licenses/

// Please leave any author credits and headers intact in any script you use or publish.
// If you don't like these restrictions, then don't use these scripts.

// Global variables

string FrontPart;
string LocationKey;
string AvatarKey;
string Location;
string Avatar; 
string URL;
key httpkey;

integer list_size =10; //how many avatar names to keep in memory
float range = 100.0; // in meters
float rate = 1.0; // in seconds
list visitor_list;
integer len;

// Functions
integer isNameOnList( string name )
{
    len = llGetListLength( visitor_list );
    integer i;
    for( i = 0; i < len; i++ )
    {
        if( llList2String(visitor_list, i) == name )
        {
            return TRUE;
        }
    }
    return FALSE;
}


// States

default
{
    state_entry()
    {
       //  llSetText("", <0,1,0>, 1.0);
        llOwnerSay("Visitor recording script started...");
        Location= llGetRegionName()+" on "+osGetGridName();
        URL=llGetObjectDesc();
        if( llGetSubString(URL, 0, 22) == "https://docs.google.com" || llGetSubString(URL, 0, 21) == "http://docs.google.com")
        {
            list TermsList = llParseString2List(URL, ["/", ".","="],[]);
            FrontPart = llList2String(llParseString2List(URL,["/viewform"], []),0);
            LocationKey = llList2String(TermsList,-3);
            AvatarKey =  llList2String(TermsList,-1);
            state recording;
        }
        else 
        {
            llOwnerSay("Please create a Google form with two questions: Location? and Avatar Name?");
            llOwnerSay("Then please paste the URL of the prefilled form into this object's description.");
            llOwnerSay("For more information, please see this article:");
            llOwnerSay("http://www.hypergridbusiness.com/2014/10/easier-google-spreadsheet-visitor-logger");
            llOwnerSay("When done, please touch object to reset.");
            state waiting;
        }
    }
}

state waiting 
{
    state_entry() 
    {
        // run this code when entering state 'on'
        // displays green "ON" as floating text above the prim
        llSetText("Waiting for form URL in description. Touch to resume.", <0,1,0>, 1.0);
    }

    touch_start(integer num_detected) 
    {
        // when touched, switch to the default state
    llSetText("", <0,1,0>, 1.0);
    state default;
    }
}

state recording 
{
    state_entry() 
    {  
        llSensorRepeat( "", "", AGENT, range, TWO_PI, rate );
    }

    sensor( integer number_detected )
    {
        integer i;
        for( i = 0; i < number_detected; i++ )
//    if( llDetectedKey( i ) != llGetOwner() )
        {
        string Avatar = llDetectedName( i );
        if( isNameOnList( Avatar ) == FALSE )
            {
            string URL = FrontPart + "/formResponse" + "?entry."+LocationKey + "="  + Location +  "&entry." + AvatarKey + "=" + 
Avatar + "&submit=Submit";
            key httpkey=llHTTPRequest(URL, [] ,"");
            visitor_list += Avatar;
            if (len > list_size)
                { 
                    visitor_list =  llDeleteSubList(visitor_list,0,0); 
                }
            }    
        }
    }

}


// END //

