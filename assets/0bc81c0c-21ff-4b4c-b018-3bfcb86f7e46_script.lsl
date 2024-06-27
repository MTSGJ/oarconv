//////////////////////////////////////////////////////////////////////////////////////
//
//    Dialog Module
//    Version 9.1 Release
//    Copyright (C) 2004-2005  Strife Onizuka
//    http://home.comcast.net/~mailerdaemon/
//    http://secondlife.com/badgeo/wakka.php?wakka=LibraryDialogModule
//    
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public License
//    as published by the Free Software Foundation; either
//    version 2.1 of the License, or (at your option) any later version.
//    
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//    
//    You should have received a copy of the GNU Lesser General Public License
//    along with this library; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//    
//////////////////////////////////////////////////////////////////////////////////////

// Defaults: Change these

//com is the second integer returned from link_message that it must match
integer DialogComm = 12;

//defaulttimeout is value used when a dialog box time to live is below the min time
//(mintime is hardcoded to 5 seconds)
integer defaulttimeout = 45;

//how often to check the for timeouts.
float ticksize = 5.0;

//////////////////////////////////////////////////////////////////////////////////////
//Don't change anything else unless you *really* need to.

list handles;
list time;
list chans;
list users;
list answer;
list button;
list prim;
list mask;
list intchan;

integer ticks;

remove(integer a)
{
    llListenRemove(llList2Integer(handles,a));
    handles = llDeleteSubList(handles,a,a);
    time    = llDeleteSubList(time,   a,a);
    chans   = llDeleteSubList(chans,  a,a);
    users   = llDeleteSubList(users,  a,a);
    answer  = llDeleteSubList(answer, a,a);
    button  = llDeleteSubList(button, a,a);
    prim    = llDeleteSubList(prim,   a,a);
    mask    = llDeleteSubList(mask,   a,a);
    intchan = llDeleteSubList(intchan,a,a);
}

string quickdump(list a, string b){  return b + llDumpList2String(a, b); }

msglink(integer link, integer chan, string ans, list ret)
{
    if(chan != DialogComm)
        llMessageLinked(link, chan, ans, dump(ret, "|"));
}

string dump(list a, string b)
{
    string c = (string)a;
    if(1+llSubStringIndex(c,b) || llStringLength(b)!=1)
    {
        b += "|\\/?!@#$%^&*()_=:;~{}[],\n\" qQxXzZ";
        integer d = -llStringLength(b);
        while(1+llSubStringIndex(c,llGetSubString(b,d,d)) && d)
            d++;
        b = llGetSubString(b,d,d);
    }
    return b + llDumpList2String(a, b);
}

dialog(key user, string message, float timeout, list buttons,
        integer buttonmask, integer retchan, list ret)
{
    llMessageLinked(llGetLinkNumber(), DialogComm, dump([retchan] + ret, "|"),
        dump([user, message, timeout, buttonmask] + buttons, "|"));
}

list parse(string a) {
    string b = llGetSubString(a,0,0);//save memory
    return llParseStringKeepNulls(llDeleteSubString(a,0,0), [b],[]);
}

clear()
{
    integer a = llGetListLength(handles);
    while(a--)
    {
        llListenRemove(llList2Integer(handles,a));
        if(llList2Integer(mask,a)&0x20000)
            msglink(llList2Integer(prim,a), llList2Integer(intchan,a),
                    llList2String(answer,a), [-2, "", llList2Key(users,a), "", -1]);
    }
    chans = handles = time = users = mask = 
            intchan = prim = button = answer = [];
    ticks = 0;
    llSetTimerEvent(0);
}

