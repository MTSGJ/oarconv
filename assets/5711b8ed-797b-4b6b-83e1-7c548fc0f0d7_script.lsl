// Copyright 2009 -  University of Edinburgh School of Informatics 
// Davie Munro - www.vue.ed.ac.uk & www.inf.ed.ac.uk
//
// SlideShow Keynote System
//
// 23-Jul-2009 Minor change by Ai Austin to use 5 prims as non-preview panes
// 27-Sep-2009 Simplified local and global variables to not overlap in name.
//             currentSlide = slideNumber; added in displaySlide to fix bug

integer currentSlide = 0;       //current slide being displayed
integer totalSlides = 0;        //total number of slides in the contents folder
integer NonPreviewPrims = 5;    //screen, frame, left, right and reset/go to start
integer previewPanes = 0;       //number of preview panes to display upcoming slides
string objectName = "blank";    //object name of current slide
string texturePreview = "blank";

key texturePreviewKey;

update(integer slideNumber)
{
    previewPanes = llGetNumberOfPrims() - NonPreviewPrims;
    integer slot = 1;
    integer slideToPreview=slideNumber + 1;
        
    do {
                
        if(slideToPreview<totalSlides){
        texturePreview = llGetInventoryName(INVENTORY_TEXTURE, slideToPreview);
        texturePreviewKey = llGetInventoryKey(texturePreview);
          
        
        //llSay(0, texturePreviewKey);
        llMessageLinked(slot+1, slideToPreview, texturePreview, texturePreviewKey);
        
        //we want to move to the next slide preview
        slideToPreview = slideToPreview + 1;
        
        }
        else {
            //there is no slide to preview so show blank texture
            llMessageLinked(slot+1, -1, "blank", TEXTURE_BLANK);
            
        }
        
        slot = slot + 1;
               
        }
        while(slot <= previewPanes && slideNumber<totalSlides);
    
}

nextSlide()
{
        currentSlide = currentSlide + 1; 
        if (currentSlide >= totalSlides) currentSlide = 0;
        objectName = llGetInventoryName(INVENTORY_TEXTURE, currentSlide);
        // llSay(0,"Slide: "+(string)currentSlide+" = "+objectName);
        llSetTexture(objectName, 1);             
        update(currentSlide);
           
}
   
previousSlide()
{
        currentSlide = currentSlide - 1;
        if (currentSlide < 0) currentSlide = totalSlides - 1;
        if (currentSlide >= totalSlides) currentSlide = 0;
        objectName = llGetInventoryName(INVENTORY_TEXTURE, currentSlide);
        // llSay(0,"Slide: "+(string)currentSlide+" = "+objectName);       
        llSetTexture(objectName, 1);       
        update(currentSlide);
       
}      

displaySlide(integer slideNumber){
    
    if(slideNumber< totalSlides && slideNumber>0){
        objectName = llGetInventoryName(INVENTORY_TEXTURE, slideNumber);
        currentSlide = slideNumber; // Added by Ai Austin 25-Sep-2009 to fix bug
        // llSay(0,"Slide: "+(string)currentSlide+" = "+objectName);
        llSetTexture(objectName, 1);
        update(currentSlide);
    }         
    
}            
                                    
default
{
    link_message(integer sender_num, integer num, string message, key id) {
        
         if(message == "next" )
            {
             nextSlide();
            }
        else if(message == "previous")
            {
             previousSlide();
            }
        else if(message == "refresh")
            {
             llResetScript(); 
            }
        else if(message == "displayslide")
        {
            // llSay(0,"Slide: "+(string)num);
            displaySlide(num);
            
        }
                        
        }

    
    state_entry()
    {
        totalSlides = llGetInventoryNumber(INVENTORY_TEXTURE);
        llSay(0,(string)totalSlides+" Slides.");
        currentSlide = 0;
        objectName = llGetInventoryName(INVENTORY_TEXTURE, currentSlide);
        llSetTexture(objectName, 1);
        update(currentSlide);
        
    }

    on_rez(integer foobar)
    {
        llResetScript();
        update(currentSlide);
    }


}

