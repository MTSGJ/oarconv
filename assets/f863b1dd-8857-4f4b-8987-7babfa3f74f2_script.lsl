default
{
    state_entry()                                   
    {    
        llSetTextureAnim (ANIM_ON | LOOP, ALL_SIDES, 4, 4, 0, 0, 12.0);              

    }
    on_rez(integer start_param) {
        // every time we're rezzed, reset the script
        // this ensures that if we're transferred to a new owner, we're listening to them and not still to the previous one
        llResetScript();

    
    }   
    
     
}
