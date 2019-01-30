
using UnityEngine;

public delegate void MonsterTriggerHandler(Collider other);


public class Punch_Cube : MonoBehaviour 
{
	public event MonsterTriggerHandler OnEnter;
	public event MonsterTriggerHandler OnStay;
	public event MonsterTriggerHandler OnExit;

	//isTrigger 체크되어있으면 충돌하고 통과
	private void OnTriggerEnter(Collider other)
	{
		if (OnEnter != null)
			OnEnter(other);
	}

	private void OnTriggerStay(Collider other)
	{
		if (OnStay != null)
			OnStay(other);
	}

	private void OnTriggerExit(Collider other)
	{
		if (OnExit != null)
			OnExit(other);
	}
	void Update () 
	{
		
	}
}
