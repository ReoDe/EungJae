using UnityEngine;

public class YBot : MonoBehaviour 
{
    [SerializeField]
    private float _hp = 100;

    [SerializeField]
    private float _walkSpeed = 2;

    [SerializeField, Range(1, 5)]
    private float _searchRange = 3;

    [SerializeField, Range(1, 5)]
    private float _attackRange = 1;

    private Animator _animator;
    private Transform _player;

    private BoxCollider _boxCollider;

    private bool _bPunch = false;

    private enum State
    {
        Idle, Walk, Punching, Dying
    }
    private State _state = State.Idle;
    private State _prevState = State.Idle;


    private void Awake()
    {
        _animator = this.GetComponent<Animator>();
        _boxCollider = this.GetComponent<BoxCollider>();


        AnimationClip clip;
        clip = _animator.GetClip("Punching");
        clip.AddEvent("OnFinishPunch", 78);
    }

    private void Start()
    {
        _player = GameObject.Find("Kachujin").GetComponent<Transform>();
    }

    void Update () 
	{
        if (_state == State.Dying)
            return;


        float distance = GetPlayerDistance();

        if (distance > _searchRange)
            _state = State.Idle;
        else
            _state = State.Walk;

        if (distance < _attackRange)
            _state = State.Punching;


        switch(_state)
        {
            case State.Idle: Idling(); break;
            case State.Walk: Walking(); break;
            case State.Punching: Punching(); break;
        }

        _prevState = _state;
    }

    private void Idling()
    {
        if (_prevState == State.Walk)
            _animator.SetBool("Walking", false);
    }

    private void Walking()
    {
        if(_prevState == State.Idle)
            _animator.SetBool("Walking", true);

        SetLookAt();

        if(_bPunch == false)
        {
            Vector3 forward = this.transform.forward * _walkSpeed * Time.deltaTime;
            this.transform.Translate(forward, Space.World);
        }
    }

    private void Punching()
    {
        if (_bPunch == true)
            return;

        _bPunch = true;
        _animator.SetTrigger("Punching");
    }

    private void SetLookAt()
    {
        Vector3 position = this.transform.position;
        Vector3 playerPosition = _player.position;
        position.y = playerPosition.y = 0.0f;

        Vector3 direction = playerPosition - position;
        if(direction.magnitude > 0.0f)
        {
            direction.Normalize();

            Quaternion target = Quaternion.LookRotation(direction);
            this.transform.rotation = Quaternion.RotateTowards
            (
                this.transform.rotation, target, 10
            );
        }
    }

    private float GetPlayerDistance()
    {
        Vector2 position;
        position.x = this.transform.position.x;
        position.y = this.transform.position.z;

        Vector2 playerPosition;
        playerPosition.x = _player.position.x;
        playerPosition.y = _player.position.z;

        return Vector2.Distance(position, playerPosition);
    }

    private void OnDrawGizmos()
    {
        Gizmos.color = Color.red;
        Gizmos.DrawWireSphere(this.transform.position, _searchRange);

        Gizmos.color = Color.green;
        Gizmos.DrawWireSphere(this.transform.position, _attackRange);
    }

    private Color[] _colors;
    private void Damaged(float damage)
    {
        _hp -= damage;
        if (_hp <= 0)
            Dying();


        Renderer[] renderers = this.GetComponentsInChildren<Renderer>();

        if (_colors == null)
            _colors = new Color[renderers.Length];

        for (int i = 0; i < renderers.Length; i++)
        {
            _colors[i] = renderers[i].material.color;
            renderers[i].material.color = new Color(1, 0, 0);
        }
        
        Invoke("FinishDamaged", 0.2f);
    }

    private void FinishDamaged()
    {
        Renderer[] renderers = this.GetComponentsInChildren<Renderer>();
        for (int i = 0; i < renderers.Length; i++)
            renderers[i].material.color = _colors[i];
    }

    private void Dying()
    {
        if (_state == State.Dying)
            return;


        _boxCollider.enabled = false;

        _state = State.Dying;
        _animator.SetTrigger("Dying");

        AnimationClip clip = _animator.GetClip("Dying");
        Invoke("Dead", clip.length + 2.0f);
    }

    private void Dead()
    {
        this.gameObject.SetActive(false);
        //Destroy(this.gameObject);
    }

    private void OnFinishPunch()
    {
        _bPunch = false;
    }
}
