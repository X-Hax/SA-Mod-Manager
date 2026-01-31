using Avalonia.Controls.Templates;
using Avalonia.Controls;
using Avalonia.Metadata;
using System;
using System.Collections.Generic;
using SAMM.App.Models;
using SAMM.Configuration.XML;

namespace SAMM.App.Selectors
{
	public class ConfigPropertyTemplateSelector : IDataTemplate
	{
		[Content]
		public Dictionary<string, IDataTemplate> Templates { get; } = new Dictionary<string, IDataTemplate>();

		public Control Build(object? param)
		{
			var key = param as ConfigSchemaProperty;
			if (key == null)
				throw new ArgumentNullException();

			if (Templates.ContainsKey(key.Type.ToLowerInvariant()))
				return Templates[key.Type.ToLowerInvariant()].Build(param);
			else
				return Templates["enum"].Build(param);
		}

		public bool Match(object? data)
		{
			var key = data as ConfigSchemaProperty;

			bool retVal = data is ConfigSchemaProperty && !string.IsNullOrEmpty(key.Type) && Templates.ContainsKey(key.Type.ToLowerInvariant());
			if (!retVal)
				retVal = data is ConfigSchemaProperty && Templates.ContainsKey("enum");
			
			return retVal;
		}
	}
}
