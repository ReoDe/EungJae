
using UnityEngine;

public partial class YBot 
{
    private Animator _animator;
    private BoxCollider _boxCollider;
    private SphereCollider _sphereCollider;

    // ==== event ====

    private void OnFinishPunch()
    {
        float distance = GetPlayerDistance();
        if (distance < _reAttackRange)
        {
            _bReAttack = true;
            _state = State.Idle;
            _animator.SetBool("Walking", false);

            Invoke("OnFinishPunch2", _attackDelay);
           
        }
        else
        {
            OnFinishPunch2();
        }
       
    }
    private void OnFinishPunch2()
    {
        _bPunch = false;
        _bReAttack = false;

        _state = State.Idle;
        _animator.SetBool("Walking", false);
    }
    private void OffWeaponCollider()
    {
        _sphereCollider.enabled = false;
    }

    private void OnWeaponCollider()
    {
        _sphereCollider.enabled = true;
    }

    private void OnAttackEnter(Collider other)
    {
        if (other.gameObject.name == "Kachujin")
        {
            other.gameObject.SendMessage("Damaged", _attack);
        }

    }

    //  데미지 계산

    private Color[] _colors;
    private void Damaged(float damage)
    {
        if (_bDeath == true) return;

        _hp -= damage;
        _state = State.Damage;

        if (_hp <= 0.0f)
        {
            _state = State.Dying;
        }
        else
        {
            _animator.SetTrigger("Hitted");

            AnimationClip clip = _animator.GetClip("Hitted");
            Invoke("FinishDamaged", clip.length);
        }

        // 색상관련

        //Renderer[] renderers = this.GetComponentsInChildren<Renderer>();

        //if (_colors == null)
        //    _colors = new Color[renderers.Length];

        //for (int i = 0; i < renderers.Length; i++)
        //{
        //    _colors[i] = renderers[i].material.color;
        //    renderers[i].material.color = new Color(1, 0, 0);
        //}

        //Invoke("FinishDamaged", 0.2f);

        //
        
    }

    private void FinishDamaged()
    {
        //Renderer[] renderers = this.GetComponentsInChildren<Renderer>();
        //for (int i = 0; i < renderers.Length; i++)
        //    renderers[i].material.color = _colors[i];

        _bReAttack = false;
        _bPunch = false;

        _state = State.Idle;


    }

}
