vector textcolour = <0.6,0.6,0.6>;
float texttrans = 1.0;

default
{
    
    state_entry() {
        llSetText(llGetObjectName(),textcolour, texttrans);            
    }
    
    touch_start(integer total_number){   
        list pdetails = llGetParcelDetails(llGetPos(), [PARCEL_DETAILS_GROUP]);
        llSay(0, "Parcel UUID: "+llDumpList2String(pdetails,", "));
 
        key id = llDetectedKey(0);
        string avatarname = llDetectedName(0);
        llSay(0, "Avatar UUID: "+(string) id+" You are: "+avatarname);

    }
    
    
}