default
{
    state_entry()
    {
        llOwnerSay("Dialog Module, Version 9.1, " +
            "Released Under the GNU Lesser General Public License");
        llOwnerSay("Copyright (C) 2004-2005 Strife Onizuka");
        if(ticksize < 2.5)
            ticksize = 2.5;
    }
    on_rez(integer a)
    {
        clear();
    }
    link_message(integer a, integer b, string c, key d)
    {
        if(b==DialogComm)
        {
            b = llSubStringIndex(llDeleteSubString(c,0,0), llGetSubString(c,0,0));
            list    e = parse(d);
            integer buttonmask =  (integer)llList2String(e,3);
            key     user       =  llList2String(e,0);
            list    buttons    =  llDeleteSubList(e,0,3);
            float   timeout    =  (float)llList2String(e,2);
            integer cat        =  -(integer)llFrand(0x7FFFFF00) - 255;
            integer chan       =  (integer)llDeleteSubString(c, b + 1, 0);
            string  ans        =  llDeleteSubString(c,0,b);
            
            if(buttonmask & 0x100000)
                clear();
            else if(buttonmask & 0x200000)
            {//clean out other box's that went to this user.
                while(1 + (b = llListFindList(users, [user])))
                {
                    if(llList2Integer(mask,b)&0x20000)
                        msglink(llList2Integer(prim,b), llList2Integer(intchan,b),
                            llList2String(answer,b), [-2, "", llList2Key(users,b), "", -1]);
                    remove(b);
                }
                if(time == [])
                {
                    chans = handles = time = users = mask = 
                            intchan = prim = button = answer = [];
                    llSetTimerEvent(0);
                    ticks = 0;
                }
            }
            if(timeout < 5.0 || timeout > 7200)
                timeout = defaulttimeout;
            while(1+llListFindList(chans,[cat]))
                --cat;
            if(user == "" || user == NULL_KEY) //lazy check
                user = llGetOwner();
            if(llGetAgentSize(user) == ZERO_VECTOR) 
            {// target not in the sim
                if(buttonmask & 0x40000)
                    msglink(a, chan, ans, [-3, "", user, "", 0]);
                return;
            }
            
            llDialog(user, llList2String(e,1), buttons, cat);
            if(chan != DialogComm)
            {//loopback catch
                if(buttons == []) // so we can match the ok button
                    buttons = ["OK"];
                if(buttonmask & ((1<<llGetListLength(buttons)) - 1))
                { //we checked the mask to see if we should expect any values back
                    chans   +=  cat;
                    handles +=  llListen(cat,"",user,"");
                    time    +=  (ticks + (timeout / ticksize));
                    users   +=  user;
                    answer  +=  ans;
                    button  +=  quickdump(buttons,llGetSubString((string)d,0,0));
                    prim    +=  a;
                    mask    +=  buttonmask;
                    intchan +=  chan;
                    llSetTimerEvent(ticksize);
                }
                else if(buttonmask & 0x80000)
                    msglink(a, chan, ans, [-4, "", user, "", 0]);
            }
        }
    }
    listen(integer a, string b, key c, string d)
    {
        a=llListFindList(chans,[a]);
        if(a+1 && llList2Key(users,a) == c)
        {//it's one of our listens
            integer f = llListFindList(parse(llList2String(button,a)),[d]);
            if(f+1)
            {//we matched a button
                if(llList2Integer(mask,a)&(1<<f))
                    msglink(llList2Integer(prim,a), llList2Integer(intchan,a),
                            llList2String(answer,a), [f, d, c, b,
                                (llList2Float(time,a) - ticks) * ticksize]);
                remove(a);
                if(time == [])
                {
                    chans = handles = time = users = mask = 
                            intchan = prim = button = answer = [];
                    llSetTimerEvent(0);
                    ticks = 0;
                }
            }
        }
    }
    timer()
    {
        ++ticks;
        integer a;
        float c;
        key d;
        while(a<llGetListLength(time))
        {
            if( ((c = llList2Float(time,a)) <= ticks) || 
                (llGetAgentSize(d = llList2Key(users,a)) == <0.0,0.0,0.0>))
            {
                if(llList2Integer(mask,a)&0x10000)
                    msglink(llList2Integer(prim,a), llList2Integer(intchan,a),
                            llList2String(answer,a), [-1, "", d, "", (ticks - c) * ticksize]);
                remove(a);
            }
            else
                ++a;
        }
        if(time == [])
        {
            chans = handles = time = users = mask = 
                    intchan = prim = button = answer = [];
            llSetTimerEvent(0);
            ticks = 0;
        }
    }
}
 