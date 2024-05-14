//
// Mecanimのアニメーションデータが、原点で移動しない場合の Rigidbody付きコントローラ
// サンプル
// 2014/03/13 N.Kobyasahi
//

// Add Flying by Fumi.Iseki 
// 2015/05/01 
// 2015/07/11   for OARConvWin-1.1
// 2023/12/17   for OARConvWin-1.2
//

using UnityEngine;
using System.Collections;

namespace UnityChan
{
    // 必要なコンポーネントの列記
    [RequireComponent(typeof(Animator))]
    [RequireComponent(typeof(CapsuleCollider))]
    [RequireComponent(typeof(Rigidbody))]

    public class UnityChanControlScriptWithRgidBody : MonoBehaviour
    {
        public bool showInteraction = true;         // 初期メニュー表示
        public float animSpeed = 1.5f;              // アニメーション再生速度設定
        public float lookSmoother = 3.0f;           // a smoothing setting for camera motion
        public bool useCurves = true;               // Mecanimでカーブ調整を使うか設定する
                                                    // このスイッチが入っていないとカーブは使われない
        public float useCurvesHeight = 0.5f;        // カーブ補正の有効高さ（地面をすり抜けやすい時には大きくする）

        // 以下キャラクターコントローラ用パラメタ
        public float flyingSpeed = 10.0f;   // 飛行速度
        public float runningSpeed = 5.0f;   // 走る速度
        public float walkSpeed = 2.5f;      // 前進速度（歩き）
        public float backwardSpeed = 2.0f;  // 後退速度
        public float rotateSpeed = 0.8f;    // 旋回速度
        public float jumpPower = 3.0f;      // ジャンプ威力
        // キャラクターコントローラ（カプセルコライダ）の参照
        private CapsuleCollider col;
        private Rigidbody rb;
        // キャラクターコントローラ（カプセルコライダ）の移動量
        private Vector3 velocity;
        // CapsuleColliderで設定されているコライダのHeiht、Centerの初期値を収める変数
        private float orgColHight;
        private Vector3 orgVectColCenter;
        private Animator anim;                          // キャラにアタッチされるアニメーターへの参照
        private AnimatorStateInfo currentBaseState;     // base layerで使われる、アニメーターの現在の状態の参照

        private GameObject cameraObject;    // メインカメラへの参照

        // アニメーター各ステートへの参照
        static int idleState = Animator.StringToHash("Base Layer.Idle");
        static int locoState = Animator.StringToHash("Base Layer.Locomotion");
        static int jumpState = Animator.StringToHash("Base Layer.Jump");
        static int restState = Animator.StringToHash("Base Layer.Rest");

        /*
         * by Fumi.Iseki
         */
        private int flying = 0;         // 0: Land, 1: Flaying
        private int forward = 0;        // 0: Stop, 1: Forwarding, 2:Running
        private uint keycnt = 0;

        private bool uparrowKey = false;
        private bool homeKey = false;
        private bool jumpKey = false;


        // 初期化
        void Start()
        {
            // Animatorコンポーネントを取得する
            anim = GetComponent<Animator>();
            // CapsuleColliderコンポーネントを取得する（カプセル型コリジョン）
            col = GetComponent<CapsuleCollider>();
            rb = GetComponent<Rigidbody>();
            //メインカメラを取得する
            cameraObject = GameObject.FindWithTag("MainCamera");
            // CapsuleColliderコンポーネントのHeight、Centerの初期値を保存する
            orgColHight = col.height;
            orgVectColCenter = col.center;
        }


        /*
         * by Fumi.Iseki
         */
        void Update()
        {
            if (Input.GetKeyDown(KeyCode.UpArrow)) uparrowKey = true;
            if (Input.GetKeyDown(KeyCode.Home)) homeKey = true;
            if (Input.GetKeyDown(KeyCode.Space)) jumpKey = true;
        }


