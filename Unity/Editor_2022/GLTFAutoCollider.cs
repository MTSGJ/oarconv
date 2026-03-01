//
// GLTFAutoCollider by Fumi.Iseki 2024 (C) v1.1.1
//
// see also https://github.com/MTSGJ/oarconv
//
// 2024/09/14
//

using System.Collections.Generic;
using System.Linq;
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
        GameObject[] allObjects = Object.FindObjectsByType<GameObject>(FindObjectsSortMode.None);
        foreach (GameObject obj in allObjects) {
            Transform parent = obj.transform.parent;
            if (obj.GetComponent<Collider>()==null && parent!=null) {
                if (obj.scene.isLoaded && PrefabUtility.GetPrefabInstanceStatus(obj)!=PrefabInstanceStatus.NotAPrefab
                                       && obj.GetComponent<SkinnedMeshRenderer>()==null) {
                    string assetPath = GetAssetPath(obj);
                    if (assetPath.Contains("/Solids/") || assetPath.Contains("/Terrains/")) {
                        AddMeshCollider(obj);
                    }
                }
            }
        }
    }
     

    private static void AddMeshCollider(GameObject obj)
    {
        MeshFilter meshFilter = obj.GetComponent<MeshFilter>();
        if (meshFilter!=null) {
            UnityEngine.Mesh mesh = meshFilter.sharedMesh;
            if (IsValidMesh(mesh)) {
                MeshCollider meshCollider = obj.AddComponent<MeshCollider>();
                if (meshCollider!=null) {
                    meshCollider.sharedMesh = meshFilter.sharedMesh;
                    //meshCollider.convex = true;
                    //Debug.Log("MeshCollider added to" + obj.name);
                }
            }
        }
        return;
    }


    private static bool IsValidMesh(UnityEngine.Mesh mesh)
    {
        if (mesh==null) {
            //Debug.Log("Mesh is null.");
            return false;
        }
        if (mesh.normals.Length==0) {
            //Debug.Log("Mesh has no normals.");
            return false;
        }
        if (mesh.vertexCount==0) {
            //Debug.Log("Mesh has no vertices.");
            return false;
        }
        if (mesh.triangles.Length == 0) {
            //Debug.Log("Mesh has no triangles.");
            return false;
        }
        int[] triangles = mesh.triangles;
        if (!triangles.All(index=>index>=0 && index<mesh.vertexCount)) {
            //Debug.Log("Mesh has invarid triangle.");
            return false;
        }
        // Bounds Size
        int count = 0;
        if (mesh.bounds.size.x<0.0001f) count++;
        if (mesh.bounds.size.y<0.0001f) count++;
        if (mesh.bounds.size.z<0.0010f) count++;
        if (count>=2) {
            //Debug.Log("Mesh has small bounds size " + mesh.bounds.size + ".");
            return false;
        }
        /**/
        // Unique coordinates
        Vector3[] vertices = mesh.vertices;
        HashSet<Vector3> uniqueVertices = new HashSet<Vector3>();
        foreach (Vector3 vertex in vertices) {
            uniqueVertices.Add(vertex);
            if (uniqueVertices.Count>=3) return true;
        }
        //Debug.Log("Mesh has no more than 3 unique coordinates");
        return false;
        /*
        // Unique coordinates
        Vector3[] vertices = mesh.vertices;
        HashSet<Vector3> uniqueVertices = new HashSet<Vector3>();
        foreach (Vector3 vertex in vertices) uniqueVertices.Add(vertex);
        if (uniqueVertices.Count<3) {
            //Debug.Log("Mesh has no more than 3 unique coordinates");
            return false;
        }
        return true;
        */
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
