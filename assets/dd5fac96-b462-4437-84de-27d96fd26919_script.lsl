default
{
    state_entry()
    {
        // Turn off any previously set sit target on prim
        llSitTarget(ZERO_VECTOR,ZERO_ROTATION);
        
        // Stop any rotation
        llTargetOmega(<0,0,0>,0,0);

        // Turn off any particle effects in prim
        llParticleSystem([]);
        
        // Turn off any sounds in prim - see http://wiki.secondlife.com/wiki/LlStopSound
        llStopSound();
        
        llSetText("", <1,1,1>, 1.0);
        llSetSitText("");
    }
    
    touch_start (integer num_detected) {
        // Turn off any previously set sit target on prim
        llSitTarget(ZERO_VECTOR,ZERO_ROTATION);
                
        // Stop any rotation
        llTargetOmega(<0,0,0>,0,0);

        // Turn off any particle effects in prim
        llParticleSystem([]);
        
        // Turn off any sounds in prim - see http://wiki.secondlife.com/wiki/LlStopSound
        llStopSound();
        
        llSetText("", <1,1,1>, 1.0);
        llSetSitText("");
        
        llSitTarget(ZERO_VECTOR,ZERO_ROTATION);
        
    }

}
