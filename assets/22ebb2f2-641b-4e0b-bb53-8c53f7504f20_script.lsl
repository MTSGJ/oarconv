vector sittingPosition = <0,0,-0.1>;

init()
{
    llSitTarget(sittingPosition, ZERO_ROTATION);
}

default
{
    state_entry()
    {
        init();
    }
   
    changed(integer change) 
    {
       if (change & CHANGED_LINK)
        {
            key agent = llAvatarOnSitTarget();
            if (agent != NULL_KEY ) 
            {
                llRequestPermissions(agent,PERMISSION_TRIGGER_ANIMATION);
            } else {
                    llStopAnimation(llGetInventoryName(INVENTORY_ANIMATION, 0));
            }
      }        
    }
    
    run_time_permissions(integer value) 
    {
        if (value == PERMISSION_TRIGGER_ANIMATION) 
        {
            llStopAnimation("sit");
            llStartAnimation(llGetInventoryName(INVENTORY_ANIMATION, 0));
        }
    }
}