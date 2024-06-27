// Federated window controller by Jo Sartre, http://sldb.net
// change this value to another number if you want to make sure
// that your neighbour doesn't control your window ;)
integer channel = 5234;

string sepchar = "|";
integer answer = 546;
integer DialogComm = 12;

dialog(key user, string message, float timeout, list buttons, integer buttonmask, integer retchan, list ret)
{
    llMessageLinked(llGetLinkNumber(), DialogComm, dump([retchan] + ret, sepchar),
        dump([user, message, timeout, buttonmask] + buttons, sepchar));
}

string dump(list a, string b)
{
    string c = (string)a;
    if(1+llSubStringIndex(c,b) || llStringLength(b)!=1)
    {
        b += "|\\/?!@#$%^&*()_=:;~{}[],\n qQxXzZ";
        integer d = -llStringLength(b);
        while(1+llSubStringIndex(c,llGetSubString(b,d,d)) && d)
            d++;
        b = llGetSubString(b,d,d);
    }
    return b + llDumpList2String(a, b);
}

list parse(string a) {
    string b = llGetSubString(a,0,0);//save memory
    return llParseStringKeepNulls(llDeleteSubString(a,0,0), [b],[]);
}

default
{
 

    state_entry()
    {
        llOwnerSay("Federated window controller script by Jo Sartre");
        llListen(channel, "",NULL_KEY,"");
    }
    touch_start(integer a)
    {
        dialog(llDetectedKey(0), "How do you want to set the window blinds?", 30, 
        ["invisible", "open", "medium", "dark", "closed"], 0xf0fff, answer, []);
    }
    link_message(integer a, integer b, string c, key d)
    {
        if(b==answer)
        {
            integer selection;
            selection = llList2Integer(parse(d),0);
            if(selection >= 0 && selection <5) {
                llShout(channel,(string)selection);
                llMessageLinked(LINK_SET, selection, "transp", NULL_KEY);
            }
        }
    }
    listen( integer chan, string name, key id, string message) {
        if(chan==channel) { 
            integer selection;
            selection = (integer)message;
            if(selection >= 0 && selection <5) {
                llMessageLinked(LINK_SET, selection, "transp", NULL_KEY);
            }
        }
    }
}
