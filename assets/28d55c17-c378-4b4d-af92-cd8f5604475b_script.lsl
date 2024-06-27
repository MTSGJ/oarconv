default
{
    state_entry()
    {
        llSetText("Touch to Clear\nAcknowledgements",<0.5,0.5,0.5>,1.0);
    }

    touch_start(integer total_number)
    {
        llSay(134, "clear");
        llSay(0, "All votes cleared");
    }
}
