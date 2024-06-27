// osTeleportAgent
// Works for Hypergrid locations
// Uses Region name as shown in Map/HG map to effect teleport (and label Hypergate)
// UCI Admin - University of California at Irvine - 2-Feb-2009
//
// Set Destination as described below, There are a Few Options depending on Application:
// IN GRID Teleport 
// Destination = "1000,1000"; = Using In-Grid Map XXXX,YYYY coordinates
// Destination = "RegionName"; = Using RegionName
// HyperGrid Teleport (region must be HG Enabled)
// Destination = "TcpIpAddr:Port:RegionName"; = Using the Target/Destination IP Address
// Destination = "DNSname:Port:RegionName"; = Using the Target/Detination DNSname
// Note: RegionName is Optionally Specified to deliver Avatar to specific region in an instance.
// 
// ====================================================================


string region;

default
{
    state_entry()
    {
        region = llGetObjectName();
        llSetText("Direct Teleport to\n"+region, <1,1,1>, 1.0);
        llSay(0, "Script '" + llGetScriptName() + "' is a Hypergate to "+region);

    }
    
    touch_start(integer num_detected)
    {
        key agentKey = llDetectedKey(0);
        vector destination = <128,128,25>;    // or llGetPos();
        vector lookat = <-1,0,0>; // set new lookat heading WEST
        //string region = llGetObjectName();
        llWhisper(0, "Teleporting " + llDetectedName(0) + " to " + region);
        osTeleportAgent(agentKey, region, destination, lookat);
    }
}
