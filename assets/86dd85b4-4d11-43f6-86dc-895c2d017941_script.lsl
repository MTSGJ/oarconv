default
{
    state_entry()
    {
        llSitTarget(<0.5, 0.0, -0.05>, <0.00000, 0.08716, 0.00000, 0.99619>); 
    }

    touch_start(integer total_number)
    {
        llSay(0, "Right click me and choose 'Sit Here' to sit down");
    }
}
 