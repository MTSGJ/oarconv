//
// GLTFAutoCollider by Fumi.Iseki 2015-2024 (C) v0.9.0
//
// see also https://github.com/MTSGJ/oarconv
//
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
        GameObject[] allObjects = Object.FindObjectsOfType<GameObject>();
        foreach (GameObject obj in allObjects) {
            string assetPath = GetAssetPath(obj);
            if (!assetPath.Contains("/Phantoms/")) {
                if (obj.scene.isLoaded && PrefabUtility.GetPrefabInstanceStatus(obj)!=PrefabInstanceStatus.NotAPrefab) {
                    AddColliderToObject(obj);
                }
            }
        }
    }

    private static void AddColliderToObject(GameObject obj)
    {
        Transform parent = obj.transform.parent;
        if (obj.GetComponent<Collider>()==null && parent!=null && parent.transform.parent==null) {
            MeshCollider meshCollider = obj.AddComponent<MeshCollider>();
            if (meshCollider!=null) {
                MeshFilter meshFilter = obj.GetComponent<MeshFilter>();
                if (meshFilter != null) {
                    meshCollider.sharedMesh = meshFilter.sharedMesh;
                    //meshCollider.convex = true;
                    //Debug.Log("MeshCollider added to " + obj.name);
                }
            }
            else {
                BoxCollider boxCollider = obj.AddComponent<BoxCollider>();
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
