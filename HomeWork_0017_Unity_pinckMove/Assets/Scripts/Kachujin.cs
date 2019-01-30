using UnityEngine;

public partial class Kachujin : MonoBehaviour 
{
	[SerializeField]
	private float _walkSpeed = 1;

	[SerializeField]
	private float _runSpeed = 2;

	[SerializeField]
	private float _attack = 30;

	[SerializeField]
	private float _rotationSpeed = 50;

	[SerializeField, Range(1, 5)]
	private float _attackRange = 1.3f;

	private bool _bSlash = false;


	private Weapon _weapon;
	private Collider _weaponCollider;

	// 피킹

	private Camera MainCamera;
	private Terrain _terrain;


	// 목적지
	private Vector3 Direction;
	[SerializeField]
	private bool _bPicking = false;


	// 적 위치알아오기
	private BoxCollider[] _Monster = new BoxCollider[2];
	private float[] _MonsterDist = new float[2];
	
	// 공격 딜레이
	[SerializeField]
	private float _maxDelay = 1.15f;
	[SerializeField]
	private float _SlashDelay = 0;
	private void Awake()
	{
		_animator = this.GetComponent<Animator>();
		Transform sword = this.transform.FindAllChildByName("Sword");
		_weapon = sword.transform.GetComponentInChildren<Weapon>();
		_weaponCollider = sword.transform.GetComponentInChildren<BoxCollider>();

		_weapon.OnEnter += OnAttackEnter;

		// 메인카메라
		MainCamera = GameObject.Find("Main Camera").GetComponent<Camera>();
		//print(MainCamera.name);


		Direction = this.transform.position;
	}

	private void Start()
	{
		this.transform.Rotate(0, 90, 0);

		for(int i = 0; i < events.Length; i++)
		{
			AnimationClip clip = _animator.GetClip(events[i].ClipName);
			clip.AddEvent(events[i].FuncName, events[i].Frame);
		}

		_Monster = GameObject.Find("Monsters").transform.GetComponentsInChildren<BoxCollider>();
   

		//print(_Monster[0].transform.position);
		//print(_Monster[1].transform.position);

		OffWeaponCollider();

		_SlashDelay = _maxDelay - 0.1f;

	}

	private void Update () 
	{
		Moving();
	   
		//Slash();

		GetMonstersDistance(); // 몬스터와 거리계산
		

		for (int i = 0; i < 2; i ++)
		{
			if (_MonsterDist[i] < _attackRange)
			{
			
				SetLookAt(i);
				Slash();
				
			
			}
			   
		}

		
	}

	private void OnDrawGizmos()
	{

		Gizmos.color = Color.red;
		Gizmos.DrawWireSphere(this.transform.position, _attackRange);
	}

	private void SetLookAt(int i)
	{
		Vector3 position = this.transform.position;
		Vector3 playerPosition = _Monster[i].transform.position;


		Vector3 direction = playerPosition - position;
		if (direction.magnitude > 0.0f)
		{
			direction.Normalize();

			Quaternion target = Quaternion.LookRotation(direction);
			this.transform.rotation = Quaternion.RotateTowards
			(
				this.transform.rotation, target, 10
			);
		}
	}

	private void GetMonstersDistance()
	{
		Vector2 position;
		position.x = this.transform.position.x;
		position.y = this.transform.position.z;

		for (int i = 0; i < 2; i++)
		{
			Vector2 MonsterPosition;
			MonsterPosition.x = _Monster[i].transform.position.x;
			MonsterPosition.y = _Monster[i].transform.position.z;

			_MonsterDist[i] = Vector2.Distance(position, MonsterPosition);
		}

		//print(_MonsterDist[0]);
		//print(_MonsterDist[1]);
	}


	private void Slash()
	{
		//print("공격");

		_SlashDelay += Time.deltaTime;

		
		if (!_bSlash && _SlashDelay > _maxDelay)
		{
			_bSlash = true;


			_animator.SetTrigger("Slash");
		}
		
		
	}

	private void Moving()
	{
		if (_bSlash == true) return;

		float x = Input.GetAxis("Horizontal") * _walkSpeed * Time.deltaTime;
		float z = Input.GetAxis("Vertical") * _walkSpeed * Time.deltaTime;

		bool bWalk = Mathf.Abs(x) > 0.0f || Mathf.Abs(z) > 0.0f;
		if (bWalk == true)
		{
			float y = 0.0f;
			if (z > 0) y = 0;
			if (z < 0) y = 180;
			if (x < 0) y = -90;
			if (x > 0) y = 90;

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
		if(bRun == true)
		{
			x = Input.GetAxis("Horizontal") * _runSpeed * Time.deltaTime;
			z = Input.GetAxis("Vertical") * _runSpeed * Time.deltaTime;
		}
		_animator.SetBool("Running", bRun);


		Vector3 position = this.transform.position;
		this.transform.position = position + new Vector3(x, 0, z);


		MouseMoving(); // 마우스이동

	}
	private void MouseMoving()
	{
		Picking();

		// 클릭이동
		if (_bPicking)
			MouseMove();
	}
	private void MouseMove()
	{
		_animator.SetBool("Running", true);

		Vector3 position = this.transform.position;

		position.y = 0.0f;

		Vector3 direction = Direction - position;


		if (direction.magnitude > 0.0f)
		{
			direction.Normalize();

			Quaternion target = Quaternion.LookRotation(direction);
			this.transform.rotation = Quaternion.RotateTowards
			(
				this.transform.rotation, target, 10
			);
		}

		Vector3 forward = this.transform.forward * _walkSpeed * Time.deltaTime;
		this.transform.Translate(forward, Space.World);

		// 
		float dist = Vector3.Distance(Direction, position);


		if (dist <= 0.1f)
		{
			_bPicking = false;
			_animator.SetBool("Running", false);
		}

	
		
	   // print(direction.magnitude);
	}

	private void OnGUI()
	{
		//GUI.color = new Color(1, 0, 0);
		//string temp = string.Format("FPS : {0}", _horizontal);

		//GUI.Label(new Rect(0, 0, 100, 20), temp);
	}

	private void Picking()
	{
		if (Input.GetMouseButtonUp(0))
		{
			Ray ray = MainCamera.ScreenPointToRay(Input.mousePosition);
			//print(Input.mousePosition);


		 
			rayCasting(ray);
		}
	}

	private void rayCasting(Ray ray)
	{
		RaycastHit hitObj;


		if(Physics.Raycast(ray, out hitObj, Mathf.Infinity))
		{
			if (hitObj.collider != null)
			{
				string Name = hitObj.collider.gameObject.name;

				if (Name == "Terrain")
				{

					Direction = hitObj.point;

					_bPicking = true;
					// 방향값 조정
				}
				else return;

				
			}


		}
	}
}
