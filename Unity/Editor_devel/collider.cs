using UnityEngine;
using UnityEditor;
using UnityEditor.AssetImporters;
using System.IO;

public class AddCollidersPostProcessor : AssetPostprocessor
{
    static void OnPostprocessAllAssets(string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromAssetPaths)
    {
        foreach (string assetPath in importedAssets) {
            if (assetPath.EndsWith(".gltf") || assetPath.EndsWith(".glb")) {
                // GameObjectをロードする
                GameObject importedObject = AssetDatabase.LoadAssetAtPath<GameObject>(assetPath);
                if (importedObject != null) {
                    Debug.Log($"Loaded GameObject: {importedObject.name} from path: {assetPath}");
                    // インスタンスを作成してコライダーを追加
                    GameObject instance = (GameObject)PrefabUtility.InstantiatePrefab(importedObject);
                    if (instance != null) {
                        AddColliderAndRigidbody(instance);
                    }
                    else {
                        Debug.LogError($"Failed to instantiate prefab for: {importedObject.name}");
                    }
                }
                else {
                    Debug.LogError($"Failed to load GameObject from path: {assetPath}");
                }
            }
        }
    }

    static void AddColliderAndRigidbody(GameObject obj)
    {
        if (obj == null) return;

        MeshFilter meshFilter = obj.GetComponent<MeshFilter>();
        if (meshFilter != null) {
            if (meshFilter.sharedMesh == null) {
                Debug.LogWarning($"MeshFilter on {obj.name} has no sharedMesh.");
            }
            else {
                bool meshColliderAdded = false;

                try {
                    MeshCollider meshCollider = obj.AddComponent<MeshCollider>();
                    meshCollider.sharedMesh = meshFilter.sharedMesh;
                    meshColliderAdded = true;

                    // デバッグ用のメッセージ
                    Debug.Log($"Added MeshCollider to {obj.name}");
                }
                catch (System.Exception e) {
                    Debug.LogError($"Failed to add MeshCollider to {obj.name}: {e.Message}");
                }

                // MeshCollider の追加に失敗した場合、代替コライダーを追加
                if (!meshColliderAdded) {
                    BoxCollider boxCollider = obj.AddComponent<BoxCollider>();
                    Debug.LogWarning($"Added BoxCollider to {obj.name} as a fallback");
                }

                // Rigidbody コンポーネントの追加
                if (obj.GetComponent<Rigidbody>() == null) {
                    Rigidbody rb = obj.AddComponent<Rigidbody>();
                    if (rb !=NULL) rb.isKinematic = true; // オブジェクトが静止している場合
                }
            }
        }
        else {
            Debug.Log($"No MeshFilter found on {obj.name}");
        }
    }
}