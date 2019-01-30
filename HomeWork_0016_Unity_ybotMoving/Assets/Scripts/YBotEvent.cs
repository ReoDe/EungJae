using System;
using UnityEngine;

[Serializable]
public struct MonsterEvent
{
    public string FuncName;
    public string ClipName;
    public uint Frame;
}

public partial class YBot 
{
    [SerializeField]
    private MonsterEvent[] events;

    private Animator _animator;


    /*
    AnimationClip clip;
    clip = _animator.GetClip("Punching");
        clip.AddEvent("OnFinishPunch", 78);
        */

    private void OnFinishPunch()
    {
        _bPunch = false;
    }


}
