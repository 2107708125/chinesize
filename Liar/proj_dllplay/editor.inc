      include \masm32\include\windows.inc
      include \masm32\include\masm32.inc      
      include \masm32\include\gdi32.inc
      include \masm32\include\user32.inc
      include \masm32\include\kernel32.inc
      include \masm32\include\Comctl32.inc
      include \masm32\include\comdlg32.inc
      include \masm32\include\shell32.inc
      include \masm32\include\oleaut32.inc
      include \masm32\include\msvcrt.inc
      include \masm32\include\shlwapi.inc
      include \masm32\macros\macros.asm

      includelib \masm32\lib\masm32.lib
      includelib \masm32\lib\gdi32.lib
      includelib \masm32\lib\user32.lib
      includelib \masm32\lib\kernel32.lib
      includelib \masm32\lib\Comctl32.lib
      includelib \masm32\lib\comdlg32.lib
      includelib \masm32\lib\shell32.lib
      includelib \masm32\lib\oleaut32.lib
      includelib \masm32\lib\shlwapi.lib
      includelib \masm32\lib\msvcrt.lib
      
      include rsrc.inc

_GscHeader struct
	nFileSize dd ?
	nHeaderSize dd ?
	nControlStreamSize dd ?
	nIndexSize dd ?
	nTextSize dd ?
	nExtraSize1 dd ?
	nExtraSize2 dd ?
_GscHeader ends

_GscInfo struct
	sHeader _GscHeader <?> 
	lpControlStream dd ?
	lpIndex dd ?;��Ե�ַ
	lpText dd ?
	lpExtraData dd ?
	lpRelocTable dd ?;��������Ե�ַ��ָ����Ե�ַ
	lpIndexCS dd ?;������Ե�ַ
_GscInfo ends

_GscRecord struct
	szName db 16 dup (?)
	nOffset dd ?
_GscRecord ends

WM_PREGSCERR equ WM_USER+1
WM_SAVEERR equ WM_USER+2
WM_NOTAVBGSC equ WM_USER+3
WM_STREAMERR equ WM_USER+4
WM_STREAMBYTE EQU WM_USER+5
WM_REFRESHLIST EQU WM_USER+6

CT_ADD EQU 0
CT_DEL EQU 1
MAX_LEN equ 512
NOT_INSERT equ 1
TO_INSERT EQU 0

;editor.asm
_FreeGscInfo proto
_PreReadGsc proto :dword
_ProcControlStream proto
_SaveGsc proto
_CorrectSize proto :dword
_TranslateCB proto :dword,:dword
_RefreshList proto :dword


;misc.asm
_GetFileNameInPathA proto :dword
_ConnectGscPathA proto :dword,:dword
_memcpy proto
_Bin2Scr proto :dword , :dword
_Scr2Bin proto :dword , :dword,:dword
_GetInstLen proto :dword

;lineopr.asm
_AddLine proto :dword,:dword,:dword
_DelLine proto :dword
_CorrectRTCS proto :dword,:dword,:dword
_Relocate proto :dword,:dword,:dword
_InsertString proto :dword
_GetLineFromOffset proto :dword
_GetPointerFromLine proto :dword
_ModifyText proto :dword,:dword
_GetTextByIdx proto :dword

;mainwnd.asm
_EnableMainAll proto
_DisableMainAll proto
_ListDlgProc proto :dword,:dword,:dword,:dword
_MainWndProc proto :dword,:dword,:dword,:dword
_FollowDlgProc proto :dword,:dword,:dword,:dword
_CreateMainWindow proto :dword
_CreateListWindow proto :dword


.data?
	hInstance dd ?
	hWinMain dd ?
	hWinList dd ?
	
	lpCreateFileA dd ?
	
	hGsc dd ?
	nCurOffset dd ?
	nSelOffset dd 5 dup (?)
	
	bIsGscSkipped dd ?
	bFatalErr dd ?
	bListChanged dd ?

	sGscInfo _GscInfo <<?>,?,?,?,?>
	lpGscQueryTable dd ?
	
	szOriGscName db 64 dup (?)
	szNewGscName db 64 dup (?)
;	szIdxName db 64 dup (?)
	
	dwTemp dd ?
	
.data?
;config
	szNewGscFolder db 128 dup (?)
	
.data?
;�Ӵ��ھ��
	hStatusMain dd ?
	hEditMain dd ?
	hInscurMain dd ?
	hInsprevMain dd ?
	hMoreMain dd ?
	hModifyMain dd ?
	hSaveMain dd ?
	
	hListList dd ?
	hEditList dd ?
	hStatusList dd ?
	hInsList dd ?
	hDelList dd ?
	hModList dd ?
	hSetcurList dd ?
	hGotocurList dd ?
	hPlayList dd ?
	hRetList dd ?
	hFollowList dd ?
	
