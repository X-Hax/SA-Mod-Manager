﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Serialization;
using System.Threading.Tasks;

namespace SAMM.Configuration
{
	public class UserProfile
	{
		#region Variables
		/// <summary>
		/// Display name of the User Profile.
		/// </summary>
		public string Name { get; set; } = string.Empty;

		/// <summary>
		/// Name of the file storing the user profile information.
		/// </summary>
		public string Filename { get; set; } = string.Empty;

		/// <summary>
		/// Full path to the file storing the user profile information.
		/// </summary>
		public string Filepath { get { return Path.Combine(Filename); } }

		#endregion

		#region Constructors
		public UserProfile(string name, string filename)
		{
			Name = name;
			Filename = filename;
		}

		#endregion
	}

	public class UserProfiles
	{
		#region Variables
		/// <summary>
		/// Index of the current/last selected profile.
		/// </summary>
		public int ProfileIndex { get; set; } = 0;

		/// <summary>
		/// List of all profiles the user currently has saved.
		/// </summary>
		[JsonPropertyName("ProfileList")]
		public List<UserProfile> Profiles { get; set; } = new List<UserProfile>();

		#endregion
	}
}
