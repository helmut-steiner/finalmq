﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

namespace finalmq
{

    public class ParserConverter : IParserVisitor
    {
        public ParserConverter(IParserVisitor? visitor = null)
        {
            m_visitor = visitor;
        }
        public void SetVisitor(IParserVisitor visitor)
        {
            m_visitor = visitor;
        }

        // IParserVisitor
        public void NotifyError(string str, string message)
        {
            Debug.Assert(m_visitor != null);
            m_visitor.NotifyError(str, message);
        }
        public void StartStruct(MetaStruct stru)
        {
            Debug.Assert(m_visitor != null);
            m_visitor.StartStruct(stru);
        }
        public void Finished()
        {
            Debug.Assert(m_visitor != null);
            m_visitor.Finished();
        }

        public void EnterStruct(MetaField field)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_STRUCT)
            {
                m_visitor.EnterStruct(field);
            }
        }
        public void ExitStruct(MetaField field)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_STRUCT)
            {
                m_visitor.ExitStruct(field);
            }
        }

        public void EnterArrayStruct(MetaField field)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_ARRAY_STRUCT)
            {
                m_visitor.EnterArrayStruct(field);
            }
        }
        public void ExitArrayStruct(MetaField field)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_ARRAY_STRUCT)
            {
                m_visitor.ExitArrayStruct(field);
            }
        }

        public void EnterBool(MetaField field, bool value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_BOOL)
            {
                m_visitor.EnterBool(field, value);
            }
            else
            {
                ConvertNumber(field, value);
            }
        }
        public void EnterInt32(MetaField field, int value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_INT32)
            {
                m_visitor.EnterInt32(field, value);
            }
            else
            {
                ConvertNumber(field, value);
            }
        }
        public void EnterUInt32(MetaField field, uint value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_UINT32)
            {
                m_visitor.EnterUInt32(field, value);
            }
            else
            {
                ConvertNumber(field, value);
            }
        }
        public void EnterInt64(MetaField field, long value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_INT64)
            {
                m_visitor.EnterInt64(field, value);
            }
            else
            {
                ConvertNumber(field, value);
            }
        }
        public void EnterUInt64(MetaField field, ulong value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_UINT64)
            {
                m_visitor.EnterUInt64(field, value);
            }
            else
            {
                ConvertNumber(field, value);
            }
        }
        public void EnterFloat(MetaField field, float value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_FLOAT)
            {
                m_visitor.EnterFloat(field, value);
            }
            else
            {
                ConvertNumber(field, value);
            }
        }
        public void EnterDouble(MetaField field, double value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_DOUBLE)
            {
                m_visitor.EnterDouble(field, value);
            }
            else
            {
                ConvertNumber(field, value);
            }
        }
        public void EnterString(MetaField field, string value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_STRING)
            {
                m_visitor.EnterString(field, value);
            }
            else
            {
                ConvertString(field, value);
            }
        }
        public void EnterString(MetaField field, byte[] buffer, int offset, int size)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_STRING)
            {
                m_visitor.EnterString(field, buffer, offset, size);
            }
            else
            {
                ConvertString(field, Encoding.UTF8.GetString(buffer, offset, size));
            }
        }
        public void EnterBytes(MetaField field, byte[] value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_BYTES)
            {
                m_visitor.EnterBytes(field, value);
            }
            else
            {
//todo                streamError << "bytes not expected";
            }
        }
        public void EnterEnum(MetaField field, int value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_ENUM)
            {
                m_visitor.EnterEnum(field, value);
            }
            else
            {
                ConvertNumber(field, value);
            }
        }
        public void EnterEnum(MetaField field, string value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_ENUM)
            {
                m_visitor.EnterEnum(field, value);
            }
            else
            {
                ConvertString(field, value);
            }
        }

        public void EnterArrayBool(MetaField field, bool[] value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_ARRAY_BOOL)
            {
                m_visitor.EnterArrayBool(field, value);
            }
            else
            {
                ConvertArraytNumber(field, value);
            }
        }
        public void EnterArrayInt32(MetaField field, int[] value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_ARRAY_INT32)
            {
                m_visitor.EnterArrayInt32(field, value);
            }
            else
            {
                ConvertArraytNumber(field, value);
            }
        }
        public void EnterArrayUInt32(MetaField field, uint[] value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_ARRAY_UINT32)
            {
                m_visitor.EnterArrayUInt32(field, value);
            }
            else
            {
                ConvertArraytNumber(field, value);
            }
        }
        public void EnterArrayInt64(MetaField field, long[] value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_ARRAY_INT64)
            {
                m_visitor.EnterArrayInt64(field, value);
            }
            else
            {
                ConvertArraytNumber(field, value);
            }
        }
        public void EnterArrayUInt64(MetaField field, ulong[] value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_ARRAY_UINT64)
            {
                m_visitor.EnterArrayUInt64(field, value);
            }
            else
            {
                ConvertArraytNumber(field, value);
            }
        }
        public void EnterArrayFloat(MetaField field, float[] value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_ARRAY_FLOAT)
            {
                m_visitor.EnterArrayFloat(field, value);
            }
            else
            {
                ConvertArraytNumber(field, value);
            }
        }
        public void EnterArrayDouble(MetaField field, double[] value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_ARRAY_DOUBLE)
            {
                m_visitor.EnterArrayDouble(field, value);
            }
            else
            {
                ConvertArraytNumber(field, value);
            }
        }
        public void EnterArrayString(MetaField field, IList<string> value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_ARRAY_STRING)
            {
                m_visitor.EnterArrayString(field, value);
            }
            else
            {
                ConvertArraytString(field, value);
            }
        }
        public void EnterArrayBytes(MetaField field, IList<byte[]> value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_ARRAY_BYTES)
            {
                m_visitor.EnterArrayBytes(field, value);
            }
            else
            {
//                streamError << "bytes array not expected";
            }
        }
        public void EnterArrayEnum(MetaField field, int[] value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_ARRAY_ENUM)
            {
                m_visitor.EnterArrayEnum(field, value);
            }
            else
            {
                ConvertArraytNumber(field, value);
            }
        }
        public void EnterArrayEnum(MetaField field, IList<string> value)
        {
            Debug.Assert(m_visitor != null);
            if (field.TypeId == MetaTypeId.TYPE_ARRAY_ENUM)
            {
                m_visitor.EnterArrayEnum(field, value);
            }
            else
            {
                ConvertArraytString(field, value);
            }
        }


        /// //////////////////////////////////////////////////////

        public void ConvertNumber(MetaField field, dynamic value)
        {
            Debug.Assert(m_visitor != null);

            switch (field.TypeId)
            {
            case MetaTypeId.TYPE_BOOL:
                m_visitor.EnterBool(field, (bool)value);
                break;
            case MetaTypeId.TYPE_INT32:
                m_visitor.EnterInt32(field, (int)value);
                break;
            case MetaTypeId.TYPE_UINT32:
                m_visitor.EnterUInt32(field, (uint)value);
                break;
            case MetaTypeId.TYPE_INT64:
                m_visitor.EnterInt64(field, (long)value);
                break;
            case MetaTypeId.TYPE_UINT64:
                m_visitor.EnterUInt64(field, (ulong)value);
                break;
            case MetaTypeId.TYPE_FLOAT:
                m_visitor.EnterFloat(field, (float)value);
                break;
            case MetaTypeId.TYPE_DOUBLE:
                m_visitor.EnterDouble(field, (double)value);
                break;
            case MetaTypeId.TYPE_STRING:
                m_visitor.EnterString(field, System.Convert.ToString(value, System.Globalization.CultureInfo.InvariantCulture));
                break;
            case MetaTypeId.TYPE_ENUM:
                m_visitor.EnterEnum(field, (int)value);
                break;
            case MetaTypeId.TYPE_ARRAY_BOOL:
                m_visitor.EnterArrayBool(field, new bool[] { (bool)value });
                break;
            case MetaTypeId.TYPE_ARRAY_INT32:
                m_visitor.EnterArrayInt32(field, new int[] { (int)value });
                break;
            case MetaTypeId.TYPE_ARRAY_UINT32:
                m_visitor.EnterArrayUInt32(field, new uint[] { (uint)value });
                break;
            case MetaTypeId.TYPE_ARRAY_INT64:
                m_visitor.EnterArrayInt64(field, new long[] { (long)value });
                break;
            case MetaTypeId.TYPE_ARRAY_UINT64:
                m_visitor.EnterArrayUInt64(field, new ulong[] { (ulong)value });
                break;
            case MetaTypeId.TYPE_ARRAY_FLOAT:
                m_visitor.EnterArrayFloat(field, new float[] { (float)value });
                break;
            case MetaTypeId.TYPE_ARRAY_DOUBLE:
                m_visitor.EnterArrayDouble(field, new double[] { (double)value });
                break;
            case MetaTypeId.TYPE_ARRAY_STRING:
                m_visitor.EnterArrayString(field, new string[] { System.Convert.ToString(value, System.Globalization.CultureInfo.InvariantCulture) });
                break;
            case MetaTypeId.TYPE_ARRAY_BYTES:
                m_visitor.EnterArrayBytes(field, new byte[][] { Encoding.UTF8.GetBytes(System.Convert.ToString(value, System.Globalization.CultureInfo.InvariantCulture)) });
                break;
            case MetaTypeId.TYPE_ARRAY_ENUM:
                m_visitor.EnterArrayEnum(field, new int[] { (int)value });
                break;
            default:
//todo                streamError << "number not expected";
                break;
            }
        }

        void ConvertString(MetaField field, string value)
        {
            Debug.Assert(m_visitor != null);

            switch (field.TypeId)
            {
            case MetaTypeId.TYPE_BOOL:
                m_visitor.EnterBool(field, Convertion.Convert<bool>(value));
                break;
            case MetaTypeId.TYPE_INT32:
                m_visitor.EnterInt32(field, Convertion.Convert<int>(value));
                break;
            case MetaTypeId.TYPE_UINT32:
                m_visitor.EnterUInt32(field, Convertion.Convert<uint>(value));
                break;
            case MetaTypeId.TYPE_INT64:
                m_visitor.EnterInt64(field, Convertion.Convert<long>(value));
                break;
            case MetaTypeId.TYPE_UINT64:
                m_visitor.EnterUInt64(field, Convertion.Convert<ulong>(value));
                break;
            case MetaTypeId.TYPE_FLOAT:
                m_visitor.EnterFloat(field, Convertion.Convert<float>(value));
                break;
            case MetaTypeId.TYPE_DOUBLE:
                m_visitor.EnterDouble(field, Convertion.Convert<double>(value));
                break;
            case MetaTypeId.TYPE_STRING:
                m_visitor.EnterString(field, value);
                break;
            case MetaTypeId.TYPE_ENUM:
                m_visitor.EnterEnum(field, value);
                break;
            case MetaTypeId.TYPE_ARRAY_BOOL:
                m_visitor.EnterArrayBool(field, new bool[] { Convertion.Convert<bool>(value) });
                break;
            case MetaTypeId.TYPE_ARRAY_INT32:
                m_visitor.EnterArrayInt32(field, new int[] { Convertion.Convert<int>(value) });
                break;
            case MetaTypeId.TYPE_ARRAY_UINT32:
                m_visitor.EnterArrayUInt32(field, new uint[] { Convertion.Convert<uint>(value) });
                break;
            case MetaTypeId.TYPE_ARRAY_INT64:
                m_visitor.EnterArrayInt64(field, new long[] { Convertion.Convert<long>(value) });
                break;
            case MetaTypeId.TYPE_ARRAY_UINT64:
                m_visitor.EnterArrayUInt64(field, new ulong[] { Convertion.Convert<ulong>(value) });
                break;
            case MetaTypeId.TYPE_ARRAY_FLOAT:
                m_visitor.EnterArrayFloat(field, new float[] { Convertion.Convert<float>(value) });
                break;
            case MetaTypeId.TYPE_ARRAY_DOUBLE:
                m_visitor.EnterArrayDouble(field, new double[] { Convertion.Convert<double>(value) });
                break;
            case MetaTypeId.TYPE_ARRAY_STRING:
                m_visitor.EnterArrayString(field, new string[] { value });
                break;
            case MetaTypeId.TYPE_ARRAY_ENUM:
                m_visitor.EnterArrayString(field, new string[] { value });
                break;
            default:
//todo                streamError << "number not expected";
                break;
            }
        }

        void ConvertArraytNumber<T>(MetaField field, T[] value)
        {
            Debug.Assert(m_visitor != null);
            Debug.Assert(value != null);
            switch (field.TypeId)
            {
            case MetaTypeId.TYPE_ARRAY_BOOL:
                {
                    bool[] v = new bool[value.Length];
                    for (int i = 0; i < value.Length; ++i) 
                    {
#pragma warning disable CS8600 // Das NULL-Literal oder ein möglicher NULL-Wert wird in einen Non-Nullable-Typ konvertiert.
                        v[i] = (bool)(dynamic)value[i];
#pragma warning restore CS8600 // Das NULL-Literal oder ein möglicher NULL-Wert wird in einen Non-Nullable-Typ konvertiert.
                    }
                    m_visitor.EnterArrayBool(field, v);
                }
                break;
            case MetaTypeId.TYPE_ARRAY_INT32:
                {
                    int[] v = new int[value.Length];
                    for (int i = 0; i < value.Length; ++i)
                    {
#pragma warning disable CS8600 // Das NULL-Literal oder ein möglicher NULL-Wert wird in einen Non-Nullable-Typ konvertiert.
                            v[i] = (int)(dynamic)value[i];
#pragma warning restore CS8600 // Das NULL-Literal oder ein möglicher NULL-Wert wird in einen Non-Nullable-Typ konvertiert.
                        }
                        m_visitor.EnterArrayInt32(field, v);
                }
                break;
            case MetaTypeId.TYPE_ARRAY_UINT32:
                {
                    uint[] v = new uint[value.Length];
                    for (int i = 0; i < value.Length; ++i)
                    {
#pragma warning disable CS8600 // Das NULL-Literal oder ein möglicher NULL-Wert wird in einen Non-Nullable-Typ konvertiert.
                            v[i] = (uint)(dynamic)value[i];
#pragma warning restore CS8600 // Das NULL-Literal oder ein möglicher NULL-Wert wird in einen Non-Nullable-Typ konvertiert.
                        }
                        m_visitor.EnterArrayUInt32(field, v);
                }
                break;
            case MetaTypeId.TYPE_ARRAY_INT64:
                {
                    long[] v = new long[value.Length];
                    for (int i = 0; i < value.Length; ++i)
                    {
#pragma warning disable CS8600 // Das NULL-Literal oder ein möglicher NULL-Wert wird in einen Non-Nullable-Typ konvertiert.
                            v[i] = (long)(dynamic)value[i];
#pragma warning restore CS8600 // Das NULL-Literal oder ein möglicher NULL-Wert wird in einen Non-Nullable-Typ konvertiert.
                        }
                        m_visitor.EnterArrayInt64(field, v);
                }
                break;
            case MetaTypeId.TYPE_ARRAY_UINT64:
                {
                    ulong[] v = new ulong[value.Length];
                    for (int i = 0; i < value.Length; ++i)
                    {
#pragma warning disable CS8600 // Das NULL-Literal oder ein möglicher NULL-Wert wird in einen Non-Nullable-Typ konvertiert.
                            v[i] = (ulong)(dynamic)value[i];
#pragma warning restore CS8600 // Das NULL-Literal oder ein möglicher NULL-Wert wird in einen Non-Nullable-Typ konvertiert.
                        }
                        m_visitor.EnterArrayUInt64(field, v);
               }
                break;
            case MetaTypeId.TYPE_ARRAY_FLOAT:
                {
                    float[] v = new float[value.Length];
                    for (int i = 0; i < value.Length; ++i)
                    {
#pragma warning disable CS8600 // Das NULL-Literal oder ein möglicher NULL-Wert wird in einen Non-Nullable-Typ konvertiert.
                            v[i] = (float)(dynamic)value[i];
#pragma warning restore CS8600 // Das NULL-Literal oder ein möglicher NULL-Wert wird in einen Non-Nullable-Typ konvertiert.
                        }
                        m_visitor.EnterArrayFloat(field, v);
                }
                break;
            case MetaTypeId.TYPE_ARRAY_DOUBLE:
                {
                    double[] v = new double[value.Length];
                    for (int i = 0; i < value.Length; ++i)
                    {
#pragma warning disable CS8600 // Das NULL-Literal oder ein möglicher NULL-Wert wird in einen Non-Nullable-Typ konvertiert.
                            v[i] = (double)(dynamic)value[i];
#pragma warning restore CS8600 // Das NULL-Literal oder ein möglicher NULL-Wert wird in einen Non-Nullable-Typ konvertiert.
                        }
                        m_visitor.EnterArrayDouble(field, v);
                }
                break;
            case MetaTypeId.TYPE_ARRAY_STRING:
                {
                    string[] v = new string[value.Length];
                    for (int i = 0; i < value.Length; ++i)
                    {
                        string? s = System.Convert.ToString(value[i], System.Globalization.CultureInfo.InvariantCulture);
                        if (s != null)
                        {
                            v[i] = s;
                        }
                        else 
                        {
                            v[i] = "";
                        }
                    }
                    m_visitor.EnterArrayString(field, v);
                }
                break;
            case MetaTypeId.TYPE_ARRAY_ENUM:
                {
                    int[] v = new int[value.Length];
                    for (int i = 0; i < value.Length; ++i)
                    {
#pragma warning disable CS8600 // Das NULL-Literal oder ein möglicher NULL-Wert wird in einen Non-Nullable-Typ konvertiert.
                            v[i] = (int)(dynamic)value[i];
#pragma warning restore CS8600 // Das NULL-Literal oder ein möglicher NULL-Wert wird in einen Non-Nullable-Typ konvertiert.
                        }
                        m_visitor.EnterArrayInt32(field, v);
                }
                break;
            default:
//todo                streamError << "array not expected";
                break;
            }
        }

        void ConvertArraytString(MetaField field, IList<string> value)
        {
            Debug.Assert(m_visitor != null);

            switch (field.TypeId)
            {
            case MetaTypeId.TYPE_ARRAY_BOOL:
                {
                    bool[] v = new bool[value.Count];
                    for (int i = 0; i < value.Count; ++i)
                    {
                        v[i] = Convertion.Convert((dynamic)value[i]);
                    }
                    m_visitor.EnterArrayBool(field, v);
                }
                break;
            case MetaTypeId.TYPE_ARRAY_INT32:
                {
                    int[] v = new int[value.Count];
                    for (int i = 0; i < value.Count; ++i)
                    {
                        v[i] = Convertion.Convert((dynamic)value[i]);
                    }
                    m_visitor.EnterArrayInt32(field, v);
                }
                break;
            case MetaTypeId.TYPE_ARRAY_UINT32:
                {
                    uint[] v = new uint[value.Count];
                    for (int i = 0; i < value.Count; ++i)
                    {
                        v[i] = Convertion.Convert((dynamic)value[i]);
                    }
                    m_visitor.EnterArrayUInt32(field, v);
                }
                break;
            case MetaTypeId.TYPE_ARRAY_INT64:
                {
                    long[] v = new long[value.Count];
                    for (int i = 0; i < value.Count; ++i)
                    {
                        v[i] = Convertion.Convert((dynamic)value[i]);
                    }
                    m_visitor.EnterArrayInt64(field, v);
                }
                break;
            case MetaTypeId.TYPE_ARRAY_UINT64:
                {
                    ulong[] v = new ulong[value.Count];
                    for (int i = 0; i < value.Count; ++i)
                    {
                        v[i] = Convertion.Convert((dynamic)value[i]);
                    }
                    m_visitor.EnterArrayUInt64(field, v);
                }
                break;
            case MetaTypeId.TYPE_ARRAY_FLOAT:
                {
                    float[] v = new float[value.Count];
                    for (int i = 0; i < value.Count; ++i)
                    {
                        v[i] = Convertion.Convert((dynamic)value[i]);
                    }
                    m_visitor.EnterArrayFloat(field, v);
                }
                break;
            case MetaTypeId.TYPE_ARRAY_DOUBLE:
                {
                    double[] v = new double[value.Count];
                    for (int i = 0; i < value.Count; ++i)
                    {
                        v[i] = Convertion.Convert((dynamic)value[i]);
                    }
                    m_visitor.EnterArrayDouble(field, v);
                }
                break;
            case MetaTypeId.TYPE_ARRAY_STRING:
                m_visitor.EnterArrayString(field, value);
                break;
            case MetaTypeId.TYPE_ARRAY_ENUM:
                m_visitor.EnterArrayEnum(field, value);
                break;
            default:
//todo                streamError << "array not expected";
                break;
            }
        }

        IParserVisitor? m_visitor = null;
    }
}



