//
// GLTFAutoCollider by Fumi.Iseki 2015-2024 (C) v1.0.0
//
// see also https://github.com/MTSGJ/oarconv
//
// 2024/09/11
//

using UnityEditor;
using UnityEngine;


[InitializeOnLoad]
public class GLTFAutoCollider
{
    static GLTFAutoCollider()
    {
        EditorApplication.hierarchyChanged += OnHierarchyChanged;
    }

    private static void OnHierarchyChanged()
    {
        string unityVersion = Application.unityVersion;
        int majorVersion = int.Parse(unityVersion.Split('.')[0]);
        
        GameObject[] allObjects = Object.FindObjectsByType<GameObject>(FindObjectsSortMode.None);
        foreach (GameObject obj in allObjects) {
            if (obj.scene.isLoaded && PrefabUtility.GetPrefabInstanceStatus(obj)!=PrefabInstanceStatus.NotAPrefab) {
                string assetPath = GetAssetPath(obj);
                if (assetPath.Contains("/Solids/")) {
                    AddBoxColliderToObject(obj, majorVersion);
                }
                else if (assetPath.Contains("/Terrains/")) {
                    AddMeshColliderToObject(obj, majorVersion);
                }
            }
        }
    }


    private static void AddBoxColliderToObject(GameObject obj, int version)
    {
        Transform parent = obj.transform.parent;

        bool isAddCollider = (obj.GetComponent<Collider>()==null) && (parent!=null);
        if (version<=5) isAddCollider = isAddCollider && (parent.transform.parent==null);
        if (isAddCollider) {
            obj.AddComponent<BoxCollider>();
        	MeshFilter meshFilter = obj.GetComponent<MeshFilter>();
        }
        return;
    }


    private static void AddMeshColliderToObject(GameObject obj, int version)
    {
        Transform parent = obj.transform.parent;

        bool isAddCollider = (obj.GetComponent<Collider>()==null) && (parent!=null);
        if (version<=5) isAddCollider = isAddCollider && (parent.transform.parent==null);
        if (isAddCollider) {
        	MeshFilter meshFilter = obj.GetComponent<MeshFilter>();
            if (meshFilter!=null) {
                try {
                    MeshCollider meshCollider = obj.AddComponent<MeshCollider>();
                    if (meshCollider!=null) {
                        meshCollider.sharedMesh = meshFilter.sharedMesh;
                        //meshCollider.convex = true;
                        //Debug.Log("MeshCollider added to " + obj.name);
                    }
                }
                catch (System.Exception e) {
                    obj.AddComponent<BoxCollider>();
                }
            }
            else {
                obj.AddComponent<BoxCollider>();
                //BoxCollider boxCollider = obj.AddComponent<BoxCollider>();
                //if (boxCollider!=null) Debug.Log("BoxCollider added to " + obj.name);
            }
        }
        return;
    }

    private static string GetAssetPath(GameObject obj)
    {
        string ret = "";
        GameObject prefab = PrefabUtility.GetCorrespondingObjectFromSource(obj);
        if (prefab!=null) ret = AssetDatabase.GetAssetPath(prefab);
        else              ret = AssetDatabase.GetAssetPath(obj);
        return ret;
    }
}
