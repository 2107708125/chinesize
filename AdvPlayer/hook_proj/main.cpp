#include <windows.h>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <D3D9.h>

#include "ilhook.h"
#include "FileReader.h"
#include "FuncHelper.h"
#include "PicSupport.h"
#include "TxtSupport.h"

#include "ws2.h"

using namespace std;

#define DP(name,addr,pat,hex) {name,addr,pat,hex,sizeof(hex)-1},
PatchStruct g_Patches[] = {
    DP(nullptr, 0x44ddd,"\x75\x81","\xb8\xa1") //quote
};
#undef DP

__declspec(naked) void D3dCreate9()
{
    __asm jmp Direct3DCreate9
}

bool IsHalf(const string& s)
{
    for (auto c : s)
    {
        if ((unsigned char)c >= 0x80)
        {
            return false;
        }
    }
    return true;
}

void HOOKFUNC MyCFI(LPLOGFONTW lfi)
{
    //"�ͣ� �����å�"
    if (wcscmp(lfi->lfFaceName, L"MS UI Gothic") == 0 ||
        wcscmp(lfi->lfFaceName, L"�ͣ� �����å�") == 0)
    {
        wcscpy_s(lfi->lfFaceName, L"SimHei");
    }
}

void HOOKFUNC MyCW(wchar_t** strp)
{
    if (*strp && wcscmp(*strp, L"������ݳ֤��Τ����ͤ����������ͤ��뤤�򤺤äȡ�") == 0)
    {
        *strp = L"����Ľ֮�ĵĽ�֯��ʽ ��Ը��֯��˼�����㡫�����İ� | �aʾ��Ϸ���Ļ���ȤС�� ���� | ����Ⱥ�ţ�495988182";
    }
}

BOOL WINAPI DllMain(_In_ void* _DllHandle, _In_ unsigned long _Reason, _In_opt_ void* _Reserved)
{
    if (_Reason == DLL_PROCESS_ATTACH)
    {
        InitWs2();

        PatchMemory(g_Patches, ARRAYSIZE(g_Patches));

        static const HookPointStruct points[] = {
            //_wfopen_s�����ں�����һ���Ǻ���ͷ��һ�������м�call _wfopen_s������ȡarc��λ��
            { nullptr, 0xe9cc0, MyOpenFile1, "23", false, 0 }, //����λ��Ҳ��Ҫ����
            { nullptr, 0xea1a8, MyOpenFile, "r", false, 0 },

            //0F B6 ?? ?? ?? 89 5C 24 ?? 8B 2C 85 ?? ?? ?? 00 85 ED 0F 84
            //�����������ҵ�һ�����������������ζ�ReadInst������call��hook ReadInst�ĺ���ͷ
            { nullptr, 0xE0CE0, MyReadInst, "rf", true, 0x10 },
            
            //8B 45 ?? 2B 45 ?? 8B 33 C7 44 24 ?? 01 00 00 00 3B F0 72 05 E8
            //��������ͷ����Ϊhook��ַ�����¼�ʮ��֮���и�������lpMultiByteStr��call
            { nullptr, 0xE09B0, MySelString, "r", false, 0 },
            //��λ���������Ǹ�call֮�ڣ�call MultiByteToWideChar��λ��
            { nullptr, 0xC7CE8, MyMbtowc, "r", false, 0 },
            //wcsncmp�ĵڶ����������ڵĺ���
            { nullptr, 0x3CC00, MyChangeFont, "r", false, 0 },
        };

        if (!HookFunctions(points))
        {
            MessageBox(0, L"Hook ʧ�ܣ�", 0, 0);
            return TRUE;
        }

        static const HookPointStructWithName points2[] = {
            { "gdi32.dll", "CreateFontIndirectW", MyCFI, "1", false, 0 },
            { "user32.dll", "CreateWindowExW", MyCW, "\x03", false, 0 },
        };
        if (!HookFunctions(points2))
        {
            MessageBox(0, L"Hook2 ʧ�ܣ�", 0, 0);
        }
    }
    return TRUE;
}