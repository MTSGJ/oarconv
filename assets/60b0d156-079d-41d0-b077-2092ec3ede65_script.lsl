// Floating Text set to object name.. set to null string "" to clear
default
{
    state_entry()
    {
         llSetText(llGetObjectName(), <0.0, 1.0, 0.0>, 1.0); // Display the object's current name in green
    }

}
