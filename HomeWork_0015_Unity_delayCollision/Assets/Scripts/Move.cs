using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Move : MonoBehaviour
{
    [SerializeField] // attribute
    private float _moveSpeed = 10.0f;

    [SerializeField] // attribute
    private float _rotationSpeed = 20.0f;

    // Use this for initialization
    private void Start ()
	{
		Debug.Log("Hello, Unity");   
	}
	
	// Update is called once per frame
	private void Update ()
	{
        Vector3 position = this.transform.localPosition;

        if (Input.GetKey(KeyCode.D))
            position.x += _moveSpeed * Time.deltaTime;
        else if (Input.GetKey(KeyCode.A))
            position.x -= _moveSpeed * Time.deltaTime;

        if (Input.GetKey(KeyCode.W))
            position.z += _moveSpeed * Time.deltaTime;
        else if (Input.GetKey(KeyCode.S))
            position.z -= _moveSpeed * Time.deltaTime;

        this.transform.localPosition = position;



        Vector3 angle = this.transform.localRotation.eulerAngles;
        if (Input.GetKey(KeyCode.Q))
            angle.y += _rotationSpeed * Time.deltaTime;
        else if (Input.GetKey(KeyCode.E))
            angle.y -= _rotationSpeed * Time.deltaTime;

        this.transform.localEulerAngles = angle;


    }
}
