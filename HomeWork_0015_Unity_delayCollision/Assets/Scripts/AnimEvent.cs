
using UnityEngine;

public static class AnimEvent 
{
    public static AnimationClip GetClip(this Animator animator, string name)
    {
        AnimationClip[] clips = animator.runtimeAnimatorController.animationClips;

        for (int i = 0; i < clips.Length; i++)
        {
            if (clips[i].name == name)
                return clips[i];
        }

        return null;
    }


    public static uint GetFrameCount(this AnimationClip clip)
    {
        return (uint)(clip.frameRate * clip.length);
    }



    public static float GetTime(this AnimationClip clip, uint frame)
    {
        float lastFrame = (float)(clip.GetFrameCount() - 1);

        return (frame / lastFrame) * clip.length;
    }

    public static void AddEvent(this AnimationClip clip, string funcName, uint frame)
    {
        float exacTime = clip.GetTime(frame);

        AnimationEvent e = new AnimationEvent()
        {
            time = exacTime,
            functionName = funcName
        };
        clip.AddEvent(e);
    }

}

