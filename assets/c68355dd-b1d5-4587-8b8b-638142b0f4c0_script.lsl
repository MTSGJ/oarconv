
default
{
     state_entry()
     {
         llSetStatus(STATUS_PHANTOM, TRUE);
         llSetTexture("lit_texture", ALL_SIDES);
         llListen(0,"","","blow");
         llListen(0,"","","light");
     }

     touch_start(integer number)
     {
            llSetTexture("lit_texture",ALL_SIDES);
     }

     listen(integer number, string name, key id, string message)
     {
        if(message=="blow")
        {
           
           llDie();
        }
      
        }
     }
 
 
  