
using UnityEngine;

public delegate void CharacterTriggerHandler(Collider other);


public class Weapon : MonoBehaviour 
{
    public event CharacterTriggerHandler OnEnter;
    public event CharacterTriggerHandler OnStay;
    public event CharacterTriggerHandler OnExit;

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
