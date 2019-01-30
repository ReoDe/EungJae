using UnityEngine;

public delegate void TriggerHandler(Collider other);

public class Weapon : MonoBehaviour 
{
    public event TriggerHandler OnEnter;
    public event TriggerHandler OnStay;
    public event TriggerHandler OnExit;

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
}
