# bit-packing
Unity3D中网络流量压缩动态库构建和测试项目
 
使用zigzag压缩，比protobuf省流量，适用于高频率通讯的状态同步，或者帧同步游戏
 
使用示例：


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
        
        
        
