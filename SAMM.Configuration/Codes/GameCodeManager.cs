using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using SAMM.Configuration.Mods;
using SAMM.Utilities;

namespace SAMM.Configuration.Codes
{
	public class GameCodeManager : NotifyObject
	{
		private Dictionary<string, GameCodeFile> gameCodes = new Dictionary<string, GameCodeFile>();
		public Dictionary<string, GameCodeFile> GameCodes
		{
			get => gameCodes;
			set => RaiseAndSetIfChanged(ref gameCodes, value);
		}

		private void WriteCodeData(string filepath, GameCodeFile file)
		{
			try
			{
				using FileStream fs = File.Create(filepath);
				using BinaryWriter bw = new(fs, Encoding.ASCII);
				bw.Write(['c', 'o', 'd', 'e', 'v', '5']);
				bw.Write(GameCodes.Count);
				file.WriteCodeData(bw);
				bw.Write(byte.MaxValue);
			}
			catch (Exception ex)
			{
				ExceptionHandler.Throw(ex);
			}
		}

		public void WriteDatFiles(string filepath)
		{
			string codeFilePath = Path.Combine(filepath, "Codes.dat");
			string patchFilePath = Path.Combine(filepath, "Patches.dat");

			GameCodeFile codeFile = new GameCodeFile();
			GameCodeFile patchFile = new GameCodeFile();

			foreach (var entry in GameCodes.Values)
			{
				foreach (GameCode code in entry.Codes)
				{
					if (code.IsChecked)
					{
						if (code.IsPatch)
							patchFile.Codes.Add(code);
						else
							codeFile.Codes.Add(code);
					}
				}
			}

			if (patchFile.Codes.Count > 0)
				WriteCodeData(patchFilePath, patchFile);
			else
			{
				if (File.Exists(patchFilePath))
					File.Delete(patchFilePath);
			}

			if (codeFile.Codes.Count > 0)
				WriteCodeData(codeFilePath, codeFile);
			else
			{
				if (File.Exists(codeFilePath))
					File.Delete(codeFilePath);
			}
		}

		public GameCodeManager() { }

		private static string ValidateCodeFilePath(string filepath, string filename)
		{
			if (File.Exists(Path.Combine(filepath, $"{filename}.json")))
				filepath = Path.Combine(filepath, $"{filename}.json");
			else if (File.Exists(Path.Combine(filepath, $"{filename}.lst")))
				filepath = Path.Combine(filepath, $"{filename}.lst");
			else
			{
				Logger.Log($"No codes file located at {filepath}");
				filepath = string.Empty;
			}

			return filepath;
		}

		/// <summary>
		/// Loads all of the code files found for a single mod.
		/// </summary>
		/// <param name="filepath">Expected to be the path to the .modloader folder.</param>
		/// <param name="enabledMods">List of enabled mods to load cheats from.</param>
		/// <returns></returns>
		public static GameCodeManager LoadAllCodes(string filepath, List<ModEntry> enabledMods)
		{
			GameCodeManager codeManager = new GameCodeManager();

			if (Path.Exists(filepath))
			{
				// Load the base cheat codes.
				string codeFile = ValidateCodeFilePath(filepath, "Codes");
				if (File.Exists(codeFile))
					codeManager.GameCodes.Add("Loader Codes", GameCodeFile.FromFile(codeFile));
				else
					Logger.Log($"{codeFile} does not exist. No native cheat codes added.");

				// Load any codes associated with any mods.
				foreach (ModEntry entry in enabledMods)
				{
					if (entry.ModInfo.Codes != null)
					{
						if (entry.ModInfo.Codes.Length > 0)
						{
							string filename = Path.GetFileNameWithoutExtension(entry.ModInfo.Codes);
							string modcodepath = Path.Combine(filepath, "..", entry.Key);
							string modCodeFile = ValidateCodeFilePath(modcodepath, filename);
							codeManager.GameCodes.Add(entry.ModInfo.Name, GameCodeFile.FromFile(modCodeFile));
						}
					}
				}
			}

			return codeManager;
		}
	}
}
