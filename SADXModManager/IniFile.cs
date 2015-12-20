using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Reflection;
using System.Text;
using IniDictionary = System.Collections.Generic.Dictionary<string, System.Collections.Generic.Dictionary<string, string>>;
using IniNameGroup = System.Collections.Generic.KeyValuePair<string, System.Collections.Generic.Dictionary<string, string>>;
using IniGroup = System.Collections.Generic.Dictionary<string, string>;
using IniNameValue = System.Collections.Generic.KeyValuePair<string, string>;

namespace IniSerializer
{
    public static class IniFile
    {
        private static readonly IniCollectionSettings initialCollectionSettings = new IniCollectionSettings(IniCollectionMode.IndexOnly);
        private static readonly IniCollectionSettings defaultCollectionSettings = new IniCollectionSettings(IniCollectionMode.Normal);

        public static IniDictionary Load(string filename)
        {
            IniDictionary result = new IniDictionary();
            IniGroup curent = new IniGroup();
            result.Add(string.Empty, curent);
            string curgroup = string.Empty;
            string[] fc = System.IO.File.ReadAllLines(filename);
            for (int i = 0; i < fc.Length; i++)
            {
                string line = fc[i];
                StringBuilder sb = new StringBuilder(line.Length);
                bool startswithbracket = false;
                int firstequals = -1;
                int endbracket = -1;
                for (int c = 0; c < line.Length; c++)
                    switch (line[c])
                    {
                        case '\\': // escape character
                            if (c + 1 == line.Length) goto default;
                            c++;
                            switch (line[c])
                            {
                                case 'n': // line feed
                                    sb.Append('\n');
                                    break;
                                case 'r': // carriage return
                                    sb.Append('\r');
                                    break;
                                default: // literal character
                                    sb.Append(line[c]);
                                    break;
                            }
                            break;
                        case '=':
                            if (firstequals == -1)
                                firstequals = sb.Length;
                            goto default;
                        case '[':
                            if (c == 0)
                                startswithbracket = true;
                            goto default;
                        case ']':
                            endbracket = sb.Length;
                            goto default;
                        case ';': // comment character, stop processing this line
                            c = line.Length;
                            break;
                        default:
                            sb.Append(line[c]);
                            break;
                    }
                line = sb.ToString();
                if (startswithbracket & endbracket != -1)
                {
                    curgroup = line.Substring(1, endbracket - 1);
                    curent = new IniGroup();
                    try
                    {
                        result.Add(curgroup, curent);
                    }
                    catch (ArgumentException ex)
                    {
                        throw new Exception("INI File error: Group \"" + curgroup + "\" already exists.\n" + filename + ":line " + i, ex);
                    }
                }
                else if (!IsNullOrWhiteSpace(line))
                {
                    string key;
                    string value = string.Empty;
                    if (firstequals > -1)
                    {
                        key = line.Substring(0, firstequals);
                        value = line.Substring(firstequals + 1);
                    }
                    else
                        key = line;
                    try
                    {
                        curent.Add(key, value);
                    }
                    catch (ArgumentException ex)
                    {
                        throw new Exception("INI File error: Value \"" + key + "\" already exists in group \"" + curgroup + "\".\n" + filename + ":line " + i, ex);
                    }
                }
            }
            return result;
        }

