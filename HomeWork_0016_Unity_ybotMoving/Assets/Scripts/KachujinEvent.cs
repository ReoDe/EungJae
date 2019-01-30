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

    private void OnCheckCombo()
    {
        if (_nextCombo == true)
            _combo++;
        else
            _combo = -1;
    }

    private void OnSlashEnd()
    {
        if (_nextCombo == false)
            _bSlash = false;

        _nextCombo = false;
        _animator.SetInteger("Combo", _combo);
    }

    private void OnSlashFinish()
    {
        _bSlash = false;
        _nextCombo = false;

        _combo = -1;
        _animator.SetInteger("Combo", _combo);
    }

    private void OnAttackEnter(Collider other)
    {
        //print(other.gameObject.name);
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
