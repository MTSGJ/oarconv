// Show on map for teleport script, and give notecard if one in inventory
// Ai Austin 7-Nov-2007

string title = "";
vector label_colour = <1.0,1.0,1.0>;
float  label_alpha = 1.0;

key request;
string name;
string sim_name;
vector pos;
 
default
{
    state_entry()
    {
        
        if (title == "") title = llGetObjectName();         // unless manually specified use object name
        llSetText(title, label_colour, label_alpha);        // for floating text
    
        if (llGetInventoryNumber(INVENTORY_LANDMARK))       // check one landmark is present (more are ignored)
        {
            name = llGetInventoryName(INVENTORY_LANDMARK,0);
            request = llRequestInventoryData(name);
        }
        else
            { // no landmark yet.  Will work when one is put in object
              llAllowInventoryDrop(1); 
              llWhisper(0,"Please drop a landmark on me");
            }
    }
    
    dataserver(key id, string data)
    {
        if(id == request)
        {
            pos = (vector)data;
            sim_name = llGetRegionName();
        }
    }
    
    touch_start(integer a)
    {
        if(name != "")
            llMapDestination(sim_name, pos, pos);
    }
    
    changed(integer a)
    {
        if(a & (CHANGED_INVENTORY | CHANGED_ALLOWED_DROP))
            if(llGetInventoryNumber(INVENTORY_LANDMARK))
                name = llGetInventoryName(INVENTORY_LANDMARK,0);
                request = llRequestInventoryData(name);
    }
}
