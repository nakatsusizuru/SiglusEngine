#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <windows.h>
#include <iostream>
#include <string>
using namespace std;



typedef unsigned char byte;
typedef unsigned short word;
unsigned char* buf;
typedef unsigned int ulong;

//0x4C //头大小,下面是数据..
//0x50 文件数量
//0x44 file_offset_info_s

typedef struct FILE_INFO{
	DWORD offset;
	DWORD length;
} FILE_INFO,*PFILE_INFO;

typedef struct HEADERPAIR{
	int offset;
	int count;
}HEADERPAIR;

typedef struct ss_header{
	DWORD szZiped;
	DWORD szOrigianl;
}ss_header;

typedef struct SCENEHEADER{
	int headerLength;
	HEADERPAIR varInfo;
	HEADERPAIR varNameIndex;
	HEADERPAIR varName;
	HEADERPAIR cmdInfo;
	HEADERPAIR cmdNameIndex;
	HEADERPAIR cmdName;
	HEADERPAIR SceneNameIndex;
	HEADERPAIR SceneName;
	HEADERPAIR SceneInfo;
	HEADERPAIR SceneData;
	int ExtraKeyUse;
	int SourceHeaderLength;
}SCENEHEADER;
typedef struct file_offset_info_s
{
	ulong offsets;
	ulong sizes;
}file_offset_info_t;


typedef struct compress_file_header_s
{
	ulong comp_size;
	ulong decomp_size;
}compress_file_header_t;



void decrypt_1(byte* debuf,size_t desize)
{
	size_t key_idx = 0;
	size_t xor_idx = 0;
	static byte key[] = 
	{
		0x62,0x18,0x8B,0x36,0x7C,0xE0,0x47,0xFB,0xB0,0x0D,0xB0,0x2F,0xF0,0x61,0xA2,0x3D,0xBD,0xC9,0xC6,0x23,0xD9,0xA1,0xC1,0x00,0x24,0xF2,0x18,0x00,0x2C,0xE4,0x08,0x01,0x07,0x00,0x00,0x00
	};

	for(xor_idx = 0;xor_idx<desize;xor_idx++,key_idx++,key_idx = key_idx & 0x8000000F)
	{
		debuf[xor_idx] ^= key[key_idx];
	}
}

void decrypt_2(byte* debuf,size_t desize)
{
	size_t key_idx = 0;
	size_t xor_idx = 0;

	static byte key[] = 
	{
		0x70,0xF8,0xA6,0xB0,0xA1,0xA5,0x28,0x4F,0xB5,0x2F,0x48,0xFA,0xE1,0xE9,0x4B,0xDE,0xB7,0x4F,0x62,0x95,0x8B,0xE0,0x03,0x80,0xE7,0xCF,0x0F,0x6B,0x92,0x01,0xEB,0xF8,0xA2,0x88,0xCE,0x63,0x04,0x38,0xD2,0x6D,0x8C,0xD2,0x88,0x76,0xA7,0x92,0x71,0x8F,0x4E,0xB6,0x8D,0x01,0x79,0x88,0x83,0x0A,0xF9,0xE9,0x2C,0xDB,0x67,0xDB,0x91,0x14,0xD5,0x9A,0x4E,0x79,0x17,0x23,0x08,0x96,0x0E,0x1D,0x15,0xF9,0xA5,0xA0,0x6F,0x58,0x17,0xC8,0xA9,0x46,0xDA,0x22,0xFF,0xFD,0x87,0x12,0x42,0xFB,0xA9,0xB8,0x67,0x6C,0x91,0x67,0x64,0xF9,0xD1,0x1E,0xE4,0x50,0x64,0x6F,0xF2,0x0B,0xDE,0x40,0xE7,0x47,0xF1,0x03,0xCC,0x2A,0xAD,0x7F,0x34,0x21,0xA0,0x64,0x26,0x98,0x6C,0xED,0x69,0xF4,0xB5,0x23,0x08,0x6E,0x7D,0x92,0xF6,0xEB,0x93,0xF0,0x7A,0x89,0x5E,0xF9,0xF8,0x7A,0xAF,0xE8,0xA9,0x48,0xC2,0xAC,0x11,0x6B,0x2B,0x33,0xA7,0x40,0x0D,0xDC,0x7D,0xA7,0x5B,0xCF,0xC8,0x31,0xD1,0x77,0x52,0x8D,0x82,0xAC,0x41,0xB8,0x73,0xA5,0x4F,0x26,0x7C,0x0F,0x39,0xDA,0x5B,0x37,0x4A,0xDE,0xA4,0x49,0x0B,0x7C,0x17,0xA3,0x43,0xAE,0x77,0x06,0x64,0x73,0xC0,0x43,0xA3,0x18,0x5A,0x0F,0x9F,0x02,0x4C,0x7E,0x8B,0x01,0x9F,0x2D,0xAE,0x72,0x54,0x13,0xFF,0x96,0xAE,0x0B,0x34,0x58,0xCF,0xE3,0x00,0x78,0xBE,0xE3,0xF5,0x61,0xE4,0x87,0x7C,0xFC,0x80,0xAF,0xC4,0x8D,0x46,0x3A,0x5D,0xD0,0x36,0xBC,0xE5,0x60,0x77,0x68,0x08,0x4F,0xBB,0xAB,0xE2,0x78,0x07,0xE8,0x73,0xBF,0xCF,0xC9,0x9E
	};
	for(xor_idx = 0;xor_idx<desize;xor_idx++,key_idx++,key_idx = key_idx & 0x800000FF)
	{
		debuf[xor_idx] ^= key[key_idx];
	}
}
void decompress_data(byte* comp_data,byte* decomp_begin,byte* decomp_end)
{
	byte* s = comp_data;
	byte* d = decomp_begin;
	byte c;
	while(true)
	{
		c = *s++;
		byte n=8;
		do
		{
			if(d == decomp_end)
			{
				return;
			}
			if(c & 0x1)
			{
				memcpy(d,s,sizeof(byte));
				s++;
				d++;
			}
			else
			{
				unsigned int o,f;

				o = (unsigned int)*(unsigned short*)s;

				s += sizeof(unsigned short);
				f = o;
				o >>= 4;
				f = (f & 0xF) + 0x2;

				memcpy(d,(void*)(d - o),f);
				d += f;
			}
			c >>= 1;
			n--;
		}while(n);
	}
}
//void decompress_data(byte* comp_data,byte* decomp_begin,byte* decomp_end)
//{
//	__asm
//	{
//		mov esi,comp_data
//		mov edi,decomp_begin
//		xor edx,edx
//		cld
//Loop1:
//		mov dl,byte ptr [esi]
//		inc esi
//		mov dh,0x8
//Loop2:
//		cmp edi,decomp_end
//		je End
//		test dl,1
//		je DecompTag
//		movsb
//		jmp DecompTag2
//DecompTag:
//		xor eax,eax
//		lods word ptr [esi]
//		mov ecx,eax
//		shr eax,4
//		and ecx,0xF
//		add ecx,0x2
//		mov ebx,esi
//		mov esi,edi
//		sub esi,eax
//		rep movsb
//		mov esi,ebx
//DecompTag2:
//		shr dl,1
//		dec dh
//		jnz Loop2
//		jmp Loop1
//
//End:
//	}
//}


