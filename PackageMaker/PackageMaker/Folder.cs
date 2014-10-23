using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PackageMaker
{
    class Folder
    {
        private List<Folder> subFolders = new List<Folder>();
        private List<Asset> assets = new List<Asset>();
        private string name, fullPath;
        private byte[] data;

        public Folder(string _fullPath)
        {
            fullPath = _fullPath;
            name = Utils.trimPathFromName(fullPath);
        }
        
        public void addSubFolder(Folder _folder)
        {
            subFolders.Add(_folder);
        }

        public void addAsset(Asset _asset)
        {
            assets.Add(_asset);
        }

        public List<Asset> getAssets()
        {
            return assets;
        }

        public List<Folder> getSubFolders()
        {
            return subFolders;
        }

        public string getName()
        {
            return name;
        }

        public string getPath()
        {
            return fullPath;
        }

        public void serialize()
        {
            List<byte> res = new List<byte>();
            res.AddRange(Utils.serializeString(name));            

            res.AddRange(BitConverter.GetBytes((UInt32)subFolders.Count));

            foreach (Folder f in subFolders)
            {
                res.AddRange(Utils.serializeString(f.getName()));
            }

            res.AddRange(BitConverter.GetBytes((UInt32)assets.Count));

            foreach (Asset a in assets)
            {
                res.AddRange(Utils.serializeString(a.getName()));
            }


        }

        public byte[] getBytes()
        {
            return data;
        }
    }
}
