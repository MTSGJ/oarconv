using UnityEngine;
using System.Collections;
 
public class Underwater_u5 : MonoBehaviour 
{
    //This script enables underwater effects. Attach to main camera.
 
    //Define variable
    public int underwaterLevel = 0;
    public float waterDensity = 0.02f;
 
    //The scene's default fog settings
    private bool defaultFog;
    private Color defaultFogColor;
    private float defaultFogDensity;
	private Material defaultSkybox;

    void Start () 
    {
	    //Set the background color
        defaultFog = RenderSettings.fog;
        defaultFogColor = RenderSettings.fogColor;
        defaultFogDensity = RenderSettings.fogDensity;
		defaultSkybox = RenderSettings.skybox;
		GetComponent<Camera>().backgroundColor = new Color(0, 0.4f, 0.7f, 1);
    }
 
	//
    void Update () 
    {
        if (transform.position.y < underwaterLevel)
        {
            RenderSettings.fog = true;
            RenderSettings.fogColor = new Color(0, 0.4f, 0.7f, 0.6f);
            RenderSettings.fogDensity = waterDensity;
			RenderSettings.skybox = null;
        }
        else
        {
            RenderSettings.fog = defaultFog;
            RenderSettings.fogColor = defaultFogColor;
            RenderSettings.fogDensity = defaultFogDensity;
			RenderSettings.skybox = defaultSkybox;
        }
    }
}
