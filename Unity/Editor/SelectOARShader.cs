//
// SelectOARShader for oarconv by Fumi.Iseki 2015-2024 (C) v1.7.4
//
// see also https://github.com/MTSGJ/oarconv
//
//

using UnityEngine;
using UnityEditor;
using System;
using System.IO;



public sealed class SelectOARShader : AssetPostprocessor
{
    private string GeneralShader;
    private string TreeShader;
    private string EarthShader;
    private string TransShader;          // Alpha Blending
    private string TransCutShader;       // Alpha Cutoff

    private const string MaterialFolder = "Materials";
    private const string PhantomFolder  = "Phantoms";

    private float colorRed      = 0.0f;
    private float colorGreen    = 0.0f;
    private float colorBlue     = 0.0f;
    private float transparent   = 1.0f;  //
    private float cutoff        = 0.0f;
    private float shininess     = 0.0f;
    private float glow          = 0.0f;
    private float bright        = 0.0f;
    private float light         = 0.0f;
    private int   alphaMode     = 0;     // 0: NONE, 1: BLENDING, 2: MASKING, 3: EMISSIVE
    private bool  hasAlpha      = false;
//  private float shiftU        = 0.0f;
//  private float shiftV        = 0.0f;
//  private float scaleU        = 1.0f;
//  private float scaleV        = 1.0f;
//  private float rotate        = 0.0f;
    private char  kind          = 'O';   // O: Object, T: Tree, G: Grass, E: Earth

    private const string HDRP_Shader   = "HDRP/Lit";
    //private const string URP_Shader    = "Universal Render Pipeline/Simple Lit";
    private const string URP_Shader    = "Universal Render Pipeline/Lit";
    private const string BINP_Shader   = "Standard";
    private const string NONE_Shader   = "NONE";

    private bool createdMaterialFolder = false;


    public SelectOARShader() : base()
    {
        InitShader();
    }



    void OnPreprocessModel()
    {
        //Debug.Log("Unity Version = " + UnityEngine.Application.unityVersion);
        string currentFolder = Path.GetDirectoryName(assetPath);
        if (!createdMaterialFolder && !AssetDatabase.IsValidFolder(currentFolder + "\\" + MaterialFolder)) {
            AssetDatabase.CreateFolder(currentFolder, MaterialFolder);
            createdMaterialFolder = true;
        }
        ModelImporter modelImporter = assetImporter as ModelImporter;

        modelImporter.materialImportMode = ModelImporterMaterialImportMode.ImportStandard;
        modelImporter.materialLocation = ModelImporterMaterialLocation.External;

        /*
        if (!modelImporter.importSettingsMissing) {
        }
        modelImporter.animationType = ModelImporterAnimationType.None;
        */

        modelImporter.addCollider = true;
        if (currentFolder.EndsWith("/" + PhantomFolder) || currentFolder.EndsWith("\\" + PhantomFolder)) {
            modelImporter.addCollider = false;
        }
    }


