using System.Collections.Generic;
using UnityEngine;

public static class TransformAllByName
{	
	public static Transform FindAllChildByName(this Transform parent, string name)
    {
        Transform[] transforms = parent.GetComponentsInChildren<Transform>();
        for(int i = 0; i < transforms.Length; i++)
        {
            if (transforms[i].name == name)
                return transforms[i];
        }

        return null;
    }

    public static Transform[] FindsAllChildByName(this Transform parent, string name)
    {
        List<Transform> list = new List<Transform>();

        Transform[] transforms = parent.GetComponentsInChildren<Transform>();
        for (int i = 0; i < transforms.Length; i++)
        {
            if (transforms[i].name == name)
                list.Add(transforms[i]);
        }

        if (list.Count > 0)
            return list.ToArray();

        return null;
    }
}
