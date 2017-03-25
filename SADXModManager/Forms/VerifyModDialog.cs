using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SADXModManager.Forms
{
	class VerifyModDialog : ProgressDialog
	{
		private List<Tuple<string, ModInfo>> mods;
		private readonly CancellationTokenSource tokenSource = new CancellationTokenSource();

		public List<Tuple<string, ModInfo, List<ModManifestDiff>>> Failed { get; private set; }

		public VerifyModDialog(List<Tuple<string, ModInfo>> mods)
			: base("Verifying...", true)
		{
			this.mods = mods;

			Failed = new List<Tuple<string, ModInfo, List<ModManifestDiff>>>();
			Shown += OnShown;
			CancelEvent += OnCancelEvent;
		}

		private void OnCancelEvent(object sender, EventArgs e)
		{
			tokenSource.Cancel();
		}

		private void OnShown(object sender, EventArgs e)
		{
			DialogResult = DialogResult.OK;

			int modCount = mods.Count;
			int modIndex = 0;
			SetTaskCount(modCount);

			CancellationToken token = tokenSource.Token;
			var generator = new ModManifestGenerator();

			void hashStart(object o, FileHashEventArgs args)
			{
				args.Cancel = token.IsCancellationRequested;
				SetStep($"Checking file {args.FileIndex}/{args.FileCount}: {args.FileName}");
				SetProgress(args.FileIndex / (double)args.FileCount);
			}

			void hashEnd(object o, FileHashEventArgs args)
			{
				args.Cancel = token.IsCancellationRequested;
			}

			generator.FileHashStart += hashStart;
			generator.FileHashEnd += hashEnd;

			using (var task = new Task(() =>
			{
				foreach (Tuple<string, ModInfo> i in mods)
				{
					string path = Path.Combine("mods", i.Item1);
					ModInfo info = i.Item2;

					SetTaskAndStep($"Verifying mod {++modIndex}/{modCount}: {info.Name}",
						"Loading local manifest...");

					List<ModManifest> local = ModManifest.FromFile(Path.Combine(path, "mod.manifest"));
					List<ModManifestDiff> diff = generator.Verify(path, local);

					if (diff?.Any(x => x.State != ModManifestState.Unchanged) == true)
					{
						Failed.Add(new Tuple<string, ModInfo, List<ModManifestDiff>>(i.Item1, info, diff));
					}

					NextTask();
				}
			}))
			{
				task.Start();

				while (!task.IsCompleted && !task.IsCanceled)
				{
					Application.DoEvents();
				}

				task.Wait(token);
			}
		}
	}
}
