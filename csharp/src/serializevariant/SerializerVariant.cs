﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

namespace finalmq
{
    using VariantList = List<Variant>;
    using VariantStruct = List<NameValue>;

    class SerializerVariant : ParserConverter
    {
        private static string STR_VARVALUE = "finalmq.variant.VarValue";

        public SerializerVariant(Variant root, bool enumAsString = true, bool skipDefaultValues = false)
        {
            m_internal = new SerializerVariant.Internal(this, root, enumAsString);
            m_parserProcessDefaultValues = new ParserProcessDefaultValues(skipDefaultValues, m_internal);
            SetVisitor(m_parserProcessDefaultValues);
        }


        class Internal : IParserVisitor
        {
            public Internal(SerializerVariant outer, Variant root, bool enumAsString)
            {
                m_root = root;
                m_enumAsString = enumAsString;
                m_outer = outer;
            }

            public void NotifyError(string str, string message)
            {

            }
            public void StartStruct(MetaStruct stru)
            {
                m_root.SetData(new VariantStruct());
                m_stack.Add(m_root);
                m_current = m_stack.Last();
            }
            public void Finished()
            {

            }
            public void EnterStruct(MetaField field)
            {
                Debug.Assert(m_current != null);

                if (field.TypeName == STR_VARVALUE)
                {
                    //todo                    m_varValueToVariant = null;
                    Variant variant = new Variant();
                    m_current.Add(field.Name, variant);
                    //todo                    m_varValueToVariant = new VarValueToVariant(variant);
                    //todo                    m_outer.SetVisitor(m_varValueToVariant.GetVisitor());
                    //todo                    m_outer.m_parserProcessDefaultValues.SetVisitor(m_varValueToVariant.GetVisitor());
                    //                    m_varValueToVariant.SetExitNotification([this, &field]() {
                    //                        assert(m_varValueToVariant);
                    //m_outer.ParserConverter::setVisitor(*m_outer.m_parserProcessDefaultValues);
                    //m_outer.m_parserProcessDefaultValues->resetVarValueActive();
                    // m_outer.m_parserProcessDefaultValues->setVisitor(*this);
                    //m_varValueToVariant->convert();
                    //m_varValueToVariant->setExitNotification(-1);
                    //});
                }
                else
                {
                    if (m_current.VarType == (int)MetaTypeId.TYPE_STRUCT)
                    {
                        Variant variant = Variant.Create(new VariantStruct());
                        m_current.Add(field.Name, variant);
                        m_stack.Add(variant);
                    }
                    else
                    {
                        Debug.Assert(m_current.VarType == (int)MetaTypeId.TYPE_ARRAY_STRUCT);   // VariantList
                        Variant variant = Variant.Create(new VariantStruct());
                        m_current.Add(variant);
                        m_stack.Add(variant);
                    }
                    m_current = m_stack.Last();
                }
            }
            public void ExitStruct(MetaField field)
            {
                if (m_stack.Count != 0)
                {
                    m_stack.RemoveAt(m_stack.Count - 1);
                    if (m_stack.Count != 0)
                    {
                        m_current = m_stack.Last();
                    }
                    else
                    {
                        m_current = null;
                    }
                }
            }
            public void EnterArrayStruct(MetaField field)
            {
                if (m_stack.Count != 0)
                {
                    Debug.Assert(m_current != null);
                    if (m_current.VarType == (int)MetaTypeId.TYPE_STRUCT)
                    {
                        Variant variant = Variant.Create(new VariantList());
                        m_current.Add(field.Name, variant);
                        m_stack.Add(variant);
                    }
                    else
                    {
                        Debug.Assert(m_current.VarType == (int)MetaTypeId.TYPE_ARRAY_STRUCT);    //VariantList
                        Variant variant = Variant.Create(new VariantList());
                        m_current.Add(variant);
                        m_stack.Add(variant);
                    }
                    m_current = m_stack.Last();
                }
            }
            public void ExitArrayStruct(MetaField field)
            {
                if (m_stack.Count != 0)
                {
                    m_stack.RemoveAt(m_stack.Count - 1);
                    if (m_stack.Count != 0)
                    {
                        m_current = m_stack.Last();
                    }
                    else
                    {
                        m_current = null;
                    }
                }
            }
            public void EnterBool(MetaField field, bool value)
            {
                if (m_current != null)
                {
                    if (m_current.VarType == (int)MetaTypeId.TYPE_STRUCT)
                    {
                        m_current.Add(field.Name, value);
                    }
                    else
                    {
                        Debug.Assert(m_current.VarType == (int)MetaTypeId.TYPE_ARRAY_STRUCT);    // VariantList
                        m_current.Add(value);
                    }
                }
            }

