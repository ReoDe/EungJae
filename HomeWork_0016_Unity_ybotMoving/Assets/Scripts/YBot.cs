

using UnityEngine;

public partial class YBot : MonoBehaviour 
{
	[SerializeField]
	private float _hp = 100;

	[SerializeField]
	private float _walkSpeed = 2;

	[SerializeField, Range(1, 5)]
	private float _searchRange = 3;

	[SerializeField, Range(1, 5)]
	private float _attackRange = 1;

   
	private Transform _player;
	

	private BoxCollider _boxCollider;
	
	[SerializeField]
	private bool _bPunch = false;


	// 필드에서 서성거리는것
	private BoxCollider[] _DB = new BoxCollider[6];

	[SerializeField]
	private int _boxNumber = 0;

	[SerializeField]
	private float[] _maxBoxTime = new float[6];
	[SerializeField]
	private float[] _BoxTime = new float[6];

	private bool _AtPlayer = false; // 플레이어와 이동충돌
	[SerializeField]
	private bool _AtBox = false;  // 플레이어와 박스충돌


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

		_DB = GameObject.Find("DirectionBoxes").transform.GetComponentsInChildren<BoxCollider>();

		for (int i = 0; i < 6; i++)
		{
			_maxBoxTime[i] = Random.Range(1.0f, 5.0f);
			_BoxTime[i] = 0;
		}


		//for (int i = 0; i < 6; i++)
		//{
		//	print(_directionBoxCollider[i].enabled);
		//}

		//for (int i = 0; i < 6; i++)
		//    print(_DB[i].name);

	}

	private void Start()
	{
		_player = GameObject.Find("Kachujin").GetComponent<Transform>();
		for (int i = 0; i < events.Length; i++)
		{
			AnimationClip clip = _animator.GetClip(events[i].ClipName);
			clip.AddEvent(events[i].FuncName, events[i].Frame);
		}


		// 초기값 설정

		//박스위치랑 내 위치 비교




		GetDirectionNumber();



	}

	

	void Update () 
	{
		if (_state == State.Dying)
			return;


		float boxDistance = GetBoxesDistance(_boxNumber);

		//print(boxDistance);

		float distance = GetPlayerDistance();

		//print(distance);

		if (distance > _searchRange)
		{
			_AtPlayer = false;

			if (_AtBox)
			{
				if (boxDistance > _attackRange)
				{
					_AtBox = false;
				}

				_state = State.Idle;
			}
			else
			{
				if (boxDistance < _attackRange) // 거리가 가까워지면 멈추고
				{
					//print("idle");

					_AtBox = true;
					_state = State.Idle;
				}
				else _AtBox = false;

				_state = State.Walk;

				//GetDirectionNumber();
			}
			

		}
		else
		{
			_AtPlayer = true;
			_state = State.Walk;
		}

		//if (boxDistance < _attackRange)
		//{
		//    print("idle");

		//    _AtBox = true;
		//    _state = State.Idle;
		//}
		//else _AtBox = false;
			

		if (distance < _attackRange)
			_state = State.Punching;

		

		switch (_state)
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


		_BoxTime[_boxNumber] += Time.deltaTime;

		if (_BoxTime[_boxNumber] > _maxBoxTime[_boxNumber])
		{
			_BoxTime[_boxNumber] = 0;

			int _intRandom;
			_intRandom = Random.Range(0, 10);
			print(_intRandom);

			if (_intRandom % 2 == 0)
			{
				if (_boxNumber == 5)
					_boxNumber = 0;
				else _boxNumber++;
			}
			else
			{
				if (_boxNumber == 0)
					_boxNumber = 5;
				else _boxNumber--;
			}


			

			_state = State.Walk;
		}

	}

	private void Walking()
	{
		if(_prevState == State.Idle)
			_animator.SetBool("Walking", true);

		if (_AtPlayer)
		{
			SetLookAt();
			GetDirectionNumber();
		}
		else
			SetLookAtBox(_boxNumber);
		

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


	// 여기가 방향보는 함수
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

	private float GetBoxesDistance(int Number)
	{
		Vector2 position;
		position.x = this.transform.position.x;
		position.y = this.transform.position.z;

		Vector2 BoxPosition;
		
		BoxPosition.x = _DB[Number].transform.position.x;
		BoxPosition.y = _DB[Number].transform.position.z;
		//print(BoxPosition);


		return Vector2.Distance(position, BoxPosition);
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
	}

	// 박스 방향전환함수

	private void DirectionBox()
	{
		_state = State.Walk;
	}

	private void SetLookAtBox(int boxNumber)
	{
		Vector3 temp = _DB[boxNumber].transform.position - this.transform.position;
		Vector3 MonsterPosition = this.transform.position;
		Vector3 directionBox = temp - MonsterPosition;
		// 박스위치랑 내 위치 비교
  //      for (int i= 0; i < 6; i++)
		//{
		//	Vector3 BoxPosition = _DB[i].transform.position;
		//	Vector3 MonsterPosition = this.transform.position;
		//	BoxPosition.y = MonsterPosition.y = 0.0f;

		//	Vector3 directionBox = BoxPosition - MonsterPosition;

		//	if (temp.magnitude > directionBox.magnitude)
		//		temp = directionBox;
		//}

		if (directionBox.magnitude > 0.0f)
		{
			directionBox.Normalize();

			Quaternion target = Quaternion.LookRotation(temp);
			this.transform.rotation = Quaternion.RotateTowards
			(
				this.transform.rotation, target, 10
			);
		}
	}

	void GetDirectionNumber()
	{
		Vector3 temp = _DB[_boxNumber].transform.position - this.transform.position;

		for (int i = 0; i < 6; i++)
		{
			int directionBoxNumber = i;

			Vector3 BoxPosition = _DB[i].transform.position;
			Vector3 MonsterPosition = this.transform.position;
			BoxPosition.y = MonsterPosition.y = 0.0f;

			Vector3 directionBox = BoxPosition - MonsterPosition;

			if (temp.magnitude > directionBox.magnitude)
			{
				temp = directionBox;
				_boxNumber = directionBoxNumber;
			}

		}
	}
}
