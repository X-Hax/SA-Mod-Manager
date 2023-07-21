using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ModManagerWPF.Models
{
    public class CodesViewModel
    {
		public string Name { get; set; }
		public ModManagerCommon.Code Code { get; set; }
		public bool IsExpanded { get; set; }
		public bool IsRoot { get; set; }
		public IEnumerable<CodesViewModel> Children { get; set; }
    }
}