    void InitShader()
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
            EarthShader         = "HDRP/Lit";
            TransShader         = "Unlit/Transparent";                                  // Alpha Blending
            TransCutShader      = "Unlit/Transparent";                                  // Alpha Cutoff
        }
        else if (GeneralShader == URP_Shader) {
            TreeShader          = "Unlit/Transparent";
            EarthShader         = "Universal Render Pipeline/Simple Lit";
            TransShader         = "Unlit/Transparent";                                  // Alpha Blending
            TransCutShader      = "Unlit/Transparent";                                  // Alpha Cutoff
        }
        else if (GeneralShader == BINP_Shader) {
            TreeShader          = "Legacy Shaders/Transparent/Cutout/Soft Edge Unlit";
            EarthShader         = "Standard";
            TransShader         = "Unlit/Transparent";                                   // Alpha Blending
            TransCutShader      = "Legacy Shaders/Transparent/Cutout/Diffuse";           // Alpha Cutoff
        }

        //Debug.Log("General Shadr = " + GeneralShader);
        return;
    }



    Material OnAssignMaterialModel(Material material, Renderer renderer)
    {
        //Debug.Log("Renderer = " + renderer.name);
        string materialName = material.name;
        string currentFolder = Path.GetDirectoryName(assetPath);
        string materialPath = string.Format("{0}\\{1}\\{2}.mat", currentFolder, MaterialFolder, materialName);

        Material mt = AssetDatabase.LoadAssetAtPath<Material>(materialPath);
        if (mt != null) {
            material = mt;
            return material;
        }
        
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
        AssetDatabase.CreateAsset(material, materialPath);

        return material;
    }


    /**
     * High Definition Render Pipeline (HDRP)
     */
    void SetMaterialShader_HDRP(Material material)
    {
        string materialName = material.name;
        getParamsFromMaterialName(materialName);

        material.shader = Shader.Find(GeneralShader);

        if (kind == 'T' || kind == 'G') {       // Tree or Grass
            material.shader = Shader.Find(TreeShader);
        }
        else if (kind == 'E') {                 // Terrain
            material.shader = Shader.Find(EarthShader);
            if (material.HasProperty("_Smoothness")) material.SetFloat("_Smoothness", 0.0f); // Stop the reflection
        }
        //
        else if (alphaMode == 0) {              // Alpha NONE
            material.shader = Shader.Find(GeneralShader);
        }
        //
        else if (alphaMode == 1) {              // Alpha BLENDING
            if (hasAlpha) {
                material.shader = Shader.Find(TransShader);
            }
            else if (transparent < 0.9f) {
                material.shader = Shader.Find(GeneralShader);
                material.SetFloat("_SurfaceType", 1.0f);
                material.SetFloat("_BlendMode", 1.0f);
                // No Specular for full transparency
                if (transparent<0.01f) material.SetFloat("_EnableBlendModePreserveSpecularLighting", 0.0f);
            }
        }
        //
        else if (alphaMode == 2 && hasAlpha) {  // Alpha MASKING (Cutoff)
            material.shader = Shader.Find(TransCutShader);
            if (material.HasProperty("_Cutoff")) {
                if (cutoff > 0.0f) material.SetFloat("_Cutoff", cutoff);
                else               material.SetFloat("_Cutoff", 0.9f);
            }
        }
        //
        else if (alphaMode == 3 && hasAlpha){  // Alpha EMISSIVE (not supported)
            //
        }

        //
        if (glow > 0.0f || bright > 0.0f) {
            float emission = Math.Max(glow, bright);
            //
            material.EnableKeyword("_EMISSION");
            if (material.HasProperty("_EmissionColor")) {
                Color col = material.GetColor("_Color");
                float fac = col.maxColorComponent;
                if (fac > 0.01f) {
                    col = col * (emission / fac);
                }
                material.SetColor("_EmissionColor", col);
            }
        }
        //
        if (shininess > 0.0f) {
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


    /**
     * Universal Render Pipeline (URP)
     */
    void SetMaterialShader_URP(Material material)
    {
        string materialName = material.name;
        getParamsFromMaterialName(materialName);

        material.shader = Shader.Find(GeneralShader);

        if (kind == 'T' || kind == 'G') {       // Tree or Grass
            material.shader = Shader.Find(TreeShader);
        }
        else if (kind == 'E') {                 // Terrain
            material.shader = Shader.Find(EarthShader);
        }
        //
        else if (alphaMode == 0) {              // Alpha NONE
            material.shader = Shader.Find(GeneralShader);
        }
        //
        else if (alphaMode == 1) {              // Alpha BLENDING
            if (hasAlpha) {
                material.shader = Shader.Find(TransShader);
            }
            else if (transparent<0.9f) {
                material.shader = Shader.Find(GeneralShader);
                material.SetFloat("_Surface", 1.0f);
                material.SetOverrideTag("RenderType", "Transparent");
            }
        }
        //
        else if (alphaMode == 2 && hasAlpha) {  // Alpha MASKING (Cutoff)
            material.shader = Shader.Find(TransCutShader);
            if (material.HasProperty("_Cutoff")) {
                if (cutoff > 0.0f) material.SetFloat("_Cutoff", cutoff);
                else               material.SetFloat("_Cutoff", 0.9f);
            }
        }
        //
        else if (alphaMode == 3 && hasAlpha) {  // Alpha EMISSIVE (not supported)
            //
        }

        //
        if (glow > 0.0f || bright > 0.0f) {
            float emission = Math.Max(glow, bright);
            //
            material.EnableKeyword("_EMISSION");
            if (material.HasProperty("_EmissionColor")) {
                Color col = material.GetColor("_Color");
                float fac = col.maxColorComponent;
                if (fac > 0.01f) {
                    col = col * (emission / fac);
                }
                material.SetColor("_EmissionColor", col);
            }
        }
        //
        if (shininess > 0.0f) {
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


    /**
     * Bult-in Render Pioeline  (BRP)
     */
    void SetMaterialShader_BRP(Material material)
    {
        string materialName = material.name;
        getParamsFromMaterialName(materialName);

        material.shader = Shader.Find(GeneralShader);

        if (kind == 'T' || kind == 'G') {       // Tree or Grass
            material.shader = Shader.Find(TreeShader);
        }
        else if (kind == 'E') {                 // Terrain
            material.shader = Shader.Find(EarthShader);
        }
        //
        else if (alphaMode == 0) {              // Alpha NONE
            material.shader = Shader.Find(GeneralShader);
        }
        //
        else if (alphaMode == 1 && hasAlpha) {  // Alpha BLENDING
            material.shader = Shader.Find(TransShader);
        }
        //
        else if (alphaMode == 2 && hasAlpha) {  // Alpha MASKING (Cutoff)
            material.shader = Shader.Find(TransCutShader);
            if (material.HasProperty("_Cutoff")) {
                if (cutoff > 0.0f) material.SetFloat("_Cutoff", cutoff);
                else               material.SetFloat("_Cutoff", 0.9f);
            }
        }
        //
        else if (alphaMode == 3 && hasAlpha) {  // Alpha EMISSIVE (not supported)
            //
        }   

        //
        if (glow > 0.0f || bright > 0.0f) {
            float emission = Math.Max(glow, bright);
            //
            material.EnableKeyword("_EMISSION");
            if (material.HasProperty("_EmissionColor")) {
                Color col = material.GetColor("_Color");
                float fac = col.maxColorComponent;
                if (fac > 0.01f) {
                    col = col * (emission / fac);
                }
                material.SetColor("_EmissionColor", col);
            }
        }
        //
        if (shininess > 0.0f) {
            //if (material.HasProperty("_Shininess")) material.SetFloat("_Shininess", shininess);
            //if (material.HasProperty("_Metallic")) material.SetFloat("_Metallic", shininess / 2.0f);
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



    private void getParamsFromMaterialName(string name)
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
