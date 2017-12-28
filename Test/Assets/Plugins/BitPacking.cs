namespace DataCompresion
{
    using System.Runtime.InteropServices;

    public class BitPacking
    {
#if UNITY_IPHONE && !UNITY_EDITOR
        const string BitPackingDLL = "__Internal";
#else
        const string BitPackingDLL = "BitPacking";
#endif

        [DllImport(BitPackingDLL, CallingConvention=CallingConvention.Cdecl)]
        public static extern int WriteInt32(int val, byte[] data, int start);
        [DllImport(BitPackingDLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReadInt32(ref int val, byte[] data, int start);
        [DllImport(BitPackingDLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int WriteUInt32(uint val, byte[] data, int start);
        [DllImport(BitPackingDLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReadUInt32(ref uint val, byte[] data, int start);
    }
}

