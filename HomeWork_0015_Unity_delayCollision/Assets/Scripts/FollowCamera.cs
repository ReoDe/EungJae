
using UnityEngine;

public class FollowCamera : MonoBehaviour 
{
    [SerializeField]
    private float _lerpTime = 6.0f;

	private Transform _kachujin;
	private Vector3 _offset;

	private void Awake()
	{
		GameObject obj = GameObject.Find("Kachujin");
		_kachujin = obj.GetComponent<Transform>();

		_offset = this.transform.position - _kachujin.position;
	}

	private void Start()
	{
		
	}

	void Update () 
	{
        Vector3 b = _kachujin.position + _offset;
        Vector3 a = this.transform.position;

        this.transform.position = Vector3.Lerp(a, b, _lerpTime * Time.deltaTime);

	}
}