            void Add<T>(MetaField field, T value)
            {
                if (m_current != null)
                {
                    if (m_current.VarType == (int)MetaTypeId.TYPE_STRUCT)
                    {
                        m_current.Add(field.Name, value);
                    }
                    else
                    {
                        Debug.Assert(m_current.VarType == (int)MetaTypeId.TYPE_ARRAY_STRUCT);  //VariantList
                        m_current.Add(value);
                    }
                }
            }


            public void EnterInt32(MetaField field, int value)
            {
                Add(field, value);
            }
            public void EnterUInt32(MetaField field, uint value)
            {
                Add(field, value);
            }
            public void EnterInt64(MetaField field, long value)
            {
                Add(field, value);
            }
            public void EnterUInt64(MetaField field, ulong value)
            {
                Add(field, value);
            }
            public void EnterFloat(MetaField field, float value)
            {
                Add(field, value);
            }
            public void EnterDouble(MetaField field, double value)
            {
                Add(field, value);
            }
            public void EnterString(MetaField field, string value)
            {
                Add(field, value);
            }
            public void EnterString(MetaField field, byte[] buffer, int offset, int size)
            {
                Add(field, Encoding.UTF8.GetString(buffer, offset, size));
            }
            public void EnterBytes(MetaField field, byte[] value)
            {
                Add(field, value);
            }
            public void EnterEnum(MetaField field, int value)
            {
                if (m_enumAsString)
                {
                    string name = MetaDataGlobal.Instance.GetEnumAliasByValue(field, value);
                    Add(field, name);
                }
                else
                {
                    Add(field, value);
                }
            }
            public void EnterEnum(MetaField field, string value)
            {
                if (m_enumAsString)
                {
                    Add(field, value);
                }
                else
                {
                    int v = MetaDataGlobal.Instance.GetEnumValueByName(field, value);
                    Add(field, v);
                }
            }
            public void EnterArrayBool(MetaField field, bool[] value)
            {
                Add(field, value);
            }
            public void EnterArrayInt32(MetaField field, int[] value)
            {
                Add(field, value);
            }
            public void EnterArrayUInt32(MetaField field, uint[] value)
            {
                Add(field, value);
            }
            public void EnterArrayInt64(MetaField field, long[] value)
            {
                Add(field, value);
            }
            public void EnterArrayUInt64(MetaField field, ulong[] value)
            {
                Add(field, value);
            }
            public void EnterArrayFloat(MetaField field, float[] value)
            {
                Add(field, value);
            }
            public void EnterArrayDouble(MetaField field, double[] value)
            {
                Add(field, value);
            }
            public void EnterArrayString(MetaField field, IList<string> value)
            {
                Add(field, value);
            }
            public void EnterArrayBytes(MetaField field, IList<byte[]> value)
            {
                Add(field, value);
            }
            public void EnterArrayEnum(MetaField field, int[] value)
            {
                Debug.Assert(field.TypeId == MetaTypeId.TYPE_ARRAY_ENUM);

                if (m_enumAsString)
                {
                    string[] enums = new string[value.Length];
                    for (int i = 0; i < enums.Length; ++i)
                    {
                        enums[i] = MetaDataGlobal.Instance.GetEnumAliasByValue(field, value[i]);
                    }
                    Add(field, enums);
                }
                else
                {
                    Add(field, value);
                }
            }
            public void EnterArrayEnum(MetaField field, IList<string> value)
            {
                if (m_enumAsString)
                {
                    Add(field, value);
                }
                else
                {
                    int[] enums = new int[value.Count];
                    for (int i = 0; i < enums.Length; ++i)
                    {
                        enums[i] = MetaDataGlobal.Instance.GetEnumValueByName(field, value[i]);
                    }
                    Add(field, enums);
                }
            }


            readonly Variant m_root;
            Variant? m_current = null;
            readonly IList<Variant> m_stack = new List<Variant>();
            readonly bool m_enumAsString = true;
//todo            VarValueToVariant m_varValueToVariant;
            readonly SerializerVariant m_outer;
        }

        private readonly Internal m_internal;
        private readonly ParserProcessDefaultValues m_parserProcessDefaultValues;

    }
}