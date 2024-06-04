//
// SelectOARShader for oarconv by Fumi.Iseki 2015-2024 (C) v2.1.0
//
// see also https://github.com/MTSGJ/oarconv
//
//

using UnityEngine;
using UnityEditor;
using System;
using System.IO;
using Unity.VisualScripting;


public class SelectOARShader : AssetPostprocessor
{
    private static string GeneralShader;
    private static string TreeShader;
    private static string SpecularShader;
    private static string GlowShader;
    private static string BrightShader;
    private static string TransShader;          // Alpha Blending
    private static string TransCutShader;       // Alpha Cutoff
    private static string TransCutSoftShader;   // Alpha Blending + Cutoff
    private static string TransSpecShader;      // Alpha Blending + Specular
    private static string TransCutSpecShader;   // Alpha Cutoff + Specular

    private static float colorRed      = 0.0f;
    private static float colorGreen    = 0.0f;
    private static float colorBlue     = 0.0f;
    private static float transparent   = 1.0f;  // 不透明
    private static float cutoff        = 0.0f;
    private static float shininess     = 0.0f;
    private static float glow          = 0.0f;
    private static float bright        = 0.0f;
    private static float light         = 0.0f;
    private static int   alphaMode     = 0;     // 0: NONE, 1: BLENDING, 2: MASKING, 3: EMISSIVE
    private static bool  hasAlpha      = false;
    //  private static float shiftU    = 0.0f;
    //  private static float shiftV    = 0.0f;
    //  private static float scaleU    = 1.0f;
    //  private static float scaleV    = 1.0f;
    //  private static float rotate    = 0.0f;
    private static char  kind          = 'O';   // O: Object, T: Tree, G: Grass, E: Earth

    private const string HDRP_Shader   = "HDRP/Lit";
    private const string URP_Shader    = "Universal Render Pipeline/Simple Lit";
    private const string BINP_Shader   = "Legacy Shaders/Diffuse";
    private const string NONE_Shader   = "NONE";

    private const string PhantomFolder = "Phantoms";


    void OnPreprocessModel()
    {
        string currentFolder = Path.GetDirectoryName(assetPath);
        ModelImporter modelImporter = assetImporter as ModelImporter;

        modelImporter.materialImportMode = ModelImporterMaterialImportMode.ImportStandard;
        //modelImporter.materialLocation = ModelImporterMaterialLocation.External;

        modelImporter.addCollider = true;
        if (currentFolder.EndsWith("/" + PhantomFolder) || currentFolder.EndsWith("\\" + PhantomFolder)) {
            modelImporter.addCollider = false;
        }
    }



    static void InitialShader()
    {
        string _generalShader = HDRP_Shader;
        Shader shdr = Shader.Find(_generalShader);
        if (shdr == null) { 
            _generalShader = URP_Shader;
            shdr = Shader.Find(_generalShader);
        }
        if (shdr == null) {
            _generalShader = BINP_Shader;
            shdr = Shader.Find(_generalShader);
        }
        if (shdr == null) {
            GeneralShader = NONE_Shader;
        }
        else {
            GeneralShader = _generalShader;
        }

        //
        if (GeneralShader == HDRP_Shader) {
            TreeShader          = "Unlit/Transparent"; 
            SpecularShader      = "HDRP/Lit";
            //GlowShader        = "HDRP/Lit";
            //BrightShader      = "HDRP/Lit";
            TransShader         = "Unlit/Transparent";                                  // Alpha Blending
            //TransCutShader    = "Unlit/Transparent";                                  // Alpha Cutoff
            //TransCutSoftShader= "Unlit/Transparent";                                  // Alpha Blending + Cutoff
            //TransSpecShader   = "Unlit/Transparent";                                  // Alpha Blending + Specular
            //TransCutSpecShader= "Unlit/Transparent";                                  // Alpha Cutoff + Specular
        }
        else if (GeneralShader == URP_Shader) {
            TreeShader          = "Unlit/Transparent";
            SpecularShader      = "Universal Render Pipeline/Simple Lit";
            //GlowShader        = "Universal Render Pipeline/Simple Lit";
            //BrightShader      = "Universal Render Pipeline/Simple Lit";
            TransShader         = "Unlit/Transparent";                                  // Alpha Blending
            //TransCutShader    = "Unlit/Transparent";                                  // Alpha Cutoff
            //TransCutSoftShader= "Unlit/Transparent";                                  // Alpha Blending + Cutoff
            //TransSpecShader   = "Unlit/Transparent";                                  // Alpha Blending + Specular
            //TransCutSpecShader= "Unlit/Transparent";                                  // Alpha Cutoff + Specular
        }
        else if (GeneralShader == BINP_Shader) {
            TreeShader          = "Legacy Shaders/Transparent/Cutout/Soft Edge Unlit";
            SpecularShader      = "Standard";
            GlowShader          = "Standard";
            BrightShader        = "Legacy Shaders/Self-Illumin/Specular";
            TransShader         = "Legacy Shaders/Transparent/Diffuse";                  // Alpha Blending
            TransCutShader      = "Legacy Shaders/Transparent/Cutout/Diffuse";           // Alpha Cutoff
            TransCutSoftShader  = "Legacy Shaders/Transparent/Cutout/Soft Edge Unlit";   // Alpha Blending + Cutoff
            TransSpecShader     = "Legacy Shaders/Transparent/Specular";                 // Alpha Blending + Specular
            TransCutSpecShader  = "Legacy Shaders/Transparent/Cutout/Specular";          // Alpha Cutoff + Specular
        }

        //Debug.Log("General Shadr = " + GeneralShader);
        return;
    }



