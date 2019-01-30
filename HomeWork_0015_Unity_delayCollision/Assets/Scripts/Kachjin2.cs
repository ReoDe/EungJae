
using UnityEngine;

public partial class Kachjin2 : MonoBehaviour
{

    [SerializeField]
    private float _walkSpeed = 2;

    [SerializeField]
    private float _runSpeed = 4;

    [SerializeField]
    private bool _bSlash = false;

    [SerializeField]
    private float _rotationSpeed = 50;


    private Weapon _weapon; // 무기
    private Collider _weaponCollider;

    // 생성자(객체 초기화)
    private void Awake()
    {
        _animator = this.GetComponent<Animator>(); // 애니메이터 가져옴


        // 무기 등록
        Transform sword = this.transform.FindAllChildByName("Sword");
        _weapon = sword.transform.GetComponentInChildren<Weapon>();


        // 무기 이벤트 등록
        _weapon.OnEnter += OnAttackEnter; // 이벤트 등록
        

        _weaponCollider = sword.transform.GetComponentInChildren<BoxCollider>();
        _weaponCollider.enabled = false;

        // 시작할때 끔

        //print(_weapon != null);
    }


    // 업데이트가 최초 콜 되기 직전
    private void Start()
    {
        this.transform.Rotate(0, 90, 0);

        for (int i = 0; i < events.Length; i++)
        {
            AnimationClip clip = _animator.GetClip(events[i].ClipName);


            clip.AddEvent(events[i].FuncName, events[i].Frame);

          
        }

    }
 
    // Update is called once per frame
    void Update()
    {
        Moving();

        Slash();
    }

    private void Slash()
    {
        if (Input.GetButtonDown("Slash"))
        {
            if (!_bSlash)
            {
                _bSlash = true;

                _animator.SetTrigger("Slash");
            }
           
        }
    }


    private void Moving()
    {
        // 이동처리
        if (_bSlash == true) return;

        float x = Input.GetAxis("Horizontal") * _walkSpeed * Time.deltaTime;
        float z = Input.GetAxis("Vertical") * _walkSpeed * Time.deltaTime;

        bool bWalk = Mathf.Abs(x) > 0.0f || Mathf.Abs(z) > 0.0f; 

        if (bWalk)
        {
            float y = 0.0f;

            if (z > 0) y = 0;
            if (z < 0) y = 180;
            if (x > 0) y = 90.0f;
            if (x < 0) y = -90.0f;

            if (z > 0 && x > 0) y = 45;
            if (z < 0 && x > 0) y = 135;
            if (z < 0 && x < 0) y = -135;
            if (z > 0 && x < 0) y = -45;


            this.transform.rotation = Quaternion.RotateTowards
                (
                    this.transform.rotation, Quaternion.Euler(0, y, 0), _rotationSpeed * 10 * Time.deltaTime
                );
        }

        _animator.SetBool("Walking", bWalk);

        bool bRun = Input.GetButton("Run");
        if (bRun)
        {
            x = Input.GetAxis("Horizontal") * _runSpeed * Time.deltaTime;
            z = Input.GetAxis("Vertical") * _runSpeed * Time.deltaTime;
        }
        _animator.SetBool("Running", bRun);


        Vector3 position = this.transform.position;
        this.transform.position = position + new Vector3(x, 0, z);
    }

    private void OnGUI()
    {   
   
    }


    private Color[] _colors;
    // 피격시 색바뀜
    private void Damaged()
    {
        Renderer[] renderers = this.GetComponentsInChildren<Renderer>();

        if (_colors == null)
            _colors = new Color[renderers.Length];

        for (int i = 0; i < renderers.Length; i++)
        {
            _colors[i] = renderers[i].material.color;
            renderers[i].material.color = Color.blue;
        }

        Invoke("FinishDamaged", 0.2f);
    }

    private void FinishDamaged()
    {
        Renderer[] renderers = this.GetComponentsInChildren<Renderer>();
        for (int i = 0; i < renderers.Length; i++)
        {
            renderers[i].material.color = _colors[i];
        }
    }
}
