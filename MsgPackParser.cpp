// ConsoleApplication5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdint.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "cmp/cmp.h"

using namespace std;

void WriteObject(cmp_ctx_t& ctx, int level);

static bool frame_reader(cmp_ctx_t *ctx, void *data, size_t limit) {
    auto p = reinterpret_cast<std::ifstream*>(ctx->buf);
    if (p)
        return (bool) (p->read((char*) data, limit));
    else
        return false;
}

string StringReaderHelper(cmp_ctx_t& ctx, cmp_object_t& obj, uint32_t stringSize)
{
    if (stringSize > INT_MAX)
    {
        return "";
    }

    int size = static_cast<int>(stringSize);

    char* pBuff = new char[size + 1];

    frame_reader(&ctx, pBuff, stringSize);

    pBuff[size] = 0;
    string value(pBuff);
    delete[] pBuff;

    return value;
}

void tabs(int x)
{
    for (int i = 0; i < x; i++)
        cout << "\t";
}

void WriteExt(cmp_ctx_t& ctx, cmp_object_t& obj, int level)
{
    int8_t type;
    uint32_t size;
    cmp_object_as_ext(&obj, &type, &size);

    cout << "Extended Type: " << type << " Size: " << size << " ";
}

void WriteMap(cmp_ctx_t& ctx, cmp_object_t& obj, int level)
{
    uint32_t size;
    cmp_object_as_map(&obj, &size);

    cout << "Map Size: " << size << " { " << endl;
    for (int i = 0; i < size; i++)
    {
        tabs(level + 1);
        WriteObject(ctx, level + 1);
        cout << " : ";
        WriteObject(ctx, level + 1);
        cout << endl;
    }

    tabs(level);

    cout << "} ";
}

void WriteArray(cmp_ctx_t& ctx, cmp_object_t& obj, int level)
{
    uint32_t size;
    cmp_object_as_array(&obj, &size);

    cout << "Array Size: " << size << " [ " << endl;
    for (int i = 0; i < size; i++)
    {
        tabs(level + 1);
        WriteObject(ctx, level + 1);
        cout << endl;
    }

    tabs(level);

    cout << "] ";
}

void WriteObject(cmp_ctx_t& ctx, int level)
{
    cmp_object_t obj;

    if(cmp_read_object(&ctx, &obj))
    {
        if (cmp_object_is_ext(&obj))
        {
            WriteExt(ctx, obj, level);
        }
        else if (cmp_object_is_map(&obj))
        {
            WriteMap(ctx, obj, level);
        }
        else if (cmp_object_is_array(&obj))
        {
            WriteArray(ctx, obj, level);
        }
        else if (cmp_object_is_bin(&obj))
        {
            uint32_t size;
            vector<unsigned char> v;

            cmp_object_as_bin(&obj, &size);
            v.resize(size);

            frame_reader(&ctx, v.data(), size);

            cout << "Binary Data (size: " << size << ") ";
        }
        else if (cmp_object_is_str(&obj))
        {
            uint32_t size;

            cmp_object_as_str(&obj, &size);
            auto s = StringReaderHelper(ctx, obj, size);
            cout << s << " ";
        }
        else if (cmp_object_is_bool(&obj))
        {
            bool b;
            cmp_object_as_bool(&obj, &b);

            if (b)
                cout << "True; ";
            else
                cout << "False; ";
        }
        else if (cmp_object_is_nil(&obj))
        {
            cout << "Nil";
        }
        else if (cmp_object_is_double(&obj))
        {
            double d;
            cmp_object_as_double(&obj, &d);
            cout << d << " (double) ";
        }
        else if (cmp_object_is_float(&obj))
        {
            float d;
            cmp_object_as_float(&obj, &d);
            cout << d << " (float) ";
        }
        else if (cmp_object_is_uinteger(&obj))
        {
            uint64_t d;
            cmp_object_as_uinteger(&obj, &d);
            cout << d << " (uinteger) ";
        }
        else if (cmp_object_is_ulong(&obj))
        {
            uint64_t d;
            cmp_object_as_ulong(&obj, &d);
            cout << d << " (ulong) ";
        }
        else if (cmp_object_is_uint(&obj))
        {
            uint32_t d;
            cmp_object_as_uint(&obj, &d);
            cout << d << " (uint) ";
        }
        else if (cmp_object_is_ushort(&obj))
        {
            uint16_t d;
            cmp_object_as_ushort(&obj, &d);
            cout << d << " (ushort) ";
        }
        else if (cmp_object_is_uchar(&obj))
        {
            uint8_t d;
            cmp_object_as_uchar(&obj, &d);
            cout << d << " (uchar) ";
        }
        else if (cmp_object_is_sinteger(&obj))
        {
            int64_t d;
            cmp_object_as_sinteger(&obj, &d);
            cout << d << " (sinteger) ";
        }
        else if (cmp_object_is_long(&obj))
        {
            int64_t d;
            cmp_object_as_long(&obj, &d);
            cout << d << " (long) ";
        }
        else if (cmp_object_is_int(&obj))
        {
            int32_t d;
            cmp_object_as_int(&obj, &d);
            cout << d << " (int) ";
        }
        else if (cmp_object_is_short(&obj))
        {
            int16_t d;
            cmp_object_as_short(&obj, &d);
            cout << d << " (short) ";
        }
        else if (cmp_object_is_char(&obj))
        {
            int8_t c;
            cmp_object_as_char(&obj, &c);
            cout << c << " (char) ";
        }
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    std::ifstream f;

    f.open(argv[1], std::ios::binary);

    if (f.is_open())
    {
        cmp_ctx_t cmp;
        cmp_init(&cmp, &f, frame_reader, nullptr);

        WriteObject(cmp, 0);
    }
    else
    {
        cout << "Error reading file." << endl;
    }
	return 0;
}

