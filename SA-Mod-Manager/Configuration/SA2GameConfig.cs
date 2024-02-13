using SAModManager.Ini;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAModManager.Configuration
{
    public class SA2GameConfig
    {
        public int FullScreen { get; set; } = 0;


    }

    public class SA2ConfigFile
    { 

        public SA2GameConfig GameConfig { get; set; } = new();
    }
}
