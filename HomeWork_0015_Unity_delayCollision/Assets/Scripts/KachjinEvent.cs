using System;
using UnityEngine;

[Serializable]
public struct CharacterEvent
{
    public string FuncName;
    public string ClipName;
    public uint Frame;
}

public partial class Kachjin2
{
    [SerializeField]
    private CharacterEvent[] events;

    private Animator _animator;

    private void OnSlashEnd()
    {
        _bSlash = false;
    }

    private void OnAttackEnter(Collider other)
    {
        print(other.gameObject.name);
        // Destroy(other.gameObject);
        other.gameObject.SendMessage("Damaged"); // damage함수 호출
        
    }

    private void OnWeaponCollider()
    {
        _weaponCollider.enabled = true;
    }

    private void OffWeaponCollider()
    {
        _weaponCollider.enabled = false;
    }

    
}
