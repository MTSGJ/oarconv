//
// SetLocationByPatameter for oarconv by Fumi.Iseki 2024 (C) v1.0.0
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
            if (AssetDatabase.GetAssetOrScenePath(gameObject).Contains(".unity")) {
                gameObjectList.Add(gameObject);
            }
        }

        foreach (GameObject gameObject in gameObjectList) {
            Vector3 position = getLocationFromName(gameObject.name);
            if (position!=Vector3.zero) {
                gameObject.transform.position = position;
            }
        }
        AssetDatabase.Refresh();
    }


    private static Vector3 getLocationFromName(string name)
    {
        int pos_len = 16;    // POSITION LENGTH (4*3/3*4 = 16)
        float[] shift = new float[3];
        //
        if (name.Length >= pos_len) {
            int pos = name.LastIndexOf("_");
            if (pos>=0) {
                string sub = name.Substring(pos+1, pos_len);
                //UnityEngine.Debug.Log("Base64 String = " + sub);
                string enc = sub.Replace('-', '/');
                if (enc.Length == pos_len) {
                    try {
                        byte[] dec = Convert.FromBase64String(enc);    // 12 Byte
                        Buffer.BlockCopy(dec, 0, shift, 0, 4);
                        Buffer.BlockCopy(dec, 4, shift, 4, 4);
                        Buffer.BlockCopy(dec, 8, shift, 8, 4);
                        //UnityEngine.Debug.Log(name + " ===> " + shift[0] + ", " + shift[1] + ", " + shift[2]);
                    }
                    catch {
                        UnityEngine.Debug.Log("Not Supported File Name = " + name);
                    }
                }
            }
        }
        Vector3 position = new Vector3(shift[0], shift[1], shift[2]);
        return position;
    }
}
