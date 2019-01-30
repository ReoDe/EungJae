using UnityEngine;

public partial class YBot : MonoBehaviour 
{
	[SerializeField]
	private float _hp = 100;


	[SerializeField]
	private float _attack = 30;

	[SerializeField]
	private float _attackDelay = 3;


	[SerializeField]
	private float _walkSpeed = 2;

	[SerializeField, Range(1, 5)]
	private float _searchRange = 3;

	[SerializeField, Range(1, 5)]
	private float _attackRange = 1;


	[SerializeField, Range(1, 5)]
	private float _reAttackRange = 1;

	private Transform _player;



	private bool _bPunch = false;
	private bool _bDeath = false;
	private bool _bReAttack = false;

	private enum State
	{
		Idle, Follow, Punching, Damage, Dying
	}
	[SerializeField]
	private State _state = State.Idle;
	[SerializeField]
	private State _prevState = State.Idle;

	private void Awake()
	{
		_animator = this.GetComponent<Animator>();
		_boxCollider = this.GetComponent<BoxCollider>();
		Transform rightHand = this.transform.FindAllChildByName("RightHand");
		_sphereCollider = rightHand.GetComponent<SphereCollider>();

		Weapon _weapon = rightHand.GetComponent<Weapon>();
		_weapon.OnEnter += OnAttackEnter;



		AnimationClip clip;
		clip = _animator.GetClip("Punching");
		clip.AddEvent("OnFinishPunch", 78);
		clip.AddEvent("OnWeaponCollider", 27);
		clip.AddEvent("OffWeaponCollider", 37);

		//clip = _animator.GetClip("Hitted");
		//clip.AddEvent("OnFinishDamage", 59);


	}

	private void Start()
	{
		_player = GameObject.Find("Kachujin").GetComponent<Transform>();

		OffWeaponCollider();
	}

	void Update () 
	{
		if (_bDeath)
			return;

		SetLookAt();

		

		switch (_state)
		{
			case State.Idle: Idling(); break;
			case State.Follow: Follow(); break;
			case State.Punching: Punching(); break;
			
			case State.Dying: Dying(); break;
		}
		_prevState = _state;

	}

	private void Idling()
	{
		float distance = GetPlayerDistance();

		if (_bReAttack == false)
		{
			if (distance <= _attackRange && _state != State.Damage)
				_state = State.Punching;
			else if (distance < _searchRange)
				_state = State.Follow;
		}

		if (_animator.GetBool("Walking") == true)
			_animator.SetBool("Walking", false);


	}

	private void Follow()
	{


		float distance = GetPlayerDistance();

		if (distance > _searchRange)
		{
			_state = State.Idle;
			_animator.SetBool("Walking", false);
		}


		if (distance <= _attackRange && _state != State.Damage)
			_state = State.Punching;


		if (distance <= _searchRange)
		{
			Vector3 forward = this.transform.forward * _walkSpeed * Time.deltaTime;
			this.transform.Translate(forward, Space.World);
		}

		if (_animator.GetBool("Walking") == false)
			_animator.SetBool("Walking", true);

	}

	private void Punching()
	{
		if (_prevState == State.Punching)
			_animator.SetBool("Walking", false);

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

	

	private void Dying()
	{
		if (_bDeath)
			return;

		_bDeath = true;

		_boxCollider.enabled = false;

		_state = State.Dying;
		_animator.SetTrigger("Dying");

		AnimationClip clip = _animator.GetClip("Dying");
		Invoke("Dead", clip.length + 2.0f);
	}

	private void Dead()
	{
		this.gameObject.SetActive(false);
	}


}
