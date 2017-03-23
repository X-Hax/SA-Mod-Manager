using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Reflection;
using IniDictionary = System.Collections.Generic.Dictionary<string, System.Collections.Generic.Dictionary<string, string>>;
using IniGroup = System.Collections.Generic.Dictionary<string, string>;
using IniNameGroup = System.Collections.Generic.KeyValuePair<string, System.Collections.Generic.Dictionary<string, string>>;
using IniNameValue = System.Collections.Generic.KeyValuePair<string, string>;

namespace IniFile
{
	public static class IniSerializer
	{
		private static readonly IniCollectionSettings initialCollectionSettings = new IniCollectionSettings(IniCollectionMode.IndexOnly);
		private static readonly IniCollectionSettings defaultCollectionSettings = new IniCollectionSettings(IniCollectionMode.Normal);

		public static void Serialize(object Object, string Filename)
		{
			IniFile.Save(Serialize(Object), Filename);
		}

		public static void Serialize(object Object, TypeConverter Converter, string Filename)
		{
			IniFile.Save(Serialize(Object, Converter), Filename);
		}

		public static void Serialize(object Object, IniCollectionSettings CollectionSettings, string Filename)
		{
			IniFile.Save(Serialize(Object, CollectionSettings), Filename);
		}

		public static void Serialize(object Object, IniCollectionSettings CollectionSettings, TypeConverter Converter, string Filename)
		{
			IniFile.Save(Serialize(Object, CollectionSettings, Converter), Filename);
		}

		public static IniDictionary Serialize(object Object)
		{
			return Serialize(Object, initialCollectionSettings, (TypeConverter)null);
		}

		public static IniDictionary Serialize(object Object, TypeConverter Converter)
		{
			return Serialize(Object, initialCollectionSettings, Converter);
		}

		public static IniDictionary Serialize(object Object, IniCollectionSettings CollectionSettings)
		{
			return Serialize(Object, CollectionSettings, (TypeConverter)null);
		}

		public static IniDictionary Serialize(object Object, IniCollectionSettings CollectionSettings, TypeConverter Converter)
		{
			IniDictionary ini = new IniDictionary() { { string.Empty, new IniGroup() } };
			SerializeInternal("value", Object, ini, string.Empty, true, CollectionSettings, Converter);
			return ini;
		}

		private static void SerializeInternal(string name, object value, IniDictionary ini, string groupName, bool rootObject, IniCollectionSettings collectionSettings, TypeConverter converter)
		{
			IniGroup group = ini[groupName];
			if (value == null || value == DBNull.Value) return;
			if (!value.GetType().IsComplexType(converter))
			{
				group.Add(name, value.ConvertToString(converter));
				return;
			}
			if (value is IList)
			{
				int i = collectionSettings.StartIndex;
				switch (collectionSettings.Mode)
				{
					case IniCollectionMode.Normal:
						foreach (object item in (IList)value)
							SerializeInternal(name + "[" + (i++).ConvertToString(collectionSettings.KeyConverter) + "]", item, ini, groupName, false, defaultCollectionSettings, collectionSettings.ValueConverter);
						return;
					case IniCollectionMode.IndexOnly:
						foreach (object item in (IList)value)
							SerializeInternal((i++).ConvertToString(collectionSettings.KeyConverter), item, ini, groupName, false, defaultCollectionSettings, collectionSettings.ValueConverter);
						return;
					case IniCollectionMode.NoSquareBrackets:
						foreach (object item in (IList)value)
							SerializeInternal(name + (i++).ConvertToString(collectionSettings.KeyConverter), item, ini, groupName, false, defaultCollectionSettings, collectionSettings.ValueConverter);
						return;
					case IniCollectionMode.SingleLine:
						List<string> line = new List<string>();
						foreach (object item in (IList)value)
							line.Add(item.ConvertToString(collectionSettings.ValueConverter));
						group.Add(name, string.Join(collectionSettings.Format, line.ToArray()));
						return;
				}
			}
			if (value is IDictionary)
			{
				switch (collectionSettings.Mode)
				{
					case IniCollectionMode.Normal:
						foreach (DictionaryEntry item in (IDictionary)value)
							SerializeInternal(name + "[" + item.Key.ConvertToString(collectionSettings.KeyConverter) + "]", item.Value, ini, groupName, false, defaultCollectionSettings, collectionSettings.ValueConverter);
						return;
					case IniCollectionMode.IndexOnly:
						foreach (DictionaryEntry item in (IDictionary)value)
							SerializeInternal(item.Key.ConvertToString(collectionSettings.KeyConverter), item.Value, ini, groupName, false, defaultCollectionSettings, collectionSettings.ValueConverter);
						return;
					case IniCollectionMode.NoSquareBrackets:
						foreach (DictionaryEntry item in (IDictionary)value)
							SerializeInternal(name + item.Key.ConvertToString(collectionSettings.KeyConverter), item.Value, ini, groupName, false, defaultCollectionSettings, collectionSettings.ValueConverter);
						return;
					case IniCollectionMode.SingleLine:
						throw new InvalidOperationException("Cannot serialize IDictionary with IniCollectionMode.SingleLine!");
				}
			}
			string newgroup = groupName;
			if (!rootObject)
			{
				if (!string.IsNullOrEmpty(newgroup))
					newgroup += '.';
				newgroup += name;
				ini.Add(newgroup, new Dictionary<string, string>());
			}
			foreach (MemberInfo member in value.GetType().GetMembers(BindingFlags.Public | BindingFlags.Instance))
			{
				if (Attribute.GetCustomAttribute(member, typeof(IniIgnoreAttribute), true) != null)
					continue;
				string membername = member.Name;
				if (Attribute.GetCustomAttribute(member, typeof(IniNameAttribute), true) != null)
					membername = ((IniNameAttribute)Attribute.GetCustomAttribute(member, typeof(IniNameAttribute), true)).Name;
				object item;
				object defval;
				switch (member.MemberType)
				{
					case MemberTypes.Field:
						FieldInfo field = (FieldInfo)member;
						item = field.GetValue(value);
						defval = field.FieldType.GetDefaultValue();
						break;
					case MemberTypes.Property:
						PropertyInfo property = (PropertyInfo)member;
						defval = property.PropertyType.GetDefaultValue();
						if (property.GetIndexParameters().Length > 0) continue;
						MethodInfo getmethod = property.GetGetMethod();
						if (getmethod == null) continue;
						item = getmethod.Invoke(value, null);
						break;
					default:
						continue;
				}
				DefaultValueAttribute defattr = (DefaultValueAttribute)Attribute.GetCustomAttribute(member, typeof(DefaultValueAttribute), true);
				if (defattr != null)
					defval = defattr.Value;
				if (Attribute.GetCustomAttribute(member, typeof(IniAlwaysIncludeAttribute), true) != null || !object.Equals(item, defval))
				{
					IniCollectionSettings settings = defaultCollectionSettings;
					IniCollectionAttribute collattr = (IniCollectionAttribute)Attribute.GetCustomAttribute(member, typeof(IniCollectionAttribute));
					if (collattr != null)
						settings = collattr.Settings;
					TypeConverter conv = null;
					TypeConverterAttribute convattr = (TypeConverterAttribute)Attribute.GetCustomAttribute(member, typeof(TypeConverterAttribute));
					if (convattr != null)
						conv = (TypeConverter)Activator.CreateInstance(Type.GetType(convattr.ConverterTypeName));
					SerializeInternal(membername, item, ini, newgroup, false, settings, conv);
				}
			}
		}

