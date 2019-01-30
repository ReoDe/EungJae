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

	private bool _bSlash = false;
	private bool _nextCombo = false;
	private int _combo = -1;

	private Weapon _weapon;
	private Collider _weaponCollider;

	private void Awake()
	{
		_animator = this.GetComponent<Animator>();
		Transform sword = this.transform.FindAllChildByName("Sword");
		_weapon = sword.transform.GetComponentInChildren<Weapon>();
		_weaponCollider = sword.transform.GetComponentInChildren<BoxCollider>();

		_weapon.OnEnter += OnAttackEnter;
	}

	private void Start()
	{
		this.transform.Rotate(0, 90, 0);

		for(int i = 0; i < events.Length; i++)
		{
			AnimationClip clip = _animator.GetClip(events[i].ClipName);
			clip.AddEvent(events[i].FuncName, events[i].Frame);
		}

		OffWeaponCollider();
	}

	private void Update () 
	{
		Moving();
		Slash();
	}

	private void Slash()
	{
		if (Input.GetButtonDown("Slash"))
		{
			if(_bSlash == true)
			{
				_nextCombo = true;
			}
			else
			{
				if(_combo == -1)
				{
					_combo = 0;
					_animator.SetTrigger("Slash");
				}

				_bSlash = true;
				_animator.SetInteger("Combo", _combo);
			}
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
	}

	private void OnGUI()
	{
		//GUI.color = new Color(1, 0, 0);
		//string temp = string.Format("FPS : {0}", _horizontal);

		//GUI.Label(new Rect(0, 0, 100, 20), temp);
	}
}
