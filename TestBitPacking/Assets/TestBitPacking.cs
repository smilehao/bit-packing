using UnityEngine;
using System.Collections;
using DataCompresion;

public class TestBitPacking : MonoBehaviour
{
    int a = -11234;
    int b = 0;
    uint c = 11234;
    uint d = 0;
    byte[] arr = new byte[100];
    byte[] brr = new byte[100];
    byte[] crr = new byte[100];
    byte[] drr = new byte[100];

    // Use this for initialization
    void Start () {
        for (int i = 0; i < 100; i++)
        {
            arr[i] = 0;
            brr[i] = 0;
            crr[i] = 0;
            drr[i] = 0;
        }

        brr[0] = 195;
        brr[1] = 175;
        brr[2] = 1;
        drr[0] = 226;
        drr[1] = 87;

        DoTest();
    }
	
	void DoTest ()
    {
        Debug.Log("write&read int : -11234\n");
        int reta = BitPacking.WriteInt32(a, arr, 0);
        Debug.Log(string.Format("write = {0}: {1}\t{2}\t{3}\t{4}\t{5}\n", reta, arr[0], arr[1], arr[2], arr[3], arr[4]));
        int retb = BitPacking.ReadInt32(ref b, brr, 0);
        Debug.Log(string.Format("read = {0}: {1}\n", retb, b));

        Debug.Log("write&read uint : 11234\n");
        int retc = BitPacking.WriteUInt32(c, crr, 0);
        Debug.Log(string.Format("write = {0}: {1}\t{2}\t{3}\t{4}\t{5}\n", retc, crr[0], crr[1], crr[2], crr[3], crr[4]));
        int retd = BitPacking.ReadUInt32(ref d, drr, 0);
        Debug.Log(string.Format("read = {0}: {1}\n", retd, d));

        if (reta != retb)
        {
            Debug.LogError("reta != retb");
        }
        if (a != b)
        {
            Debug.LogError("a != b");
        }
        if (retc != retd)
        {
            Debug.LogError("retc != retd");
        }
        if (c != d)
        {
            Debug.LogError("c != d");
        }
        Debug.Log("TestDone!\n");
    }
}