		public static T Deserialize<T>(string filename)
		{
			return Deserialize<T>(IniFile.Load(filename), (TypeConverter)null);
		}

		public static T Deserialize<T>(string filename, TypeConverter Converter)
		{
			return Deserialize<T>(IniFile.Load(filename), Converter);
		}

		public static object Deserialize(Type Type, string Filename)
		{
			return Deserialize(Type, IniFile.Load(Filename), (TypeConverter)null);
		}

		public static object Deserialize(Type Type, string Filename, TypeConverter Converter)
		{
			return Deserialize(Type, IniFile.Load(Filename), Converter);
		}

		public static T Deserialize<T>(IniDictionary INI)
		{
			return (T)Deserialize(typeof(T), INI, (TypeConverter)null);
		}

		public static T Deserialize<T>(IniDictionary INI, TypeConverter Converter)
		{
			return (T)Deserialize(typeof(T), INI, Converter);
		}

		public static object Deserialize(Type Type, IniDictionary INI)
		{
			return Deserialize(Type, INI, initialCollectionSettings, null);
		}

		public static object Deserialize(Type Type, IniDictionary INI, TypeConverter Converter)
		{
			return Deserialize(Type, INI, initialCollectionSettings, Converter);
		}

		public static T Deserialize<T>(string filename, IniCollectionSettings CollectionSettings)
		{
			return Deserialize<T>(IniFile.Load(filename), CollectionSettings, null);
		}

		public static T Deserialize<T>(string filename, IniCollectionSettings CollectionSettings, TypeConverter Converter)
		{
			return Deserialize<T>(IniFile.Load(filename), CollectionSettings, Converter);
		}

		public static object Deserialize(Type Type, string Filename, IniCollectionSettings CollectionSettings)
		{
			return Deserialize(Type, IniFile.Load(Filename), CollectionSettings, null);
		}

		public static object Deserialize(Type Type, string Filename, IniCollectionSettings CollectionSettings, TypeConverter Converter)
		{
			return Deserialize(Type, IniFile.Load(Filename), CollectionSettings, Converter);
		}

		public static T Deserialize<T>(IniDictionary INI, IniCollectionSettings CollectionSettings)
		{
			return (T)Deserialize(typeof(T), INI, CollectionSettings, null);
		}

		public static T Deserialize<T>(IniDictionary INI, IniCollectionSettings CollectionSettings, TypeConverter Converter)
		{
			return (T)Deserialize(typeof(T), INI, CollectionSettings, Converter);
		}

