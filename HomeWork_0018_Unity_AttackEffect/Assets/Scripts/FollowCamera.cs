using UnityEngine;

public class FollowCamera : MonoBehaviour 
{
	[SerializeField]
	private float _lerpTime = 6.0f;

	private Transform _kachujin;
	private Vector3 _offset;

	private float _shake = 0.0f;
	public float Shake
	{
		set { _shake = value; }
	}
	[SerializeField]
	private float _shakeAmount = 0.7f;

	[SerializeField]
	private float _decreasFactor = 1.0f;

	private void Awake()
	{
		GameObject obj = GameObject.Find("Kachujin");
		_kachujin = obj.GetComponent<Transform>();

		//Debug.Log(_kachujin != null);

		_offset = this.transform.position - _kachujin.position;
	}

	private void Start()
	{

	}

	void Update () 
	{
		Vector3 a = this.transform.position;
		Vector3 b = _kachujin.position + _offset;

		this.transform.position = Vector3.Lerp(a, b, _lerpTime * Time.deltaTime);



	}
}
