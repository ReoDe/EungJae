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

    // 
    private GameObject _particleDest2 = null;



    private void OnAttackEnter(Collider other)
    {
        if (other != null)
        {
            if (other.name == "Plane")
                return;

            if (other.name == "RightHand")
                return;

            other.gameObject.SendMessage("Damaged", _attack);
        }
            
        
        if (_combo == 1)
            _particleDest2 = Instantiate(_particle2, other.transform.position, Quaternion.identity, this.transform);
        
    }

    private Vector3 _BasicCollider; // 원래값

    private void OnWeaponCollider()
    {
        _weaponCollider.enabled = true;

        if (_combo == 2)
        {
            _BasicCollider = _weaponCollider.size;
            Vector3 Skill3 = _BasicCollider;
            Skill3.x = 6;
            Skill3.y = 6;
            Skill3.z = 6;
            // 검의 박스를 크게하자
            _weaponCollider.size = Skill3;
        }
    }

    private void OffWeaponCollider()
    {
        _weaponCollider.enabled = false;

        if (_combo == 2)
        {
            _weaponCollider.size = _BasicCollider;
        }
    }

    private void Damaged(float damage)
    {
        _hp -= damage;
    }


    // 이펙트 3

    private GameObject _particleDest = null;

    private void PlaySlash3Particle()
    {
        _particleDest = Instantiate(_particle, _weapon.transform.position, Quaternion.identity, this.transform);

        AnimationClip clip = _animator.GetClip("Slash3");
        Invoke("FinishSlash3Particle", clip.length - 1.2f);
    }

    private void FinishSlash3Particle()
    {
        GameObject.Destroy(_particleDest);
    }

    

}