    static void OnPostprocessAllAssets(string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromAssetPaths, bool didDomainReload)
    {
        InitialShader();
        if (GeneralShader == NONE_Shader) return;
        //
        foreach (string asset in importedAssets) {
            UnityEngine.Object[] objects = AssetDatabase.LoadAllAssetsAtPath(asset);
            if (objects!=null) {
                foreach (UnityEngine.Object obj in objects) {
                    string objName = obj.name;
                    if (objName.StartsWith("MATERIAL_")) {
                        Material material = obj as Material;
                        //
                        if (GeneralShader == HDRP_Shader) {
                            SetMaterialShader_HDRP(material);
                        }
                        else if (GeneralShader == URP_Shader) {
                            SetMaterialShader_URP(material);
                        }
                        else { 
                            SetMaterialShader_BRP(material);
                        }
                        //
                        EditorUtility.SetDirty(material);
                    }
                }
            }
        }
        AssetDatabase.Refresh();
    }



    static void SetMaterialShader_HDRP(Material material)
    {
        string materialName = material.name;
        getParamsFromMaterialName(materialName);

        // Shader の選択 開始
        material.shader = Shader.Find(GeneralShader);
        if (material.HasProperty("_Smoothness")) material.SetFloat("_Smoothness", 0.0f); // 反射を止める

        if (kind == 'T' || kind == 'G') {   // Tree or Grass
            material.shader = Shader.Find(TreeShader);
        }
        // Alpha Channell
        else if (hasAlpha) {
            material.shader = Shader.Find(TransShader);
            if (cutoff>0.0f) {
                if (material.HasProperty("_AlphaCutoffEnable")) material.SetFloat("_AlphaCutoffEnable", 1.0f);
                if (material.HasProperty("_AlphaCutoff")) material.SetFloat("_AlphaCutoff", cutoff);
            }
        }
        // Shininess
        else if (shininess > 0.01f) {
            material.shader = Shader.Find(SpecularShader);
        }
        // Shader の選択終わり

        // Color
        if (material.HasProperty("_BaseColor")) {
            material.SetColor("_BaseColor", new Color(colorRed, colorGreen, colorBlue, transparent));
        }
        else {
            material.SetColor("_Color", new Color(colorRed, colorGreen, colorBlue, transparent));
        }

        // Shininess
        if (shininess > 0.01f) {
            if (material.HasProperty("_Smoothness")) material.SetFloat("_Smoothness", 0.5f);
            if (material.HasProperty("_Metallic")) material.SetFloat("_Metallic", shininess * 3.0f);
        }

        /*
        if (material.HasProperty("_EmissiveColor")) {
            //material.EnableKeyword("_EmissiveIntensity");
            Color col = material.GetColor("_Color");
            float fac = col.maxColorComponent;
            if (fac > 0.01f) {
                if (glow > 0.99f) glow = 0.99f;
                col = col * (glow / fac);
            }
            material.SetColor("_EmissiveColor", col);
            material.SetFloat("_EmissiveIntensity", 1.0f);
        }*/

        return;
    }



