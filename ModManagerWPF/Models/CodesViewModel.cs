using SAModManager.Common;
using System.Collections.Generic;

namespace SAModManager.Models
{
    public class CodesViewModel
    {
		public string Name { get; set; }
		public Code Code { get; set; }
		public bool IsExpanded { get; set; }
		public bool IsRoot { get; set; }
		public IEnumerable<CodesViewModel> Children { get; set; }
    }
}
