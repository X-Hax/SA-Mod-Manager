using Avalonia.Controls.Templates;
using Avalonia.Controls;
using Avalonia.Metadata;
using System;
using System.Collections.Generic;
using SAMM.App.Models;

namespace SAMM.App.Selectors
{
	public class ConfigPropertyTemplateSelector : IDataTemplate
	{
		[Content]
		public Dictionary<string, IDataTemplate> Templates { get; } = new Dictionary<string, IDataTemplate>();

		public Control Build(object? param)
		{
			var key = param as ModConfigPropertyModel;
			if (key == null)
				throw new ArgumentNullException();

			if (Templates.ContainsKey(key.Type.ToLowerInvariant()))
				return Templates[key.Type.ToLowerInvariant()].Build(param);
			else
				throw new NotImplementedException();
		}

		public bool Match(object? data)
		{
			var key = data as ModConfigPropertyModel;

			return data is ModConfigPropertyModel && !string.IsNullOrEmpty(key.Type) && Templates.ContainsKey(key.Type.ToLowerInvariant());
		}
	}
}