        // 以下、メイン処理.リジッドボディと絡めるので、FixedUpdate内で処理を行う.
        void FixedUpdate()
        {
            float h = Input.GetAxis("Horizontal");              // 入力デバイスの水平軸をhで定義
            float v = Input.GetAxis("Vertical");                // 入力デバイスの垂直軸をvで定義

            anim.SetFloat("Speed", v);                          // Animator側で設定している"Speed"パラメタにvを渡す
            anim.SetFloat("Direction", h);                      // Animator側で設定している"Direction"パラメタにhを渡す
            anim.speed = animSpeed;                             // Animatorのモーション再生速度に animSpeedを設定する
            currentBaseState = anim.GetCurrentAnimatorStateInfo(0); // 参照用のステート変数にBase Layer (0)の現在のステートを設定する
            //rb.useGravity = true;                                //ジャンプ中に重力を切るので、それ以外は重力の影響を受けるようにする 

            /* 
             * by Fumi.Iseki
             */
            if (flying == 0)
            {
                //rb.useGravity = false;
                if (uparrowKey)
                {
                    if (keycnt <= 20) {
                        forward = 2;
                        anim.SetBool("Running", true);
                        anim.SetBool("Walking", false);
                    }
                    uparrowKey = false;
                    keycnt = 0;
                }
                //
                else
                {
                    if (forward == 0 && v > 0.1f)
                    {
                        //Debug.Log("Walk!");
                        forward = 1;
                        anim.SetBool("Walking", true);
                        anim.SetBool("Running", false);
                    }
                    //
                    else if ((forward == 1 || forward == 2) && v <= 0.1f)
                    {
                        //Debug.Log("Stop!");
                        forward = 0;
                        anim.SetBool("Running", false);
                        anim.SetBool("Walking", false);
                    }
                }

                // to Fly
                if (homeKey && forward == 0)
                {
                    flying = 1;
                    rb.useGravity = false;
                    rb.velocity = new Vector3(0, 0, 0);
                    transform.localPosition += Vector3.up * 0.2f;
                    anim.SetBool("Hovering", true);
                    homeKey = false;
                }

                if (Input.GetKey(KeyCode.PageUp))
                {
                    flying = 1;
                    forward = 0;
                    rb.useGravity = false;
                    rb.velocity = new Vector3(0, 0, 0);
                    transform.localPosition += Vector3.up * 0.2f;
                    anim.SetBool("Walking", false);
                    anim.SetBool("Running", false);
                    anim.SetBool("Up", true);
                    anim.SetBool("Hovering", true);
                    v = 0.0f;
                }
                //
                if (jumpKey)
                {  // Jump は元のコードにお任せ
                    if (forward == 2)
                    {
                        if (!anim.IsInTransition(0))
                        {
                            anim.SetBool("Jump", true);
                        }
                    }
                    else if (forward == 0)
                    {
                        if (!anim.IsInTransition(0))
                        {
                            anim.SetBool("Rest", true);
                        }
                    }
                    jumpKey = false;
                }
                else {
                    anim.SetBool("Rest", false);
                }
            }
            //
            else if (flying == 1)
            {
                anim.SetBool("Up", false);
                anim.SetBool("Down", false);
                //
                if (homeKey && forward == 0)
                {
                    flying = 0;
                    anim.SetBool("Hovering", false);
                    rb.useGravity = true;
                    homeKey = false;
                    v = 0.0f;
                }
                //
                if (Input.GetKey(KeyCode.PageUp))
                {
                    transform.localPosition += Vector3.up * Time.fixedDeltaTime * 3.5f;
                    anim.SetBool("Up", true);
                    v = 0.0f;
                }
                //
                else if (Input.GetKey(KeyCode.PageDown))
                {
                    anim.SetBool("Down", true);
                    transform.localPosition += Vector3.down * Time.fixedDeltaTime * 3.5f;
                    v = 0.0f;
                }
            }

            // 以下、キャラクターの移動処理
            velocity = new Vector3(0, 0, v);        // 上下のキー入力からZ軸方向の移動量を取得
            // キャラクターのローカル空間での方向に変換
            velocity = transform.TransformDirection(velocity);

            if (v > 0.1)
            {
                if (flying == 1)
                {
                    velocity *= flyingSpeed;
                }
                else if (forward == 1)
                {
                    velocity *= walkSpeed;
                }
                else if (forward == 2)
                {
                    velocity *= runningSpeed;
                }
            }
            else if (v < -0.1)
            {
                velocity *= backwardSpeed;
            }

            if (Input.GetButtonDown("Jump")) {   // スペースキーを入力したら
                //アニメーションのステートがLocomotionの最中のみジャンプできる
                if (currentBaseState.fullPathHash == locoState) {
                    //ステート遷移中でなかったらジャンプできる
                    if (!anim.IsInTransition(0)) {
                        rb.AddForce(Vector3.up * jumpPower, ForceMode.VelocityChange);
                        anim.SetBool("Jump", true);     // Animatorにジャンプに切り替えるフラグを送る
                    }
                }
            }


            // 上下のキー入力でキャラクターを移動させる
            transform.localPosition += velocity * Time.fixedDeltaTime;

            // 左右のキー入力でキャラクタをY軸で旋回させる
            transform.Rotate(0, h * rotateSpeed, 0);

            // 以下、Animatorの各ステート中での処理
            // Locomotion中
            // 現在のベースレイヤーがlocoStateの時
            if (currentBaseState.fullPathHash == locoState)
            {
                //カーブでコライダ調整をしている時は、念のためにリセットする
                if (useCurves)
                {
                    resetCollider();
                }
            }

            // JUMP中の処理
            // 現在のベースレイヤーがjumpStateの時
            else if (currentBaseState.fullPathHash == jumpState)
            {
                cameraObject.SendMessage("setCameraPositionJumpView");  // ジャンプ中のカメラに変更
                                                                        // ステートがトランジション中でない場合
                if (!anim.IsInTransition(0))
                {
                    // 以下、カーブ調整をする場合の処理
                    if (useCurves)
                    {
                        // 以下JUMP00アニメーションについているカーブJumpHeightとGravityControl
                        // JumpHeight:JUMP00でのジャンプの高さ（0〜1）
                        // GravityControl:1⇒ジャンプ中（重力無効）、0⇒重力有効
                        float jumpHeight = anim.GetFloat("JumpHeight");
                        float gravityControl = anim.GetFloat("GravityControl");
                        if (gravityControl > 0)
                            rb.useGravity = false;  //ジャンプ中の重力の影響を切る

                        // レイキャストをキャラクターのセンターから落とす
                        Ray ray = new Ray(transform.position + Vector3.up, -Vector3.up);
                        RaycastHit hitInfo = new RaycastHit();
                        // 高さが useCurvesHeight 以上ある時のみ、コライダーの高さと中心をJUMP00アニメーションについているカーブで調整する
                        if (Physics.Raycast(ray, out hitInfo))
                        {
                            if (hitInfo.distance > useCurvesHeight)
                            {
                                col.height = orgColHight - jumpHeight;      // 調整されたコライダーの高さ
                                float adjCenterY = orgVectColCenter.y + jumpHeight;
                                col.center = new Vector3(0, adjCenterY, 0); // 調整されたコライダーのセンター
                            }
                            else
                            {
                                // 閾値よりも低い時には初期値に戻す（念のため）                    
                                resetCollider();
                            }
                        }
                        rb.useGravity = true;                               //ジャンプ中に重力を切るので、それ以外は重力の影響を受けるようにする
                    }
                    // Jump bool値をリセットする（ループしないようにする）                
                    anim.SetBool("Jump", false);
                }
            }

            // IDLE中の処理
            // 現在のベースレイヤーがidleStateの時
            else if (currentBaseState.fullPathHash == idleState) {
                //カーブでコライダ調整をしている時は、念のためにリセットする
                if (useCurves) {
                    resetCollider ();
                }
                // スペースキーを入力したらRest状態になる
                if (Input.GetButtonDown ("Jump")) {
                    anim.SetBool ("Rest", true);
                }
            }
            // REST中の処理
            // 現在のベースレイヤーがrestStateの時
            else if (currentBaseState.fullPathHash == restState) {
                //cameraObject.SendMessage("setCameraPositionFrontView");        // カメラを正面に切り替える
                // ステートが遷移中でない場合、Rest bool値をリセットする（ループしないようにする）
                if (!anim.IsInTransition (0)) {
                    anim.SetBool ("Rest", false);
                }
            }

            uparrowKey = false;
            homeKey = false;
            jumpKey = false;
            keycnt++;
        }


        void OnGUI()
        {
            Rect rect = new Rect(10, Screen.height - 60, 400, 30);
            showInteraction = GUI.Toggle(rect, showInteraction, "Show Interaction");

            if (showInteraction) {
                GUI.Box(new Rect(  Screen.width - 260,  10, 250, 150), "Interaction");
                GUI.Label(new Rect(Screen.width - 245,  30, 250,  30), "Up Arrow : Walk");
                GUI.Label(new Rect(Screen.width - 245,  50, 250,  30), "Up Arrow x2 : Run");
                GUI.Label(new Rect(Screen.width - 245,  70, 250,  30), "Home : Fly / Land");
                GUI.Label(new Rect(Screen.width - 245,  90, 250,  30), "PageUp / PageDown : Up / Down");
                GUI.Label(new Rect(Screen.width - 245, 110, 250,  30), "Left Control : Front Camera");
                GUI.Label(new Rect(Screen.width - 245, 130, 250,  30), "Space Bar : Jump (Run) / Rest (Idle)");
            }
        }


        // キャラクターのコライダーサイズのリセット関数
        void resetCollider()
        {
            // コンポーネントのHeight、Centerの初期値を戻す
            col.height = orgColHight;
            col.center = orgVectColCenter;
        }
    }
}