.data
	szKernel32 db 'kernel32.dll',0
	szCreateFileA db 'CreateFileA',0

.data
align 4
;					0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	A,	B,	C,	D,	E,	F
	dtParamSize1 db	-1,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	4
	
;					0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	A,	B,	C,	D,	E,	F
	dtParamSize2 db 	-1,	-1,	-1,	4,	4,	4,	-1,	-1,	0,	4,	0,	0,	4,	4,	58,	4
				db	4,	0,	8,	4,	8,	0,	16,	8,	8,	4,	0,	0,	12,	8,	24,	-1
				db	24,	20,	20,	8,	8,	8,	12,	12,	8,	8,	8,	8,	4,	8,	4,	8
				db	8,	8,	4,	0,	0,	4,	-1,	0,	20,	0,	0,	16,	8,	4,	4,	12
				db	4,	4,	16,	4,	0,	0,	16,	16,	4,	8,	4,	16,	-1,	16,	-1,	-1
				db	4,	28,	24,	4,	-1,	-1,	-1,	-1,	-1,	-1,	12,	20,	8,	8,	4,	8
				db	8,	8,	8,	12,	12,	8,	4,	8,	16,	-1,	-1,	-1,	-1,	-1,	12,	12
				db	4,	8,	8,	-1,	-1,	-1,	-1,	-1,	-1,	8,	-1,	-1,	-1,	-1,	-1,	-1
				db	-1,	-1,	16,	20,	8,	-1,	12,	20,	12,	-1,	-1,	-1,	-1,	-1,	-1,	-1
				db	-1,	-1,	-1,	-1,	-1,	-1,	8,	8,	8,	8,	8,	8,	-1,	-1,	-1,	-1
				
	szDisplayName db 'Forest���ٽű������� v1.0',0
	szGang db ' - ',0
	szEnter db 0dh,0ah,0
	
	szConfigFile db '\editor.ini',0
	szSecName db 'General',0
	szKeyName db 'SaveFolder',0
	szSelectNewPos db '��ѡ���½ű��ļ�����λ��',0

	szAddFailed db '����ʧ�ܣ��볢��ͨ������ϸ���������ӣ�',0
	szAddSuccess db '���ӳɹ���',0
	szCantGetText db '�ı�����Ϊ���ҳ��Ȳ�Ҫ����512�ֽڣ�',0 
	szCantAddSel db 'ѡ��ֻ֧�ܱ��޸Ļ���ͨ��16����ָ�����ӣ�',0
	szFileOprFail db '�ļ�����ʧ�ܣ��볢���ù���ԱȨ��������Ϸ��',0
	szInsertErr db '�����ַ���ʱ��������',0	
	szNoMemory db 'ϵͳ�ڴ治�㣡�������ڴ�����´򿪽ű���',0
	szNoMemory2 db 'ϵͳ�ڴ治�㣡',0
	szNotAvbGsc db '��ǰ�ű����ǿɱ༭�ű�',0
	szNotMatch db '�ű���Ŀ��ƥ�䣡��������ϸ�����ж�λ��ǰ��Ŀ',0
	szNotMatch2 db '�����ַ������볢�Ե������ϸ�����ж�λ��Ŀ',0
	szNoVoice db 'ָ����Ŀ���������������ȷʵ������������������ϸ�������޸�',0
	szPreReadErr db 'Ԥ���ű�ʱ������������ϵ����������',0
	szSaveFailed db '�����ļ�ʧ�ܣ�',0
	szSaveSuccess db '�����ļ��ɹ���',0
	szSelCntErr db 'ѡ����Ŀ����',0
	szWrongScr db 'ָ���ʽ����',0
	szZErr db '������������ϵ����',0
	
	sz03s db '��ת��%d��',0
	sz08s db '�ӵ�ǰ�ű�����',0
	sz0e db 'ѡ��֧:%s',0
	sz0f db '����%04d.gsc�ű�',0
	sz3c db '����BGM #%d',0
	sz3e db '������Ч #%d',0
	sz42 db '��ʼ�������� #%d���ظ�%d��',0
	sz43 db 'ֹͣ��������',0
	sz45 db '�ȴ������������',0
	sz51 db '����#%d���ı�:%s',0
	
	szFF db '%04X',0
	szBeginCode db '@0x%04X',0
	szCtnCode db ' 0x%X',0
	szVoiceIn51 db '#%d ',0
	
	ANull db 0
	
	
	
	
	
		