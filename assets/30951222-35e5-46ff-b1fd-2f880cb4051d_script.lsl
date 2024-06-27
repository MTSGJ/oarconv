//Gentilmente ripreso da Phoenix Legend.

    string GoogleURL = "http://ajax.googleapis.com/ajax/services/language/translate?v=1.0";
    
    string SRCLang = "en";
    string DSTLang = "it";


default
{
    on_rez(integer start_param)
    {
        llResetScript();
    }
    
    state_entry()
    {
        
        llOwnerSay("/me pronto a tradurre..!! \n*Scrivi </babbler it en> per parlare da ITA a ING oppure </babbler en it> per parlare da ING a ITA \n*Le possibiltà sono: it-en-fr-de-es-ru. \n*Per fermarlo fai <Detach> su di esso.");
llOwnerSay("/me ready to translate..!! \n*Write </babbler it en> for speek ITA to ING or </babbler en it> for speek ING to ITA \n*The possibilities are: it-en-fr-de-es-ru. \n*For stop <Detach> it.");
        llListen(0, "", llGetOwner(), "");
        
    }
    
    listen(integer channel, string name, key id, string message)
    {
        
        if (llGetSubString(message, 0, 8) == "/babbler ")
        {
            
            string command = llGetSubString(message, 9, -1);
            list langs = llParseString2List(command, [" "], []);
            
            SRCLang = llList2String(langs, 0);
            DSTLang = llList2String(langs, 1);
            
            llOwnerSay("/me ora traduce da " + SRCLang + " a " + DSTLang);
            
        }
        else
        {
            
            string url = GoogleURL + "&q=" + llEscapeURL(message) + "&langpair=" + SRCLang + llEscapeURL("|") + DSTLang;
            llHTTPRequest(url,  [], "");
            
        }
        
    }
    
    http_response(key id, integer status, list meta, string translation)
    {
        
        llSetObjectName("" + llKey2Name(llGetOwner()));
        llSay(0, llGetSubString(translation, 36, -52));
        
    }
    
}