    static void SetMaterialShader_URP(Material material)
    {
        string materialName = material.name;
        getParamsFromMaterialName(materialName);

        // Shader の選択 開始
        material.shader = Shader.Find(GeneralShader);

        if (kind == 'T' || kind == 'G') {   // Tree or Grass
            material.shader = Shader.Find(TreeShader);
        }
        // Alpha Channell
        else if (hasAlpha) {
            material.shader = Shader.Find(TransShader);
            if (cutoff > 0.0f) {
                if (material.HasProperty("_AlphaCutoffEnable")) material.SetFloat("_AlphaCutoffEnable", 1.0f);
                if (material.HasProperty("_AlphaCutoff")) material.SetFloat("_AlphaCutoff", cutoff);
            }
        }
        // Shininess
        else if (shininess > 0.01f) {
            material.shader = Shader.Find(SpecularShader);
        }
        // Shader の選択終わり

        //
        // Color
        if (material.HasProperty("_BaseColor")) {
            material.SetColor("_BaseColor", new Color(colorRed, colorGreen, colorBlue, transparent));
        }
        else {
            material.SetColor("_Color", new Color(colorRed, colorGreen, colorBlue, transparent));
        }

        // Shininess
        if (shininess > 0.01f) {
            if (material.HasProperty("_Metallic")) material.SetFloat("_Metallic", shininess * 3.0f);
        }
        
        return;
    }



    static void SetMaterialShader_BRP(Material material)
    {
        string materialName = material.name;
        getParamsFromMaterialName(materialName);

        material.shader = Shader.Find(GeneralShader);

        if (kind == 'T' || kind == 'G') {   // Tree or Grass
            material.shader = Shader.Find(TreeShader);
        }
        //
        else if (hasAlpha) {
            if (shininess > 0.01f) {
                if (cutoff > 0.01f) {   // Alpha Cutoff
                    material.shader = Shader.Find(TransCutSpecShader);
                    if (material.HasProperty("_Cutoff")) material.SetFloat("_Cutoff", cutoff);
                }
                else {   // Alpha Blending
                    if (!hasAlpha) {
                        material.shader = Shader.Find(TransCutSpecShader);
                        if (material.HasProperty("_Cutoff")) material.SetFloat("_Cutoff", 0.2f);
                    }
                    else {
                        material.shader = Shader.Find(TransSpecShader);
                    }
                }
                if (material.HasProperty("_Metallic")) material.SetFloat("_Metallic", shininess / 2.0f);
            }
            //
            else {
                if (cutoff > 0.01f) {   // Alpha Cutoff
                    material.shader = Shader.Find(TransCutShader);
                    if (material.HasProperty("_Cutoff")) material.SetFloat("_Cutoff", cutoff);
                }
                else {   // Alpha Blending
                    if (!hasAlpha) {
                        material.shader = Shader.Find(TransCutSoftShader);
                        if (material.HasProperty("_Cutoff")) material.SetFloat("_Cutoff", 0.9f);
                    }
                    else {
                        material.shader = Shader.Find(TransShader);
                        if (material.HasProperty("_Cutoff")) material.SetFloat("_Cutoff", cutoff);
                    }
                }
            }
        }
        else if (transparent<0.9f) {
            material.shader = Shader.Find(TransShader);
            if (material.HasProperty("_Cutoff")) material.SetFloat("_Cutoff", cutoff);
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

        // Color
        if (material.HasProperty("_BaseColor")) {
            material.SetColor("_BaseColor", new Color(colorRed, colorGreen, colorBlue, transparent));
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
                    alphaMode   = (int)dec[9]/10;           // 0: NONE, 1: BLENDING, 2: MASKING, 3: EMISSIVE
                    int has_alp = (int)dec[9]%10;           // 0: No, 1: YES
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
                    if (has_alp == 1) hasAlpha = true;
                    else              hasAlpha = false;
                }
                catch {
                    UnityEngine.Debug.Log("SelectOARShader: Base64 Decode Error = " + enc);
                }
            }
        }
    }

}
