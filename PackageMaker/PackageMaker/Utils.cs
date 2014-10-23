using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PackageMaker
{
    public static class Utils
    {
        public static byte[] serializeString(string _str)
        {
            List<byte> res = new List<byte>();
            Encoding utf8 = Encoding.UTF8;
            UInt32 length = (UInt32)utf8.GetByteCount(_str);
            res.AddRange(BitConverter.GetBytes(length));
            byte[] tmp = new byte[length];
            utf8.GetBytes(_str, 0, _str.Length, tmp, tmp.GetLowerBound(0));
            res.AddRange(tmp);

            return res.ToArray();
        }

        public static string trimPathFromName(string _path)
        {
            int indexOfLastSlash = _path.LastIndexOf('\\');
            return _path.Substring(indexOfLastSlash + 1);
        }
    }
}
