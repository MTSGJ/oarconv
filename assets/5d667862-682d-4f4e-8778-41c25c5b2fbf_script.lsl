vector textcolour = <0.6,0.6,0.6>;
float texttrans = 1.0;

default
{
    
    state_entry() {
        llSetText(llGetObjectName(),textcolour, texttrans);            
    }
    
    touch_start(integer total_number)
    {
        integer number = 0;
        do
            llSay(0,"The key to who touched me is: " + (string) llDetectedKey(number));
        while(total_number > ++number);
    }
}
