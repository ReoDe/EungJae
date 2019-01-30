
using UnityEngine;

public partial class ybot : MonoBehaviour 
{

	[SerializeField]
	private float _walkSpeed = 2;

	[SerializeField, Range(1,5)]
	private float _searchRange = 3;


	[SerializeField, Range(1, 5)]
	private float _attackRange = 1;


	[SerializeField]
	private float _maxPunchDelay = 2;

	[SerializeField]
	private float _PunchDelay = 0;


   
	private Transform _player;

	[SerializeField]
	private bool _bPunch = false;

	// 무기
	private Punch_Cube _punch;
	private Collider _punchCollider;
	

	private enum State
	{
		Idle, Walk, Punching, Agro
	}

	[SerializeField]
	private State _state = State.Idle;
	private State _prevState = State.Idle;



	private void Awake()
	{
		_animator = this.GetComponent<Animator>();

		// 무기 등록
		Transform punch = this.transform.FindAllChildByName("Palm");
		//print(punch.name);
		_punch = punch.transform.GetComponentInChildren<Punch_Cube>();

  

		// 무기 이벤트 등록
		_punch.OnEnter += OnPunchEnter;

		_punchCollider = punch.transform.GetComponentInChildren<BoxCollider>();
		_punchCollider.enabled = false;
		//print(_punchCollider);

		//print(_punch != null);
	}

	private void Start()
	{

		for (int i = 0; i < events.Length; i++)
		{
			AnimationClip clip = _animator.GetClip(events[i].ClipName);

			clip.AddEvent(events[i].FuncName, events[i].Frame);
		}

		// 애니메이션
   
		//clip = _animator.GetClip("Punching");
		//clip.AddEvent("OnFinishPunch", 77);

		_player = GameObject.Find("Kachujin").GetComponent<Transform>();

		// 일단선빵
		_PunchDelay = _maxPunchDelay - 0.1f;
	}

	void Update () 
	{

	   

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
		if (_prevState == State.Idle)
			_animator.SetBool("Walking", true);


		SetLookAt(); // 방향만 바라보게


		Vector3 forward = this.transform.forward * _walkSpeed * Time.deltaTime;
		this.transform.Translate(forward, Space.World);
		// 여기가 이동하는 코드


	}

	private void Punching()
	{
		_PunchDelay += Time.deltaTime; // 펀치 딜레이 up

		if (_bPunch == true)
			return;

		if (_PunchDelay > _maxPunchDelay)
		{
			_bPunch = true;
			_animator.SetTrigger("Punching");

			_PunchDelay = 0;
		}

	}

	private void SetLookAt()
	{
		Vector3 position = this.transform.position;
		Vector3 PlayerPosition = _player.position;
		position.y = PlayerPosition.y = 0.0f;

		Vector3 direction = PlayerPosition - position;

		if (direction.magnitude > 0.3f) // 길이가 크면 (길이)
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

	private void Damaged()
	{
		// 여러가지 s, 랜더는 전부 Renderer에 상속받아있음
		Renderer[] renderers = this.GetComponentsInChildren<Renderer>();

		if (_colors == null)
			_colors = new Color[renderers.Length];

		for (int i = 0; i < renderers.Length; i++)
		{
			_colors[i] = renderers[i].material.color;
			renderers[i].material.color = Color.red;
		}

		Invoke("FinishDamaged", 0.2f); // Damaged 함수 호출 후 0.2초 뒤 FinishDamaged함수 호출
	   // print("피격");
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

/*
 * 캐릭터 피격 당하는지 확인, 색 바꿔서
 * 
 * 
 * ybot의 공격 딜레이 만듬,( Invoke 함수 쓰면 될꺼같은데)
 * 
 */

