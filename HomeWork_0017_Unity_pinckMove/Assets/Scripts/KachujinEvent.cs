using System;
using UnityEngine;

[Serializable]
public struct CharacterEvent
{
    public string FuncName;
    public string ClipName;
    public uint Frame;
}

public partial class Kachujin
{
    [SerializeField]
    private CharacterEvent[] events;

    private Animator _animator;

    
    private void OnSlashEnd()
    {
       
        _bSlash = false;
        _SlashDelay = 0;
    }
    
    private void OnAttackEnter(Collider other)
    {
        other.gameObject.SendMessage("Damaged", _attack);
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
