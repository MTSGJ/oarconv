// Give information on xloc,yloc of Region/Sim on grid
// Ai Austin, Vue and OpenVCE.net, 19-Aug-2015

string gridname;
string gridhgurl;
string gridtext;
string simhost;
string region;
string xloc;
string yloc;
vector textcolour = <0.6,0.6,0.6>;
float texttrans = 1.0;

GetRegionDetails() {
    vector sim_coord = llGetRegionCorner();
    simhost = llGetSimulatorHostname();
    region = llGetRegionName();
    xloc = (string)((integer)(sim_coord.x / 256.0));
    yloc = (string)((integer)(sim_coord.y / 256.0));
}

MapGrid (string simhost, string region) {
    llOwnerSay("Simhost="+simhost+"  Region="+region);
    if ((simhost == "ERISKAY") || (simhost == "Eriskay") || (simhost == "eriskay") || (simhost == "virtual.aiai.ed.ac.uk") || (simhost == "virtual0.aiai.ed.ac.uk") || (simhost == "ARRAN") ||  (simhost == "Arran") || (simhost == "arran") ||(simhost == "virtual3.aiai.ed.ac.uk") || (simhost == "129.215.219.144")) 
                gridname = "Openvue";
    else if ((simhost == "ai.vue.ed.ac.uk") || (simhost == "TIREE") || (simhost == "Tiree") ||(simhost == "tiree") || (simhost == "SHUNA") ||  (simhost == "Shuna") || (simhost == "shuna") ||  (simhost == "virtual2.aiai.ed.ac.uk") || (simhost == "129.215.219.143"))
                gridname = "AiLand";
    else if ((simhost == "SHETLAND") || (simhost == "Shetland") || (simhost == "shetland") || (simhost == "129.215.219.85") || (simhost == "CARNA") || (simhost == "Carna") || (simhost == "carna") || (simhost == "129.215.219.78") || (simhost == "BORERAY") || (simhost == "Boreray") || (simhost == "boreray") ||  (simhost == "virtual4.aiai.ed.ac.uk") || (simhost == "129.215.219.81"))
                gridname = "OSGrid";
        else gridname = "Unknown";
        
    if (gridname == "Openvue") {gridhgurl = "virtual.aiai.ed.ac.uk:8002"; gridtext = gridhgurl;}
    else if (gridname == "AiLand") {gridhgurl = "ai.vue.ed.ac.uk:8002"; gridtext = gridhgurl;}
    else if (gridname == "OSGrid") {gridhgurl = "hg.osgrid.org:80"; gridtext = gridhgurl;}
    else {gridhgurl = "Unknown"; gridtext = simhost;}
    
}

default
{
    state_entry()
    {
        GetRegionDetails();
        MapGrid(simhost, region);
        llSetText(gridtext+"\n"+region+" at ("+xloc+","+yloc+")", textcolour, texttrans);            
    }
    
    on_rez (integer start_param)
    {
        GetRegionDetails();
        MapGrid(simhost, region);
        llSetText(gridtext+"\n"+region+" at ("+xloc+","+yloc+")", textcolour, texttrans);            
    }
    
    changed(integer change) { // something changed
        if (change & CHANGED_REGION) { // and it was a region/sim change
            GetRegionDetails();
            MapGrid(simhost, region);
            llSetText(gridtext+"\n"+region+" at ("+xloc+","+yloc+")", textcolour, texttrans);
        }
    }
  
    touch_start(integer num) {
        GetRegionDetails();
        MapGrid(simhost, region);
        llSetText(gridtext+"\n"+region+" at ("+xloc+","+yloc+")", textcolour, texttrans);
        llSay(0,gridtext+" "+region+" at ("+xloc+","+yloc+")");    
        if (gridname != "Unknown") llSay(0,"To return here click hop://"+gridtext+"/"+region+"/128/128/23");  
    }
}