		public static object Deserialize(Type Type, IniDictionary INI, IniCollectionSettings CollectionSettings)
		{
			return Deserialize(Type, INI, CollectionSettings, null);
		}

		public static object Deserialize(Type Type, IniDictionary INI, IniCollectionSettings CollectionSettings, TypeConverter Converter)
		{
			object Object;
			IniDictionary ini = new IniDictionary();
			ini = IniFile.Combine(ini, INI);
			Object = DeserializeInternal("value", Type, Type.GetDefaultValue(), ini, string.Empty, true, CollectionSettings, Converter);
			return Object;
		}

		private static object DeserializeInternal(string name, Type type, object defaultvalue, IniDictionary ini, string groupName, bool rootObject, IniCollectionSettings collectionSettings, TypeConverter converter)
		{
			string fullname = groupName;
			if (!rootObject)
			{
				if (!string.IsNullOrEmpty(fullname))
					fullname += '.';
				fullname += name;
			}
			if (!ini.ContainsKey(groupName)) return defaultvalue;
			Dictionary<string, string> group = ini[groupName];
			if (!type.IsComplexType(converter))
			{
				if (group.ContainsKey(name))
				{
					object converted = type.ConvertFromString(group[name], converter);
					group.Remove(name);
					if (converted != null)
						return converted;
				}
				return defaultvalue;
			}
			Type generictype;
			if (type.IsArray)
			{
				Type valuetype = type.GetElementType();
				int maxind = int.MinValue;
				TypeConverter keyconverter = collectionSettings.KeyConverter ?? new Int32Converter();
				if (!IsComplexType(valuetype, collectionSettings.ValueConverter))
				{
					switch (collectionSettings.Mode)
					{
						case IniCollectionMode.Normal:
							foreach (IniNameValue item in group)
								if (item.Key.StartsWith(name + "["))
								{
									int key = (int)keyconverter.ConvertFromInvariantString(item.Key.Substring(name.Length + 1, item.Key.Length - (name.Length + 2)));
									maxind = Math.Max(key, maxind);
								}
							break;
						case IniCollectionMode.IndexOnly:
							foreach (IniNameValue item in group)
								if (keyconverter.IsValid(item.Key))
									maxind = Math.Max((int)keyconverter.ConvertFromInvariantString(item.Key), maxind);
							break;
						case IniCollectionMode.NoSquareBrackets:
							foreach (IniNameValue item in group)
								if (item.Key.StartsWith(name))
									if (keyconverter.IsValid(item.Key.Substring(name.Length)))
										maxind = Math.Max((int)keyconverter.ConvertFromInvariantString(item.Key.Substring(name.Length)), maxind);
							break;
						case IniCollectionMode.SingleLine:
							if (group.ContainsKey(name))
							{
								string[] items = group[name].Split(new[] { collectionSettings.Format }, StringSplitOptions.None);
								Array _obj = Array.CreateInstance(valuetype, items.Length);
								for (int i = 0; i < items.Length; i++)
									_obj.SetValue(valuetype.ConvertFromString(items[i], collectionSettings.ValueConverter), i);
								group.Remove(name);
								return _obj;
							}
							else
								return null;
					}
				}
				else
				{
					switch (collectionSettings.Mode)
					{
						case IniCollectionMode.Normal:
							foreach (IniNameGroup item in ini)
								if (item.Key.StartsWith(fullname + "["))
								{
									int key = (int)keyconverter.ConvertFromInvariantString(item.Key.Substring(name.Length + 1, item.Key.Length - (name.Length + 2)));
									maxind = Math.Max(key, maxind);
								}
							break;
						case IniCollectionMode.IndexOnly:
							foreach (IniNameGroup item in ini)
								if (!string.IsNullOrEmpty(item.Key) && keyconverter.IsValid(item.Key))
									maxind = Math.Max((int)keyconverter.ConvertFromInvariantString(item.Key), maxind);
							break;
						case IniCollectionMode.NoSquareBrackets:
							foreach (IniNameGroup item in ini)
								if (item.Key.StartsWith(fullname))
									if (keyconverter.IsValid(item.Key.Substring(fullname.Length)))
										maxind = Math.Max((int)keyconverter.ConvertFromInvariantString(item.Key.Substring(fullname.Length)), maxind);
							break;
						case IniCollectionMode.SingleLine:
							throw new InvalidOperationException("Cannot deserialize type " + valuetype + " with IniCollectionMode.SingleLine!");
					}
				}
				if (maxind == int.MinValue) return Array.CreateInstance(valuetype, 0);
				int length = maxind + 1 - (collectionSettings.Mode == IniCollectionMode.SingleLine ? 0 : collectionSettings.StartIndex);
				Array obj = Array.CreateInstance(valuetype, length);
				if (!IsComplexType(valuetype, collectionSettings.ValueConverter))
					switch (collectionSettings.Mode)
					{
						case IniCollectionMode.Normal:
							for (int i = 0; i < length; i++)
							{
								string keyname = name + "[" + keyconverter.ConvertToInvariantString(i + collectionSettings.StartIndex) + "]";
								if (group.ContainsKey(keyname))
								{
									obj.SetValue(valuetype.ConvertFromString(group[keyname], collectionSettings.ValueConverter), i);
									group.Remove(keyname);
								}
								else
									obj.SetValue(valuetype.GetDefaultValue(), i);
							}
							break;
						case IniCollectionMode.IndexOnly:
							for (int i = 0; i < length; i++)
							{
								string keyname = keyconverter.ConvertToInvariantString(i + collectionSettings.StartIndex);
								if (group.ContainsKey(keyname))
								{
									obj.SetValue(valuetype.ConvertFromString(group[keyname], collectionSettings.ValueConverter), i);
									group.Remove(keyname);
								}
								else
									obj.SetValue(valuetype.GetDefaultValue(), i);
							}
							break;
						case IniCollectionMode.NoSquareBrackets:
							for (int i = 0; i < length; i++)
							{
								string keyname = name + keyconverter.ConvertToInvariantString(i + collectionSettings.StartIndex);
								if (group.ContainsKey(keyname))
								{
									obj.SetValue(valuetype.ConvertFromString(group[keyname], collectionSettings.ValueConverter), i);
									group.Remove(keyname);
								}
								else
									obj.SetValue(valuetype.GetDefaultValue(), i);
							}
							break;
					}
				else
					switch (collectionSettings.Mode)
					{
						case IniCollectionMode.Normal:
							for (int i = 0; i < length; i++)
								obj.SetValue(DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, fullname + "[" + keyconverter.ConvertToInvariantString(i + collectionSettings.StartIndex) + "]", true, defaultCollectionSettings, collectionSettings.ValueConverter), i);
							break;
						case IniCollectionMode.IndexOnly:
							for (int i = 0; i < length; i++)
								obj.SetValue(DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, keyconverter.ConvertToInvariantString(i + collectionSettings.StartIndex), true, defaultCollectionSettings, collectionSettings.ValueConverter), i);
							break;
						case IniCollectionMode.NoSquareBrackets:
							for (int i = 0; i < length; i++)
								obj.SetValue(DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, fullname + keyconverter.ConvertToInvariantString(i + collectionSettings.StartIndex), true, defaultCollectionSettings, collectionSettings.ValueConverter), i);
							break;
					}
				return obj;
			}
			if (ImplementsGenericDefinition(type, typeof(IList<>), out generictype))
			{
				object obj = Activator.CreateInstance(type);
				Type valuetype = generictype.GetGenericArguments()[0];
				CollectionDeserializer deserializer = (CollectionDeserializer)Activator.CreateInstance(typeof(ListDeserializer<>).MakeGenericType(valuetype));
				deserializer.Deserialize(obj, group, groupName, collectionSettings, name, ini, fullname);
				return obj;
			}
			if (type.ImplementsGenericDefinition(typeof(IDictionary<,>), out generictype))
			{
				object obj = Activator.CreateInstance(type);
				Type keytype = generictype.GetGenericArguments()[0];
				Type valuetype = generictype.GetGenericArguments()[1];
				if (keytype.IsComplexType(collectionSettings.KeyConverter)) return obj;
				CollectionDeserializer deserializer = (CollectionDeserializer)Activator.CreateInstance(typeof(DictionaryDeserializer<,>).MakeGenericType(keytype, valuetype));
				deserializer.Deserialize(obj, group, groupName, collectionSettings, name, ini, fullname);
				return obj;
			}
			object result = Activator.CreateInstance(type);
			MemberInfo collection = null;
			foreach (MemberInfo member in type.GetMembers(BindingFlags.Public | BindingFlags.Instance))
			{
				if (Attribute.GetCustomAttribute(member, typeof(IniIgnoreAttribute), true) != null)
					continue;
				string membername = member.Name;
				if (Attribute.GetCustomAttribute(member, typeof(IniNameAttribute), true) != null)
					membername = ((IniNameAttribute)Attribute.GetCustomAttribute(member, typeof(IniNameAttribute), true)).Name;
				IniCollectionSettings colset = defaultCollectionSettings;
				IniCollectionAttribute colattr = (IniCollectionAttribute)Attribute.GetCustomAttribute(member, typeof(IniCollectionAttribute), true);
				if (colattr != null)
					colset = colattr.Settings;
				TypeConverter conv = null;
				TypeConverterAttribute convattr = (TypeConverterAttribute)Attribute.GetCustomAttribute(member, typeof(TypeConverterAttribute), true);
				if (convattr != null)
					conv = (TypeConverter)Activator.CreateInstance(Type.GetType(convattr.ConverterTypeName));
				switch (member.MemberType)
				{
					case MemberTypes.Field:
						FieldInfo field = (FieldInfo)member;
						if (colset.Mode == IniCollectionMode.IndexOnly && typeof(ICollection).IsAssignableFrom(field.FieldType))
						{
							if (collection != null) throw new Exception("IniCollectionMode.IndexOnly cannot be used on multiple members of a Type.");
							collection = member;
							continue;
						}
						object defval = field.FieldType.GetDefaultValue();
						DefaultValueAttribute defattr = (DefaultValueAttribute)Attribute.GetCustomAttribute(member, typeof(DefaultValueAttribute), true);
						if (defattr != null)
							defval = defattr.Value;
						field.SetValue(result, DeserializeInternal(membername, field.FieldType, defval, ini, fullname, false, colset, conv));
						break;
					case MemberTypes.Property:
						PropertyInfo property = (PropertyInfo)member;
						if (property.GetIndexParameters().Length > 0) continue;
						if (colset.Mode == IniCollectionMode.IndexOnly && typeof(ICollection).IsAssignableFrom(property.PropertyType))
						{
							if (collection != null) throw new Exception("IniCollectionMode.IndexOnly cannot be used on multiple members of a Type.");
							collection = member;
							continue;
						}
						defval = property.PropertyType.GetDefaultValue();
						defattr = (DefaultValueAttribute)Attribute.GetCustomAttribute(member, typeof(DefaultValueAttribute), true);
						if (defattr != null)
							defval = defattr.Value;
						object propval = DeserializeInternal(membername, property.PropertyType, defval, ini, fullname, false, colset, conv);
						MethodInfo setmethod = property.GetSetMethod();
						if (setmethod == null) continue;
						setmethod.Invoke(result, new object[] { propval });
						break;
				}
			}
			if (collection != null)
				switch (collection.MemberType)
				{
					case MemberTypes.Field:
						FieldInfo field = (FieldInfo)collection;
						field.SetValue(result, DeserializeInternal(collection.Name, field.FieldType, field.FieldType.GetDefaultValue(), ini, fullname, false, ((IniCollectionAttribute)Attribute.GetCustomAttribute(collection, typeof(IniCollectionAttribute), true)).Settings, null));
						break;
					case MemberTypes.Property:
						PropertyInfo property = (PropertyInfo)collection;
						object propval = DeserializeInternal(collection.Name, property.PropertyType, property.PropertyType.GetDefaultValue(), ini, fullname, false, ((IniCollectionAttribute)Attribute.GetCustomAttribute(collection, typeof(IniCollectionAttribute), true)).Settings, null);
						MethodInfo setmethod = property.GetSetMethod();
						if (setmethod == null) break;
						setmethod.Invoke(result, new object[] { propval });
						break;
				}
			ini.Remove(fullname);
			return result;
		}

		private static object GetDefaultValue(this Type type)
		{
			if (type.IsEnum)
				return Activator.CreateInstance(type);
			switch (Type.GetTypeCode(type))
			{
				case TypeCode.Boolean:
					return default(bool);
				case TypeCode.Byte:
					return default(byte);
				case TypeCode.Char:
					return default(char);
				case TypeCode.DBNull:
					return default(DBNull);
				case TypeCode.DateTime:
					return default(DateTime);
				case TypeCode.Decimal:
					return default(decimal);
				case TypeCode.Double:
					return default(double);
				case TypeCode.Empty:
					return null;
				case TypeCode.Int16:
					return default(short);
				case TypeCode.Int32:
					return default(int);
				case TypeCode.Int64:
					return default(long);
				case TypeCode.SByte:
					return default(sbyte);
				case TypeCode.Single:
					return default(float);
				case TypeCode.String:
					return default(string);
				case TypeCode.UInt16:
					return default(ushort);
				case TypeCode.UInt32:
					return default(uint);
				case TypeCode.UInt64:
					return default(ulong);
				default:
					if (type.IsValueType)
						return Activator.CreateInstance(type);
					return null;
			}
		}

		private static bool IsComplexType(this Type type, TypeConverter converter)
		{
			switch (Type.GetTypeCode(type))
			{
				case TypeCode.Object:
					if (converter == null)
						converter = TypeDescriptor.GetConverter(type);
					if (converter != null && !(converter is ComponentConverter) && converter.GetType() != typeof(TypeConverter))
						if (converter.CanConvertTo(typeof(string)) & converter.CanConvertFrom(typeof(string)))
							return false;
					if (type.GetType() == typeof(Type))
						return false;
					return true;
				default:
					return false;
			}
		}

		private static string ConvertToString(this object @object, TypeConverter converter)
		{
			if (@object is string) return (string)@object;
			if (@object is Enum) return @object.ToString();
			if (converter == null)
				converter = TypeDescriptor.GetConverter(@object);
			if (converter != null && !(converter is ComponentConverter) && converter.GetType() != typeof(TypeConverter))
				if (converter.CanConvertTo(typeof(string)))
					return converter.ConvertToInvariantString(@object);
			if (@object is Type)
				return ((Type)@object).AssemblyQualifiedName;
			return null;
		}

		private static object ConvertFromString(this Type type, string value, TypeConverter converter)
		{
			if (converter == null)
				converter = TypeDescriptor.GetConverter(type);
			if (converter != null && !(converter is ComponentConverter) && converter.GetType() != typeof(TypeConverter))
				if (converter.CanConvertFrom(typeof(string)))
					return converter.ConvertFromInvariantString(value);
			if (type == typeof(Type))
				return Type.GetType(value);
			return type.GetDefaultValue();
		}

		private static bool ImplementsGenericDefinition(this Type type, Type genericInterfaceDefinition, out Type implementingType)
		{
			if (type.IsInterface)
			{
				if (type.IsGenericType)
				{
					Type interfaceDefinition = type.GetGenericTypeDefinition();
					if (genericInterfaceDefinition == interfaceDefinition)
					{
						implementingType = type;
						return true;
					}
				}
			}
			foreach (Type i in type.GetInterfaces())
			{
				if (i.IsGenericType)
				{
					Type interfaceDefinition = i.GetGenericTypeDefinition();

					if (genericInterfaceDefinition == interfaceDefinition)
					{
						implementingType = i;
						return true;
					}
				}
			}
			implementingType = null;
			return false;
		}

		private abstract class CollectionDeserializer
		{
			public abstract void Deserialize(object listObj, IniGroup group, string groupName, IniCollectionSettings collectionSettings, string name, IniDictionary ini, string fullname);
		}

		private sealed class ListDeserializer<T> : CollectionDeserializer
		{
			public override void Deserialize(object listObj, IniGroup group, string groupName, IniCollectionSettings collectionSettings, string name, IniDictionary ini, string fullname)
			{
				Type valuetype = typeof(T);
				IList<T> list = (IList<T>)listObj;
				int maxind = int.MinValue;
				TypeConverter keyconverter = collectionSettings.KeyConverter ?? new Int32Converter();
				if (!IsComplexType(valuetype, collectionSettings.ValueConverter))
				{
					switch (collectionSettings.Mode)
					{
						case IniCollectionMode.Normal:
							foreach (IniNameValue item in group)
								if (item.Key.StartsWith(name + "["))
								{
									int key = (int)keyconverter.ConvertFromInvariantString(item.Key.Substring(name.Length + 1, item.Key.Length - (name.Length + 2)));
									maxind = Math.Max(key, maxind);
								}
							break;
						case IniCollectionMode.IndexOnly:
							foreach (IniNameValue item in group)
								if (keyconverter.IsValid(item.Key))
									maxind = Math.Max((int)keyconverter.ConvertFromInvariantString(item.Key), maxind);
							break;
						case IniCollectionMode.NoSquareBrackets:
							foreach (IniNameValue item in group)
								if (item.Key.StartsWith(name))
									if (keyconverter.IsValid(item.Key.Substring(name.Length)))
										maxind = Math.Max((int)keyconverter.ConvertFromInvariantString(item.Key.Substring(name.Length)), maxind);
							break;
						case IniCollectionMode.SingleLine:
							if (group.ContainsKey(name))
							{
								string[] items = group[name].Split(new[] { collectionSettings.Format }, StringSplitOptions.None);
								for (int i = 0; i < items.Length; i++)
									list.Add((T)valuetype.ConvertFromString(items[i], collectionSettings.ValueConverter));
								group.Remove(name);
							}
							break;
					}
				}
				else
				{
					switch (collectionSettings.Mode)
					{
						case IniCollectionMode.Normal:
							foreach (IniNameGroup item in ini)
								if (item.Key.StartsWith(fullname + "["))
								{
									int key = (int)keyconverter.ConvertFromInvariantString(item.Key.Substring(name.Length + 1, item.Key.Length - (name.Length + 2)));
									maxind = Math.Max(key, maxind);
								}
							break;
						case IniCollectionMode.IndexOnly:
							foreach (IniNameGroup item in ini)
								if (!string.IsNullOrEmpty(item.Key) && keyconverter.IsValid(item.Key))
									maxind = Math.Max((int)keyconverter.ConvertFromInvariantString(item.Key), maxind);
							break;
						case IniCollectionMode.NoSquareBrackets:
							foreach (IniNameGroup item in ini)
								if (item.Key.StartsWith(fullname))
									if (keyconverter.IsValid(item.Key.Substring(name.Length)))
										maxind = Math.Max((int)keyconverter.ConvertFromInvariantString(item.Key.Substring(name.Length)), maxind);
							break;
						case IniCollectionMode.SingleLine:
							throw new InvalidOperationException("Cannot deserialize type " + valuetype + " with IniCollectionMode.SingleLine!");
					}
				}
				if (maxind == int.MinValue) return;
				int length = maxind + 1 - (collectionSettings.Mode == IniCollectionMode.SingleLine ? 0 : collectionSettings.StartIndex);
				if (!IsComplexType(valuetype, collectionSettings.ValueConverter))
					switch (collectionSettings.Mode)
					{
						case IniCollectionMode.Normal:
							for (int i = 0; i < length; i++)
							{
								string keyname = name + "[" + keyconverter.ConvertToInvariantString(i + collectionSettings.StartIndex) + "]";
								if (group.ContainsKey(keyname))
								{
									list.Add((T)valuetype.ConvertFromString(group[keyname], collectionSettings.ValueConverter));
									group.Remove(keyname);
								}
								else
									list.Add((T)valuetype.GetDefaultValue());
							}
							break;
						case IniCollectionMode.IndexOnly:
							for (int i = 0; i < length; i++)
							{
								string keyname = keyconverter.ConvertToInvariantString(i + collectionSettings.StartIndex);
								if (group.ContainsKey(keyname))
								{
									list.Add((T)valuetype.ConvertFromString(group[keyname], collectionSettings.ValueConverter));
									group.Remove(keyname);
								}
								else
									list.Add((T)valuetype.GetDefaultValue());
							}
							break;
						case IniCollectionMode.NoSquareBrackets:
							for (int i = 0; i < length; i++)
							{
								string keyname = name + keyconverter.ConvertToInvariantString(i + collectionSettings.StartIndex);
								if (group.ContainsKey(keyname))
								{
									list.Add((T)valuetype.ConvertFromString(group[keyname], collectionSettings.ValueConverter));
									group.Remove(keyname);
								}
								else
									list.Add((T)valuetype.GetDefaultValue());
							}
							break;
					}
				else
					switch (collectionSettings.Mode)
					{
						case IniCollectionMode.Normal:
							for (int i = 0; i < length; i++)
								list.Add((T)DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, fullname + "[" + keyconverter.ConvertToInvariantString(i + collectionSettings.StartIndex) + "]", true, defaultCollectionSettings, collectionSettings.ValueConverter));
							break;
						case IniCollectionMode.IndexOnly:
							for (int i = 0; i < length; i++)
								list.Add((T)DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, keyconverter.ConvertToInvariantString(i + collectionSettings.StartIndex), true, defaultCollectionSettings, collectionSettings.ValueConverter));
							break;
						case IniCollectionMode.NoSquareBrackets:
							for (int i = 0; i < length; i++)
								list.Add((T)DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, fullname + keyconverter.ConvertToInvariantString(i + collectionSettings.StartIndex).ToString(), true, defaultCollectionSettings, collectionSettings.ValueConverter));
							break;
					}
			}
		}

		private sealed class DictionaryDeserializer<TKey, TValue> : CollectionDeserializer
		{
			public override void Deserialize(object listObj, IniGroup group, string groupName, IniCollectionSettings collectionSettings, string name, IniDictionary ini, string fullname)
			{
				Type keytype = typeof(TKey);
				Type valuetype = typeof(TValue);
				IDictionary<TKey, TValue> list = (IDictionary<TKey, TValue>)listObj;
				if (!valuetype.IsComplexType(collectionSettings.ValueConverter))
				{
					List<string> items = new List<string>();
					switch (collectionSettings.Mode)
					{
						case IniCollectionMode.Normal:
							foreach (IniNameValue item in group)
								if (item.Key.StartsWith(name + "["))
									items.Add(item.Key.Substring(name.Length + 1, item.Key.Length - (name.Length + 2)));
							break;
						case IniCollectionMode.IndexOnly:
							foreach (IniNameValue item in group)
								items.Add(item.Key);
							break;
						case IniCollectionMode.NoSquareBrackets:
							foreach (IniNameValue item in group)
								if (item.Key.StartsWith(name))
									items.Add(item.Key.Substring(name.Length));
							break;
						case IniCollectionMode.SingleLine:
							throw new InvalidOperationException("Cannot deserialize IDictionary<TKey, TValue> with IniCollectionMode.SingleLine!");
					}
					switch (collectionSettings.Mode)
					{
						case IniCollectionMode.Normal:
							foreach (string item in items)
							{
								list.Add((TKey)keytype.ConvertFromString(item, collectionSettings.KeyConverter), (TValue)valuetype.ConvertFromString(group[name + "[" + item + "]"], collectionSettings.ValueConverter));
								group.Remove(name + "[" + item + "]");
							}
							break;
						case IniCollectionMode.IndexOnly:
							foreach (string item in items)
							{
								list.Add((TKey)keytype.ConvertFromString(item, collectionSettings.KeyConverter), (TValue)valuetype.ConvertFromString(group[item], collectionSettings.ValueConverter));
								group.Remove(item);
							}
							break;
						case IniCollectionMode.NoSquareBrackets:
							foreach (string item in items)
							{
								list.Add((TKey)keytype.ConvertFromString(item, collectionSettings.KeyConverter), (TValue)valuetype.ConvertFromString(group[name + item], collectionSettings.ValueConverter));
								group.Remove(name + item);
							}
							break;
					}
				}
				else
				{
					List<string> items = new List<string>();
					switch (collectionSettings.Mode)
					{
						case IniCollectionMode.Normal:
							foreach (IniNameGroup item in ini)
								if (item.Key.StartsWith(name + "["))
									items.Add(item.Key.Substring(name.Length + 1, item.Key.Length - (name.Length + 2)));
							break;
						case IniCollectionMode.IndexOnly:
							foreach (IniNameGroup item in ini)
								if (item.Key != groupName)
									items.Add(item.Key);
							break;
						case IniCollectionMode.NoSquareBrackets:
							foreach (IniNameGroup item in ini)
								if (item.Key.StartsWith(name))
									items.Add(item.Key.Substring(name.Length));
							break;
						case IniCollectionMode.SingleLine:
							throw new InvalidOperationException("Cannot deserialize IDictionary<TKey, TValue> with IniCollectionMode.SingleLine!");
					}
					switch (collectionSettings.Mode)
					{
						case IniCollectionMode.Normal:
							foreach (string item in items)
								list.Add((TKey)keytype.ConvertFromString(item, collectionSettings.KeyConverter), (TValue)DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, name + "[" + item + "]", true, defaultCollectionSettings, collectionSettings.ValueConverter));
							break;
						case IniCollectionMode.IndexOnly:
							foreach (string item in items)
								list.Add((TKey)keytype.ConvertFromString(item, collectionSettings.KeyConverter), (TValue)DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, item, true, defaultCollectionSettings, collectionSettings.ValueConverter));
							break;
						case IniCollectionMode.NoSquareBrackets:
							foreach (string item in items)
								list.Add((TKey)keytype.ConvertFromString(item, collectionSettings.KeyConverter), (TValue)DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, name + item, true, defaultCollectionSettings, collectionSettings.ValueConverter));
							break;
					}
				}
			}
		}
	}

	[AttributeUsage(AttributeTargets.Property | AttributeTargets.Field, Inherited = true, AllowMultiple = false)]
	public sealed class IniIgnoreAttribute : Attribute { }

	public class IniCollectionSettings
	{
		public IniCollectionSettings(IniCollectionMode mode)
		{
			Mode = mode;
		}

		public IniCollectionMode Mode { get; private set; }

		public string Format { get; set; }

		/// <summary>
		/// The index of the first item in the collection. Does not apply to Dictionary objects or <see cref="IniCollectionMode.SingleLine"/>.
		/// </summary>
		public int StartIndex { get; set; }

		/// <summary>
		/// A <see cref="System.ComponentModel.TypeConverter"/> used to convert indexes/keys to and from <see cref="System.String"/>.
		/// </summary>
		public TypeConverter KeyConverter { get; set; }

		/// <summary>
		/// A <see cref="System.ComponentModel.TypeConverter"/> used to convert values to and from <see cref="System.String"/>.
		/// </summary>
		public TypeConverter ValueConverter { get; set; }
	}

	public enum IniCollectionMode
	{
		/// <summary>
		/// The collection is serialized normally.
		/// </summary>
		Normal,
		/// <summary>
		/// The collection is serialized using only the index in the ini entry's key.
		/// </summary>
		IndexOnly,
		/// <summary>
		/// The collection is serialized using the collection's name and index in the ini entry's key, with no square brackets.
		/// </summary>
		NoSquareBrackets,
		/// <summary>
		/// The <paramref name="Format"/> property is used with <seealso cref="String.Join"/> to create the ini entry's value. The key is the collection's name.
		/// </summary>
		SingleLine
	}

	[AttributeUsage(AttributeTargets.Property | AttributeTargets.Field, Inherited = true, AllowMultiple = false)]
	public sealed class IniCollectionAttribute : Attribute
	{
		public IniCollectionAttribute(IniCollectionMode mode)
		{
			Settings = new IniCollectionSettings(mode);
		}

		public string Format
		{
			get { return Settings.Format; }
			set { Settings.Format = value; }
		}

		/// <summary>
		/// The index of the first item in the collection. Does not apply to Dictionary objects or <see cref="IniCollectionMode.SingleLine"/>.
		/// </summary>
		public int StartIndex
		{
			get { return Settings.StartIndex; }
			set { Settings.StartIndex = value; }
		}

		/// <summary>
		/// The <see cref="System.Type"/> of a <see cref="System.ComponentModel.TypeConverter"/> used to convert indexes/keys to and from <see cref="System.String"/>.
		/// </summary>
		public Type KeyConverter
		{
			get { return Settings.KeyConverter == null ? null : Settings.KeyConverter.GetType(); }
			set { Settings.KeyConverter = (TypeConverter)Activator.CreateInstance(value); }
		}

		/// <summary>
		/// The <see cref="System.Type"/> of a <see cref="System.ComponentModel.TypeConverter"/> used to convert values to and from <see cref="System.String"/>.
		/// </summary>
		public Type ValueConverter
		{
			get { return Settings.ValueConverter == null ? null : Settings.ValueConverter.GetType(); }
			set { Settings.ValueConverter = (TypeConverter)Activator.CreateInstance(value); }
		}

		public IniCollectionSettings Settings { get; private set; }
	}

	[AttributeUsage(AttributeTargets.Property | AttributeTargets.Field, Inherited = true, AllowMultiple = false)]
	public sealed class IniNameAttribute : Attribute
	{
		public IniNameAttribute(string name)
		{
			Name = name;
		}

		public string Name { get; private set; }
	}

	[AttributeUsage(AttributeTargets.Property | AttributeTargets.Field, Inherited = true, AllowMultiple = false)]
	public sealed class IniAlwaysIncludeAttribute : Attribute { }
}