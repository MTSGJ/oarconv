// Copyright 2009 -  University of Edinburgh School of Informatics 
// Davie Munro - www.vue.ed.ac.uk & www.inf.ed.ac.uk
//
// SlideShow Keynote System
//

integer slide;

default {
    link_message(integer sender_num, integer slideNumber, string texture, key textureID) {
        
            
        if(texture=="blank")
            llSetTexture(TEXTURE_BLANK,1); 
                else        
            llSetTexture(textureID,1); 
            
            
            
        
            slide = slideNumber;           
        }
        
        
        touch_start(integer total_number)
    {
        llMessageLinked(1, slide, "displayslide", "");
    }
}