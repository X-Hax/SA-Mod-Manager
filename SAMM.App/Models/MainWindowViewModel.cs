using ReactiveUI;
using SAMM.Configuration;

namespace SAMM.App.Models
{
	public class MainWindowViewModel : ReactiveObject
	{
		#region Variables
		public ManagerSettings ManagerSettings { get; set; } = new ManagerSettings();

		public GameConfig GameConfig { get; set; } = new GameConfig();

		public GameSettings? GameSettings { get; set; }

		#endregion

		#region Constructors


		#endregion
	}
}
