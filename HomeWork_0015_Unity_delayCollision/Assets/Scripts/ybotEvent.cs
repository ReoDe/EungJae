using System;
using UnityEngine;


[Serializable]
public struct MonsterEvent
{
    public string FuncName;
    public string ClipName;
    public uint Frame;
}


public partial class ybot
{
    [SerializeField]
    private MonsterEvent[] events;


    private Animator _animator;

    // 여기부터 이벤트

    private void OnFinishPunch()
    {
        _bPunch = false;
    }

    private void OnPunchEnter(Collider other)
    {
        print(other.gameObject.name);
        other.gameObject.SendMessage("Damaged"); // damage함수 호출
    }

    private void OnPunchCollider()
    {
        _punchCollider.enabled = true;
    }

    private void OffPunchCollider()
    {
        _punchCollider.enabled = false;
    }

}