int main(int argc,char** args)
{
	char* file_name;
	if(argc > 1)
	{
		file_name = args[1];
	}
	else
		return 0;
	FILE* f= fopen(file_name,"rb");
	fseek(f,0,SEEK_END);
	unsigned int size = ftell(f);
	fseek(f,0,SEEK_SET);
	buf = (unsigned char*)malloc(size);
	fread(buf,size,1,f);
	
	wchar_t usFileName[128];
	SCENEHEADER* header = (SCENEHEADER*)buf;
	HEADERPAIR* SceneNameLength = (HEADERPAIR*)&buf[header->SceneNameIndex.offset];
	wchar_t* SceneNameString = (wchar_t*)&buf[header->SceneName.offset];
	HEADERPAIR* SceneDataInfo = (HEADERPAIR*)&buf[header->SceneInfo.offset];
	byte* SceneData = (byte*)&buf[header->SceneData.offset];
	byte* disk_buffer = (byte*)malloc(0);
	unsigned int disk_offset = 0;
	for(DWORD i=0;i<header->SceneNameIndex.count;i++)
	{
		memset(&usFileName,0,sizeof(usFileName));
		wcsncpy(usFileName,&SceneNameString[SceneNameLength[i].offset],SceneNameLength[i].count);
		wchar_t ss[33];
		wsprintf(ss,L"_%d",i);
		wcscat(usFileName,ss);
		wcscat(usFileName,L".ss");


		FILE* pf = _wfopen(usFileName,L"rb");
		fseek(pf,0,SEEK_END);
		size_t length_of_ss = ftell(pf);
		fseek(pf,0,SEEK_SET);
		byte* tmpbuf = new byte[length_of_ss + sizeof(compress_file_header_t)];
		fread(&tmpbuf[sizeof(compress_file_header_t)],length_of_ss,1,pf);
		fclose(pf);

		SceneDataInfo[i].offset = disk_offset;
		SceneDataInfo[i].count = length_of_ss + sizeof(compress_file_header_t);
		compress_file_header_t compress_info;
		disk_buffer = (byte*)realloc(disk_buffer,disk_offset + length_of_ss + sizeof(compress_file_header_t));

		compress_info.comp_size = length_of_ss;
		compress_info.decomp_size = length_of_ss;

		memcpy(tmpbuf,&compress_info,sizeof(compress_file_header_t));

		decrypt_2(tmpbuf,length_of_ss + sizeof(compress_file_header_t));
		decrypt_1(tmpbuf,length_of_ss + sizeof(compress_file_header_t));

		memcpy(&disk_buffer[disk_offset],tmpbuf,length_of_ss + sizeof(compress_file_header_t));
		disk_offset += length_of_ss + sizeof(compress_file_header_t);

		delete tmpbuf;
	}

	size_t new_size = size + disk_offset;

	unsigned char* new_buf =(byte*)malloc(new_size);
	memcpy(new_buf,buf,size);
	memcpy(&new_buf[size],disk_buffer,disk_offset);

	SCENEHEADER* new_header = (SCENEHEADER*)new_buf;
	new_header->SceneData.offset = size;
	std::string new_name = file_name;
	new_name += ".new";

	FILE* newf = fopen(new_name.c_str(),"wb");
	fwrite(new_buf,new_size,1,newf);
	fclose(newf);

	return 0;
}