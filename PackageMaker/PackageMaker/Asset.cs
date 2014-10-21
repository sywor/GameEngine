using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PackageMaker
{
    class Asset
    {
        private string name, path;
        private byte[] data;
        private UInt32 startByte;

        public Asset(string _path, byte[]_data)
        {
            path = _path;
            name = Utils.trimPathFromName(path);
            data = _data;
        }

        public void setStart(UInt32 _start)
        {
            startByte = _start;
        }

        public UInt32 getStart()
        {
            return startByte;
        }

        public byte[] getBytes()
        {
            return data;
        }

        public string getName()
        {
            return name;
        }

        public string getPath()
        {
            return path;
        }

        public UInt32 getSize()
        {
            return (UInt32)data.Length;
        }
    }
}
