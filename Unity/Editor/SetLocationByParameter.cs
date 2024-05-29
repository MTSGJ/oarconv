//
// SetLocationByPatameter for oarconv by Fumi.Iseki 2024 (C) v1.1.2
//
// see also https://github.com/MTSGJ/oarconv
//
//

using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;


public sealed class SetLocationByPatameter : EditorWindow
{
	static string _magicSTR = "metaverse.jp.net.";
	
    static SetLocationByPatameter()
    {
        SetLocation();
    }

    [MenuItem("Tools/Set Location By Patameter")]


    //
    public static void SetLocation()
    {
        List<GameObject> gameObjectList = new List<GameObject>();
        // Scene 中の全ての Objectを探す
        foreach (GameObject gameObject in Resources.FindObjectsOfTypeAll(typeof(GameObject))) {
            if (gameObject.name!=null && gameObject.name.Contains(_magicSTR)) gameObjectList.Add(gameObject);
        }

        foreach (GameObject gameObject in gameObjectList) {
            bool trans = false;
            //UnityEngine.Debug.Log("File Name = " + gameObject.name);
            string obj_name = (string)gameObject.name;
            Vector3 position = getLocationFromName(obj_name, ref trans);
            if (trans) gameObject.transform.position = position;
        }
        AssetDatabase.Refresh();
    }


    private static Vector3 getLocationFromName(string name, ref bool trans)
    {
        trans = false;
        int magic_len = _magicSTR.Length;
        int pos_len = 16;    // POSITION LENGTH (4*3/3*4 = 16)
        float[] shift = new float[3];
        //
        int pos = name.LastIndexOf(_magicSTR);
        if (pos>=0) {
            string sub = name[(pos + magic_len)..];
            if (sub.Length>=pos_len) {
                if (sub.Length > pos_len) sub = sub.Substring(0, pos_len);
                string enc = sub.Replace('-', '/');
                //UnityEngine.Debug.Log("Base64 String = " + enc);
                try {
                    byte[] dec = Convert.FromBase64String(enc);    // 12 Byte
                    Buffer.BlockCopy(dec, 0, shift, 0, 4);
                    Buffer.BlockCopy(dec, 4, shift, 4, 4);
                    Buffer.BlockCopy(dec, 8, shift, 8, 4);
                    trans = true;
                    //UnityEngine.Debug.Log(name + " ===> " + shift[0] + ", " + shift[1] + ", " + shift[2]);
                }
                catch {
                    UnityEngine.Debug.Log("SetLocationByPatameter: Base64 Decode Error = " + enc);
                }
            }
        }
        Vector3 position = new Vector3(shift[0], shift[1], shift[2]);
        return position;
    }
}

