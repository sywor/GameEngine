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
    }
}