        public static void Save(IniDictionary INI, string filename)
        {
            List<string> result = new List<string>();
            foreach (IniNameGroup group in INI)
            {
                if (!string.IsNullOrEmpty(group.Key))
                    result.Add("[" + group.Key.Replace(@"\", @"\\").Replace("\n", @"\n").Replace("\r", @"\r").Replace(";", @"\;") + "]");
                foreach (IniNameValue value in group.Value)
                {
                    string escapedkey = value.Key.Replace(@"\", @"\\").Replace("=", @"\=").Replace("\n", @"\n").Replace("\r", @"\r").Replace(";", @"\;");
                    if (escapedkey.StartsWith("["))
                        escapedkey = escapedkey.Insert(0, @"\");
                    result.Add(escapedkey + "=" + value.Value.Replace(@"\", @"\\").Replace("\n", @"\n").Replace("\r", @"\r").Replace(";", @"\;"));
                }
            }
            System.IO.File.WriteAllLines(filename, result.ToArray());
        }

        public static IniDictionary Combine(IniDictionary dictA, IniDictionary dictB)
        {
            IniDictionary result = new IniDictionary();
            foreach (IniNameGroup group in dictA)
                result.Add(group.Key, new IniGroup(group.Value));
            foreach (IniNameGroup group in dictB)
                if (result.ContainsKey(group.Key))
                    foreach (IniNameValue item in group.Value)
                        result[group.Key][item.Key] = item.Value;
                else
                    result.Add(group.Key, new IniGroup(group.Value));
            return result;
        }

        public static void Serialize(object Object, string Filename)
        {
            Save(Serialize(Object), Filename);
        }

        public static void Serialize(object Object, IniCollectionSettings CollectionSettings, string Filename)
        {
            Save(Serialize(Object, CollectionSettings), Filename);
        }

        public static IniDictionary Serialize(object Object)
        {
            return Serialize(Object, initialCollectionSettings);
        }

        public static IniDictionary Serialize(object Object, IniCollectionSettings CollectionSettings)
        {
            IniDictionary ini = new IniDictionary() { { string.Empty, new IniGroup() } };
            SerializeInternal("value", Object, ini, string.Empty, true, CollectionSettings);
            return ini;
        }

        private static void SerializeInternal(string name, object value, IniDictionary ini, string groupName, bool rootObject, IniCollectionSettings collectionSettings)
        {
            IniGroup group = ini[groupName];
            if (value == null || value == DBNull.Value) return;
            if (!value.GetType().IsComplexType())
            {
                group.Add(name, value.ConvertToString());
                return;
            }
            if (value is IList)
            {
                int i = collectionSettings.StartIndex;
                switch (collectionSettings.Mode)
                {
                    case IniCollectionMode.Normal:
                        foreach (object item in (IList)value)
                            SerializeInternal(name + "[" + (i++).ToString() + "]", item, ini, groupName, false, defaultCollectionSettings);
                        return;
                    case IniCollectionMode.IndexOnly:
                        foreach (object item in (IList)value)
                            SerializeInternal((i++).ToString(), item, ini, groupName, false, defaultCollectionSettings);
                        return;
                    case IniCollectionMode.NoSquareBrackets:
                        foreach (object item in (IList)value)
                            SerializeInternal(name + (i++).ToString(), item, ini, groupName, false, defaultCollectionSettings);
                        return;
                    case IniCollectionMode.SingleLine:
                        List<string> line = new List<string>();
                        foreach (object item in (IList)value)
                            line.Add(item.ConvertToString());
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
                            SerializeInternal(name + "[" + item.Key.ConvertToString() + "]", item.Value, ini, groupName, false, defaultCollectionSettings);
                        return;
                    case IniCollectionMode.IndexOnly:
                        foreach (DictionaryEntry item in (IDictionary)value)
                            SerializeInternal(item.Key.ConvertToString(), item.Value, ini, groupName, false, defaultCollectionSettings);
                        return;
                    case IniCollectionMode.NoSquareBrackets:
                        foreach (DictionaryEntry item in (IDictionary)value)
                            SerializeInternal(name + item.Key.ConvertToString(), item.Value, ini, groupName, false, defaultCollectionSettings);
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
                    IniCollectionAttribute attr = (IniCollectionAttribute)Attribute.GetCustomAttribute(member, typeof(IniCollectionAttribute));
                    if (attr != null)
                        settings = attr.Settings;
                    SerializeInternal(membername, item, ini, newgroup, false, settings);
                }
            }
        }

        public static T Deserialize<T>(string filename)
        {
            return Deserialize<T>(Load(filename));
        }

        public static object Deserialize(Type Type, string Filename)
        {
            return Deserialize(Type, Load(Filename));
        }

        public static T Deserialize<T>(IniDictionary INI)
        {
            return (T)Deserialize(typeof(T), INI);
        }

        public static object Deserialize(Type Type, IniDictionary INI)
        {
            return Deserialize(Type, INI, initialCollectionSettings);
        }

        public static T Deserialize<T>(string filename, IniCollectionSettings CollectionSettings)
        {
            return Deserialize<T>(Load(filename), CollectionSettings);
        }

        public static object Deserialize(Type Type, string Filename, IniCollectionSettings CollectionSettings)
        {
            return Deserialize(Type, Load(Filename), CollectionSettings);
        }

        public static T Deserialize<T>(IniDictionary INI, IniCollectionSettings CollectionSettings)
        {
            return (T)Deserialize(typeof(T), INI, CollectionSettings);
        }

        public static object Deserialize(Type Type, IniDictionary INI, IniCollectionSettings CollectionSettings)
        {
            object Object;
            IniDictionary ini = new IniDictionary();
            ini = Combine(ini, INI);
            Object = DeserializeInternal("value", Type, Type.GetDefaultValue(), ini, string.Empty, true, CollectionSettings);
            return Object;
        }

        private static object DeserializeInternal(string name, Type type, object defaultvalue, IniDictionary ini, string groupName, bool rootObject, IniCollectionSettings collectionSettings)
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
            if (!type.IsComplexType())
            {
                if (group.ContainsKey(name))
                {
                    object converted = type.ConvertFromString(group[name]);
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
                if (!IsComplexType(valuetype))
                {
                    switch (collectionSettings.Mode)
                    {
                        case IniCollectionMode.Normal:
                            foreach (IniNameValue item in group)
                                if (item.Key.StartsWith(name + "["))
                                {
                                    int key = int.Parse(item.Key.Substring(name.Length + 1, item.Key.Length - (name.Length + 2)));
                                    maxind = Math.Max(key, maxind);
                                }
                            break;
                        case IniCollectionMode.IndexOnly:
                            foreach (IniNameValue item in group)
                            {
                                int key;
                                if (int.TryParse(item.Key, out key))
                                    maxind = Math.Max(key, maxind);
                            }
                            break;
                        case IniCollectionMode.NoSquareBrackets:
                            foreach (IniNameValue item in group)
                                if (item.Key.StartsWith(name))
                                {
                                    int key;
                                    if (int.TryParse(item.Key.Substring(name.Length), out key))
                                        maxind = Math.Max(key, maxind);
                                }
                            break;
                        case IniCollectionMode.SingleLine:
                            string[] items = group[name].Split(new[] { collectionSettings.Format }, StringSplitOptions.None);
                            Array _obj = Array.CreateInstance(valuetype, items.Length);
                            for (int i = 0; i < items.Length; i++)
                                _obj.SetValue(valuetype.ConvertFromString(items[i]), i);
                            group.Remove(name);
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
                                    int key = int.Parse(item.Key.Substring(fullname.Length + 1, item.Key.Length - (fullname.Length + 2)));
                                    maxind = Math.Max(key, maxind);
                                }
                            break;
                        case IniCollectionMode.IndexOnly:
                            foreach (IniNameGroup item in ini)
                            {
                                int key;
                                if (int.TryParse(item.Key, out key))
                                    maxind = Math.Max(key, maxind);
                            }
                            break;
                        case IniCollectionMode.NoSquareBrackets:
                            foreach (IniNameGroup item in ini)
                                if (item.Key.StartsWith(fullname))
                                {
                                    int key = int.Parse(item.Key.Substring(fullname.Length));
                                    maxind = Math.Max(key, maxind);
                                }
                            break;
                        case IniCollectionMode.SingleLine:
                            throw new InvalidOperationException("Cannot deserialize type " + valuetype + " with IniCollectionMode.SingleLine!");
                    }
                }
                if (maxind == int.MinValue) return Array.CreateInstance(valuetype, 0);
                int length = maxind + 1 - (collectionSettings.Mode == IniCollectionMode.SingleLine ? 0 : collectionSettings.StartIndex);
                Array obj = Array.CreateInstance(valuetype, length);
                if (!IsComplexType(valuetype))
                    switch (collectionSettings.Mode)
                    {
                        case IniCollectionMode.Normal:
                            for (int i = 0; i < length; i++)
                                if (group.ContainsKey(name + "[" + (i + collectionSettings.StartIndex).ToString() + "]"))
                                {
                                    obj.SetValue(valuetype.ConvertFromString(group[name + "[" + (i + collectionSettings.StartIndex).ToString() + "]"]), i);
                                    group.Remove(name + "[" + (i + collectionSettings.StartIndex).ToString() + "]");
                                }
                                else
                                    obj.SetValue(valuetype.GetDefaultValue(), i);
                            break;
                        case IniCollectionMode.IndexOnly:
                            for (int i = 0; i < length; i++)
                                if (group.ContainsKey((i + collectionSettings.StartIndex).ToString()))
                                {
                                    obj.SetValue(valuetype.ConvertFromString(group[(i + collectionSettings.StartIndex).ToString()]), i);
                                    group.Remove((i + collectionSettings.StartIndex).ToString());
                                }
                                else
                                    obj.SetValue(valuetype.GetDefaultValue(), i);
                            break;
                        case IniCollectionMode.NoSquareBrackets:
                            for (int i = 0; i < length; i++)
                                if (group.ContainsKey(name + (i + collectionSettings.StartIndex).ToString()))
                                {
                                    obj.SetValue(valuetype.ConvertFromString(group[name + (i + collectionSettings.StartIndex).ToString()]), i);
                                    group.Remove(name + (i + collectionSettings.StartIndex).ToString());
                                }
                                else
                                    obj.SetValue(valuetype.GetDefaultValue(), i);
                            break;
                    }
                else
                    switch (collectionSettings.Mode)
                    {
                        case IniCollectionMode.Normal:
                            for (int i = 0; i < maxind; i++)
                                obj.SetValue(DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, fullname + "[" + (i + collectionSettings.StartIndex).ToString() + "]", true, defaultCollectionSettings), i);
                            break;
                        case IniCollectionMode.IndexOnly:
                            for (int i = 0; i < maxind; i++)
                                obj.SetValue(DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, (i + collectionSettings.StartIndex).ToString(), true, defaultCollectionSettings), i);
                            break;
                        case IniCollectionMode.NoSquareBrackets:
                            for (int i = 0; i < maxind; i++)
                                obj.SetValue(DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, fullname + (i + collectionSettings.StartIndex).ToString(), true, defaultCollectionSettings), i);
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
                if (keytype.IsComplexType()) return obj;
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
                        field.SetValue(result, DeserializeInternal(membername, field.FieldType, defval, ini, fullname, false, colset));
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
                        object propval = DeserializeInternal(membername, property.PropertyType, defval, ini, fullname, false, colset);
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
                        field.SetValue(result, DeserializeInternal(collection.Name, field.FieldType, field.FieldType.GetDefaultValue(), ini, fullname, false, ((IniCollectionAttribute)Attribute.GetCustomAttribute(collection, typeof(IniCollectionAttribute), true)).Settings));
                        break;
                    case MemberTypes.Property:
                        PropertyInfo property = (PropertyInfo)collection;
                        object propval = DeserializeInternal(collection.Name, property.PropertyType, property.PropertyType.GetDefaultValue(), ini, fullname, false, ((IniCollectionAttribute)Attribute.GetCustomAttribute(collection, typeof(IniCollectionAttribute), true)).Settings);
                        MethodInfo setmethod = property.GetSetMethod();
                        if (setmethod == null) break;
                        setmethod.Invoke(result, new object[] { propval });
                        break;
                }
            ini.Remove(rootObject ? string.Empty : name);
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

        private static bool IsComplexType(this Type type)
        {
            switch (Type.GetTypeCode(type))
            {
                case TypeCode.Object:
                    TypeConverter converter = TypeDescriptor.GetConverter(type);
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

        private static string ConvertToString(this object @object)
        {
            if (@object is string) return (string)@object;
            TypeConverter converter = TypeDescriptor.GetConverter(@object);
            if (converter != null && !(converter is ComponentConverter) && converter.GetType() != typeof(TypeConverter))
                if (converter.CanConvertTo(typeof(string)))
                    return converter.ConvertToInvariantString(@object);
            if (@object is Type)
                return ((Type)@object).AssemblyQualifiedName;
            return null;
        }

        private static object ConvertFromString(this Type type, string value)
        {
            TypeConverter converter = TypeDescriptor.GetConverter(type);
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

        private static bool IsNullOrWhiteSpace(string value)
        {
            if (string.IsNullOrEmpty(value))
                return true;
            for (int i = 0; i < value.Length; i++)
                if (!char.IsWhiteSpace(value[i]))
                    return false;
            return true;
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
                if (!IsComplexType(valuetype))
                {
                    switch (collectionSettings.Mode)
                    {
                        case IniCollectionMode.Normal:
                            foreach (IniNameValue item in group)
                                if (item.Key.StartsWith(name + "["))
                                {
                                    int key = int.Parse(item.Key.Substring(name.Length + 1, item.Key.Length - (name.Length + 2)));
                                    maxind = Math.Max(key, maxind);
                                }
                            break;
                        case IniCollectionMode.IndexOnly:
                            foreach (IniNameValue item in group)
                            {
                                int key;
                                if (int.TryParse(item.Key, out key))
                                    maxind = Math.Max(key, maxind);
                            }
                            break;
                        case IniCollectionMode.NoSquareBrackets:
                            foreach (IniNameValue item in group)
                                if (item.Key.StartsWith(name))
                                {
                                    int key;
                                    if (int.TryParse(item.Key.Substring(name.Length), out key))
                                        maxind = Math.Max(key, maxind);
                                }
                            break;
                        case IniCollectionMode.SingleLine:
                            string[] items = group[name].Split(new[] { collectionSettings.Format }, StringSplitOptions.None);
                            for (int i = 0; i < items.Length; i++)
                                list.Add((T)valuetype.ConvertFromString(items[i]));
                            group.Remove(name);
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
                                    int key = int.Parse(item.Key.Substring(fullname.Length + 1, item.Key.Length - (fullname.Length + 2)));
                                    maxind = Math.Max(key, maxind);
                                }
                            break;
                        case IniCollectionMode.IndexOnly:
                            foreach (IniNameGroup item in ini)
                            {
                                int key;
                                if (int.TryParse(item.Key, out key))
                                    maxind = Math.Max(key, maxind);
                            }
                            break;
                        case IniCollectionMode.NoSquareBrackets:
                            foreach (IniNameGroup item in ini)
                                if (item.Key.StartsWith(fullname))
                                {
                                    int key = int.Parse(item.Key.Substring(fullname.Length));
                                    maxind = Math.Max(key, maxind);
                                }
                            break;
                        case IniCollectionMode.SingleLine:
                            throw new InvalidOperationException("Cannot deserialize type " + valuetype + " with IniCollectionMode.SingleLine!");
                    }
                }
                if (maxind == int.MinValue) return;
                int length = maxind + 1 - (collectionSettings.Mode == IniCollectionMode.SingleLine ? 0 : collectionSettings.StartIndex);
                if (!IsComplexType(valuetype))
                    switch (collectionSettings.Mode)
                    {
                        case IniCollectionMode.Normal:
                            for (int i = 0; i < length; i++)
                                if (group.ContainsKey(name + "[" + (i + collectionSettings.StartIndex).ToString() + "]"))
                                {
                                    list.Add((T)valuetype.ConvertFromString(group[name + "[" + (i + collectionSettings.StartIndex).ToString() + "]"]));
                                    group.Remove(name + "[" + (i + collectionSettings.StartIndex).ToString() + "]");
                                }
                                else
                                    list.Add((T)valuetype.GetDefaultValue());
                            break;
                        case IniCollectionMode.IndexOnly:
                            for (int i = 0; i < length; i++)
                                if (group.ContainsKey((i + collectionSettings.StartIndex).ToString()))
                                {
                                    list.Add((T)valuetype.ConvertFromString(group[(i + collectionSettings.StartIndex).ToString()]));
                                    group.Remove((i + collectionSettings.StartIndex).ToString());
                                }
                                else
                                    list.Add((T)valuetype.GetDefaultValue());
                            break;
                        case IniCollectionMode.NoSquareBrackets:
                            for (int i = 0; i < length; i++)
                                if (group.ContainsKey(name + (i + collectionSettings.StartIndex).ToString()))
                                {
                                    list.Add((T)valuetype.ConvertFromString(group[name + (i + collectionSettings.StartIndex).ToString()]));
                                    group.Remove(name + (i + collectionSettings.StartIndex).ToString());
                                }
                                else
                                    list.Add((T)valuetype.GetDefaultValue());
                            break;
                    }
                else
                    switch (collectionSettings.Mode)
                    {
                        case IniCollectionMode.Normal:
                            for (int i = 0; i < maxind; i++)
                                list.Add((T)DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, fullname + "[" + (i + collectionSettings.StartIndex).ToString() + "]", true, defaultCollectionSettings));
                            break;
                        case IniCollectionMode.IndexOnly:
                            for (int i = 0; i < maxind; i++)
                                list.Add((T)DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, (i + collectionSettings.StartIndex).ToString(), true, defaultCollectionSettings));
                            break;
                        case IniCollectionMode.NoSquareBrackets:
                            for (int i = 0; i < maxind; i++)
                                list.Add((T)DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, fullname + (i + collectionSettings.StartIndex).ToString(), true, defaultCollectionSettings));
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
                if (!valuetype.IsComplexType())
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
                                list.Add((TKey)keytype.ConvertFromString(item), (TValue)valuetype.ConvertFromString(group[name + "[" + item + "]"]));
                                group.Remove(name + "[" + item + "]");
                            }
                            break;
                        case IniCollectionMode.IndexOnly:
                            foreach (string item in items)
                            {
                                list.Add((TKey)keytype.ConvertFromString(item), (TValue)valuetype.ConvertFromString(group[item]));
                                group.Remove(item);
                            }
                            break;
                        case IniCollectionMode.NoSquareBrackets:
                            foreach (string item in items)
                            {
                                list.Add((TKey)keytype.ConvertFromString(item), (TValue)valuetype.ConvertFromString(group[name + item]));
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
                                list.Add((TKey)keytype.ConvertFromString(item), (TValue)DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, name + "[" + item + "]", true, defaultCollectionSettings));
                            break;
                        case IniCollectionMode.IndexOnly:
                            foreach (string item in items)
                                list.Add((TKey)keytype.ConvertFromString(item), (TValue)DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, item, true, defaultCollectionSettings));
                            break;
                        case IniCollectionMode.NoSquareBrackets:
                            foreach (string item in items)
                                list.Add((TKey)keytype.ConvertFromString(item), (TValue)DeserializeInternal("value", valuetype, valuetype.GetDefaultValue(), ini, name + item, true, defaultCollectionSettings));
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

        public int StartIndex { get; set; }
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

        public int StartIndex
        {
            get { return Settings.StartIndex; }
            set { Settings.StartIndex = value; }
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