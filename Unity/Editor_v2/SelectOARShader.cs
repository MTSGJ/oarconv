//
// SelectOARShader for oarconv by Fumi.Iseki 2015-2024 (C) v2.0.0
//
// see also https://github.com/MTSGJ/oarconv
//
//

using UnityEngine;
using UnityEditor;
using System;
using System.IO;


public class SelectOARShader : AssetPostprocessor
{
    private static int UnityVersion;
    private static string NormalShader;
    private static string TransShader;             // Alpha Blending
    private static string TransCutShader;          // Alpha Cutoff
    private static string TransCutSoftShader;      // Alpha Blending
    private static string TransSpecShader;         // Alpha Blending + Specular
    private static string TransCutSpecShader;      // Alpha Cutoff + Specular
    private static string SpecularShader;
    private static string BrightShader;
    private static string GlowShader;
    private static string TreeShader;

    private static float colorRed      = 0.0f;
    private static float colorGreen    = 0.0f;
    private static float colorBlue     = 0.0f;
    private static float transparent   = 1.0f;
    private static float cutoff        = 0.0f;
    private static float shininess     = 0.0f;
    private static float glow          = 0.0f;
    private static float bright        = 0.0f;
    private static float light         = 0.0f;
//  private static float shiftU        = 0.0f;
//  private static float shiftV        = 0.0f;
//  private static float scaleU        = 1.0f;
//  private static float scaleV        = 1.0f;
//  private static float rotate        = 0.0f;
    private static char  kind          = 'O';        // Object

    private const string PhantomFolder = "Phantoms";


    void OnPreprocessModel()
    {
        string currentFolder = Path.GetDirectoryName(assetPath);
        ModelImporter modelImporter = assetImporter as ModelImporter;

        modelImporter.addCollider = true;
        if (currentFolder.EndsWith("/" + PhantomFolder) || currentFolder.EndsWith("\\" + PhantomFolder)) {
            modelImporter.addCollider = false;
        }
    }

    static void GetUinityVersion()
    {
        string version = UnityEngine.Application.unityVersion;
        //
        if (version.StartsWith("6000.")) UnityVersion = 6000;
        else if (version.StartsWith("2023.")) UnityVersion = 2023;
        else if (version.StartsWith("2022.")) UnityVersion = 2022;
        else UnityVersion = 0;
    }

    static void InitialShader()
    {
        if (UnityVersion == 6000) {
            string standardShader = "HDRP/Lit";
            string transparentShader = "Unlit/Transparent";
            Shader shdr = Shader.Find(standardShader);
            if (shdr == null) standardShader = "Universal Render Pipeline/Simple Lit";

            NormalShader       = standardShader;
            TransShader        = transparentShader;         // Alpha Blending
            TransCutShader     = transparentShader;         // Alpha Cutoff
            TransCutSoftShader = transparentShader;         // Alpha Blending
            TransSpecShader    = transparentShader;         // Alpha Blending + Specular
            TransCutSpecShader = transparentShader;         // Alpha Cutoff + Specular
            SpecularShader     = standardShader;
            BrightShader       = standardShader;
            GlowShader         = standardShader;
            TreeShader         = transparentShader;
        }
        else { 
            NormalShader       = "Legacy Shaders/Diffuse";
            TransShader        = "Legacy Shaders/Transparent/Diffuse";                  // Alpha Blending
            TransCutShader     = "Legacy Shaders/Transparent/Cutout/Diffuse";           // Alpha Cutoff
            TransCutSoftShader = "Legacy Shaders/Transparent/Cutout/Soft Edge Unlit";   // Alpha Blending
            TransSpecShader    = "Legacy Shaders/Transparent/Specular";                 // Alpha Blending + Specular
            TransCutSpecShader = "Legacy Shaders/Transparent/Cutout/Specular";          // Alpha Cutoff + Specular
            SpecularShader     = "Standard";
            BrightShader       = "Legacy Shaders/Self-Illumin/Specular";
            GlowShader         = "Standard";
            TreeShader         = "Legacy Shaders/Transparent/Cutout/Soft Edge Unlit";
        }
    }

    static void OnPostprocessAllAssets(string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromAssetPaths, bool didDomainReload)
    {
        GetUinityVersion();
        InitialShader();
        AssetDatabase.Refresh();
        //
        foreach (string asset in importedAssets) {
            //string type = Path.GetExtension(asset);
            //if (type != "dae" && type != "obj") continue;
            //
            UnityEngine.Object[] objects = AssetDatabase.LoadAllAssetsAtPath(asset);
            if (objects!=null) {
                foreach (UnityEngine.Object obj in objects) {
                    string objName = obj.name;
                    if (objName.StartsWith("MATERIAL_")) {
                        Material material = obj as Material;
                        SetMaterialShader(material);
                        EditorUtility.SetDirty(material);
                    }
                }
            }
        }
        AssetDatabase.Refresh();
    }

    static void SetMaterialShader(Material material)
    {
        //InitialShader();

        string materialName = material.name;
        getParamsFromMaterialName(materialName);

        if (kind == 'T' || kind == 'G') {   // Tree or Grass
            material.shader = Shader.Find(TreeShader);
        }
        //
        else if (transparent < 0.99f) {
            if (shininess > 0.01f) {
                if (cutoff > 0.01f) {   // Alpha Cutoff
                    material.shader = Shader.Find(TransCutSpecShader);
                    if (material.HasProperty("_Cutoff")) material.SetFloat("_Cutoff", cutoff);
                }
                else {   // Alpha Blending
                    if (transparent >= 0.90f) {
                        material.shader = Shader.Find(TransCutSpecShader);
                        if (material.HasProperty("_Cutoff")) material.SetFloat("_Cutoff", 0.2f);
                    }
                    else {
                        material.shader = Shader.Find(TransSpecShader);
                        if (material.HasProperty("_Cutoff")) material.SetFloat("_Cutoff", cutoff);
                    }
                }
                if (material.HasProperty("_Shininess")) material.SetFloat("_Shininess", 1.0f - shininess);
            }
            //
            else {
                if (cutoff > 0.01f) {   // Alpha Cutoff
                    material.shader = Shader.Find(TransCutShader);
                    if (material.HasProperty("_Cutoff")) material.SetFloat("_Cutoff", cutoff);
                }
                else {   // Alpha Blending
                    if (transparent >= 0.90f) {
                        material.shader = Shader.Find(TransCutSoftShader);
                        if (material.HasProperty("_Cutoff")) material.SetFloat("_Cutoff", 0.9f);
                    }
                    else if (transparent >= 0.70f) {
                        material.shader = Shader.Find(TransShader);
                        if (material.HasProperty("_Cutoff")) material.SetFloat("_Cutoff", cutoff);
                    }
                    else {
                        material.shader = Shader.Find(NormalShader);
                        if (material.HasProperty("_Cutoff")) material.SetFloat("_Cutoff", cutoff);
                    }
                }
            }
        }
        //
        else if (glow > 0.02f) {
            material.EnableKeyword("_EMISSION");
            material.shader = Shader.Find(GlowShader);
            if (material.HasProperty("_EmissionColor")) {
                Color col = material.GetColor("_Color");
                float fac = col.maxColorComponent;
                if (fac > 0.01f) {
                    if (glow > 0.99f) glow = 0.99f;
                    col = col * (glow / fac);
                }
                material.SetColor("_EmissionColor", col);
            }
            if (material.HasProperty("_Glossiness")) material.SetFloat("_Glossiness", shininess);
        }
        //
        else if (bright > 0.01f) {
            material.shader = Shader.Find(BrightShader);
            if (material.HasProperty("_Shininess")) material.SetFloat("_Shininess", 1.0f - shininess);
        }
        //
        else if (shininess > 0.01f) {
            material.shader = Shader.Find(SpecularShader);
            if (material.HasProperty("_Metallic")) material.SetFloat("_Metallic", shininess / 2.0f);
            if (material.HasProperty("_Glossiness")) material.SetFloat("_Glossiness", 0.5f + shininess / 2.0f);
        }
        //
        else {
            material.shader = Shader.Find(NormalShader);
        }
        //
        if (material.HasProperty("_BaseColor")) {
            material.SetColor("_BaseColor", new Color(colorRed, colorGreen, colorBlue, transparent)); // for HDRP
        }
        else {
            material.SetColor("_Color", new Color(colorRed, colorGreen, colorBlue, transparent));
        }
        return;
    }


    private static void getParamsFromMaterialName(string name)
    {
        int atr_len = 32;    // MTRL_ATTR_LEN /3*4 (36/3*4 = 32)
        //
        if (name.Length >= atr_len) {
            string sub = name.Substring(name.Length - atr_len, atr_len);
            string enc = sub.Replace('-', '/');
            enc = enc.Replace('$', '/');           // for old version
            //
            if (enc.Length == atr_len) {
                try {
                    byte[] dec = Convert.FromBase64String(enc);    // MTRL_ATTR_LEN (32/4*3 = 24)
                    colorRed    = 1.0f - (float)dec[0] / 255.0f;
                    colorGreen  = 1.0f - (float)dec[1] / 255.0f;
                    colorBlue   = 1.0f - (float)dec[2] / 255.0f;
                    transparent = 1.0f - (float)dec[3] / 255.0f;
                    cutoff      = (float)dec[4] / 255.0f;
                    shininess   = (float)dec[5] / 255.0f;
                    glow        = (float)dec[6] / 255.0f;
                    bright      = (float)dec[7] / 255.0f;
                    light       = (float)dec[8] / 255.0f;
                    /*
                    short[] tmp = new short[1];
                    Buffer.BlockCopy(dec, 13, tmp, 0, 2);
                    shiftU      = (float)tmp[0] / 2000.0f;
                    Buffer.BlockCopy(dec, 15, tmp, 0, 2);
                    shiftV      = (float)tmp[0] / 2000.0f;
                    Buffer.BlockCopy(dec, 17, tmp, 0, 2);
                    scaleU      = (float)tmp[0] / 100.0f;
                    Buffer.BlockCopy(dec, 19, tmp, 0, 2);
                    scaleV      = (float)tmp[0] / 100.0f;
                    Buffer.BlockCopy(dec, 21, tmp, 0, 2);
                    rotate      = (float)tmp[0] / 2000.0f;
                    */
                    kind = (char)dec[23];
                    if (cutoff > 0.9f) cutoff = 0.9f;
                }
                catch {
                    UnityEngine.Debug.Log("SelectOARShader: Base64 Decode Error = " + enc);
                }
            }
        }
    }

}
