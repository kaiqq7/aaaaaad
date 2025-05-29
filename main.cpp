//D3D hook by n7

#include "main.h" //helper funcs here

#include "Hook.h"


#include "ders.h"

#include <d3d9.h>







//imgui

#include "imgui\imgui.h"
#include "imgui\imgui_impl_dx9.h"
#include "imgui\imgui_impl_win32.h"
#include "\DX9LoggerImGui-master\imgui\imgui_internal.h"

//detours
#include "detours.h"
#if defined _M_X64
#pragma comment(lib, "detours.X64/detours.lib")
#elif defined _M_IX86
#pragma comment(lib, "detours.X86/detours.lib")
#endif

void ToggleButton(const char* str_id, bool* v)
{
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float height = ImGui::GetFrameHeight();
	float width = height * 1.55f;
	float radius = height * 0.50f;

	ImGui::InvisibleButton(str_id, ImVec2(width, height));

	if (ImGui::IsItemClicked())
		*v = !*v;

	float t = *v ? 1.0f : 0.0f;

	ImGuiContext& g = *GImGui;
	float ANIM_SPEED = 0.08f;
	if (g.LastActiveId == g.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
	{
		float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
		t = *v ? (t_anim) : (1.0f - t_anim);
	}

	ImU32 col_bg;
	if (ImGui::IsItemHovered())
		col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t));
	else
		col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));

	draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
	draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}

typedef HRESULT(APIENTRY *SetStreamSource)(IDirect3DDevice9*, UINT, IDirect3DVertexBuffer9*, UINT, UINT);
SetStreamSource SetStreamSource_orig = 0;

typedef HRESULT(APIENTRY *SetVertexDeclaration)(IDirect3DDevice9*, IDirect3DVertexDeclaration9*);
SetVertexDeclaration SetVertexDeclaration_orig = 0;

typedef HRESULT(APIENTRY *SetVertexShaderConstantF)(IDirect3DDevice9*, UINT, const float*, UINT);
SetVertexShaderConstantF SetVertexShaderConstantF_orig = 0;

typedef HRESULT(APIENTRY *SetVertexShader)(IDirect3DDevice9*, IDirect3DVertexShader9*);
SetVertexShader SetVertexShader_orig = 0;

typedef HRESULT(APIENTRY *SetPixelShader)(IDirect3DDevice9*, IDirect3DPixelShader9*);;
SetPixelShader SetPixelShader_orig = 0;


typedef HRESULT(APIENTRY *DrawIndexedPrimitive)(IDirect3DDevice9*, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
DrawIndexedPrimitive DrawIndexedPrimitive_orig = 0;

typedef HRESULT(APIENTRY *DrawPrimitive)(IDirect3DDevice9*, D3DPRIMITIVETYPE, UINT, UINT);
DrawPrimitive DrawPrimitive_orig = 0;

typedef HRESULT(APIENTRY *SetTexture)(IDirect3DDevice9*, DWORD, IDirect3DBaseTexture9 *);
SetTexture SetTexture_orig = 0;

typedef HRESULT(APIENTRY* Present) (IDirect3DDevice9*, const RECT *, const RECT *, HWND, const RGNDATA *);
Present Present_orig = 0;

typedef HRESULT(APIENTRY* EndScene) (IDirect3DDevice9*);
EndScene EndScene_orig = 0;

typedef HRESULT(APIENTRY *Reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
Reset Reset_orig = 0;

//==========================================================================================================================

WNDPROC game_wndproc = NULL;

// Win32 message handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ShowMenu && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		//Log("ImGui_ImplWin32_WndProcHandler");
		return true;
	}

	return CallWindowProc(game_wndproc, hWnd, msg, wParam, lParam);
}

//==========================================================================================================================
bool  m_iChams = false;
bool m_iredchams = false;
bool wallhack = false;
bool wriframe = false;
bool m_iChamsBlue = false;


#include <iostream>

HANDLE MEMORY = OpenProcess(PROCESS_ALL_ACCESS, 0, GetCurrentProcessId());
DWORD CH;
DWORD PointBlank;
DWORD BIN;
BOOL PointModule = true;



bool m_ibluename = false;

bool m_iredname = false;

// --- ping --- \\

bool m_iRecoil = false; // sekmeme %50
bool m_iRecoilUP = false; // sekmeme %50


bool m_iRepidFire = false; // 

bool m_iWeaponFast = false;

bool m_iInfAmmo = false;

bool m_iHealthBlue = false; // onaylandý

bool m_iHealthRed = false; // onaylandý

bool m_iCharacterSize = false;

bool m_iRecoilFully = false;

bool m_iDuelRecoil = false; //

bool m_iNoCbEsp = false;

// fov



bool m_imyrealityfov = false;

int mysýfýrx = 0x0;

int myfov = 80; // default

bool m_iRecoilRight = false;

bool m_inofalldamage = false;

bool m_ifastcrashing = false;

bool m_inorelaod = false;

bool m_Ýnofalldamage = false;

bool m_invisiblehelmet = false;

bool m_ifastrespasn = false;

bool m_ihidename = false;

bool m_ifastgun = false; //

bool wallhack2 = false;

bool fastdall = false;

bool m_iunlimitedammo = false;

bool m_ihpbot = false;

DWORD WINAPI MainThread(HMODULE hModule, LPDIRECT3DDEVICE9 pDevice)
{
	while (true)
	{
		
		PointBlank = (DWORD)GetModuleHandle("PointBlank.exe"); 
		
		
		if (m_ihpbot == true) // succes
		{
			DWORD WallHacks = PointBlank + 0x5170C7;
			DWORD WallValue = 2425393296;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}


		if (m_ihpbot == false) // succes
		{
			DWORD WallHacks = PointBlank + 0x5170C7;
			DWORD WallValue = 1091637235;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}
		
		
		
		
		
		if (m_iunlimitedammo == true) // 
		{
			DWORD WallHacks = PointBlank + 0x4CF94D;// 
			DWORD WallValue = 2425393296;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}


		if (m_iunlimitedammo == false) // succes
		{
			DWORD WallHacks = PointBlank + 0x4CF94D;
			DWORD WallValue = 2165379059;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}
		
		
		
		
		if (wallhack2 == true) // succes
		{
			DWORD WallHacks = PointBlank + 0x425323;
			DWORD WallValue = 2336912385;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}

	
		if (wallhack2 == false) // succes
		{
			DWORD WallHacks = PointBlank + 0x425323;
			DWORD WallValue = 2336912384;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}

		
		if (fastdall == true) // succes
		{
			DWORD Reco = PointBlank + 0x00FBFD90;
			float RecoVALUE = -99;
			DWORD RC1, RC2, RC3;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x4F4), &RC2, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC2 + 0x130), &RC3, 4, NULL);
			
		}


		if (fastdall == false) // succes
		{
			DWORD Reco = PointBlank + 0x00FBFD90;
			float RecoVALUE = 0;
			DWORD RC1, RC2, RC3;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x4F4), &RC2, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC2 + 0x130), &RC3, 4, NULL);
		}
		
		
		
		
		if (m_iRecoil == true) // succes
		{
			DWORD WallHacks = PointBlank + 0x4DC5F2;
			DWORD WallValue = 2215575720;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}
		if (m_iRecoil == false) // succes
		{
			DWORD WallHacks = PointBlank + 0x4DC5F2;
			DWORD WallValue = 2215624836;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// );
		}
		if (m_ifastgun == true)
		{
			DWORD Reco = PointBlank + 0x00FBFD90;
			float RecoVALUE = 0;
			DWORD RC1, RC2, RC3;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x4FC), &RC2, 4, NULL);
			
		}


		if (m_ifastgun == false)
		{
			DWORD Reco = PointBlank + 0x00FBFD90;
			float RecoVALUE = 0;
			DWORD RC1, RC2, RC3;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x4FC), &RC2, 4, NULL);

		}


		if (m_ihidename == true)
		{
			DWORD WallHacks = PointBlank + 0x4F1B3C;
			DWORD WallValue = 2215575720;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}


		if (m_ihidename == false)
		{
			DWORD WallHacks = PointBlank + 0x4F1B3C;
			DWORD WallValue = 2215624836;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}





		if (m_ifastrespasn == true)
		{
			DWORD WallHacks = PointBlank + 0x664626;
			DWORD WallValue = 3894167001;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}


		if (m_ifastrespasn == false)
		{
			DWORD WallHacks = PointBlank + 0x664626;
			DWORD WallValue = 3893118425;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}


		if (m_invisiblehelmet == true)
		{
			DWORD WallHacks = PointBlank + 0x4E4E1E;
			DWORD WallValue = 2366705651;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}


		if (m_invisiblehelmet == false)
		{
			DWORD WallHacks = PointBlank + 0x4E4E1E;
			DWORD WallValue = 2232487923;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}


		if (m_Ýnofalldamage == true) // succes
		{
			DWORD Reco = PointBlank + 0x00FBFD90;
			float RecoVALUE = 0;
			DWORD RC1, RC2, RC3;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x440), &RC2, 4, NULL);


		}


		if (m_Ýnofalldamage == false) // succes
		{
			DWORD Reco = PointBlank + 0x00FBFD90;
			float RecoVALUE = 0;
			DWORD RC1, RC2, RC3;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x440), &RC2, 4, NULL);


		}




		if (m_inorelaod == true) // succes
		{
			DWORD Reco = PointBlank + 0x00FBFD90;
			float RecoVALUE = 3065432129;
			DWORD RC1, RC2, RC3;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x77), &RC2, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC2 + 0x30), &RC3, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC2 + 0x3C), &RC3, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC2 + 0x6A4), &RC3, 4, NULL);

		}


		if (m_inorelaod == false) // succes
		{
			DWORD Reco = PointBlank + 0x00FBFD90;
			float RecoVALUE = 0;
			DWORD RC1, RC2, RC3;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x77), &RC2, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC2 + 0x30), &RC3, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC2 + 0x3C), &RC3, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC2 + 0x6A4), &RC3, 4, NULL);

		}

		if (m_ifastcrashing == true) // succes
		{
			DWORD Reco = PointBlank + 0x00FBFD90;
			float RecoVALUE = -99;
			DWORD RC1, RC2, RC3;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x4F4), &RC2, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC2 + 0x130), &RC3, 4, NULL);

		}


		if (m_ifastcrashing == false) // succes
		{
			DWORD Reco = PointBlank + 0x00FBFD90;
			float RecoVALUE = 0;
			DWORD RC1, RC2, RC3;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x4F4), &RC2, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC2 + 0x130), &RC3, 4, NULL);

		}

		if (m_iRecoilRight == true) // succes
		{
			DWORD Reco = PointBlank + 0x00FBFD90;
			float RecoVALUE = 0;
			DWORD RC1, RC2, RC3;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x6A4), &RC2, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x3C), &RC2, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC2 + 0x200), &RC3, 4, NULL);

		}


		if (m_iRecoilUP == false) // succes
		{
			DWORD Reco = PointBlank + 0x00FBFD90;
			float RecoVALUE = 0;
			DWORD RC1, RC2, RC3;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x514), &RC2, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC2 + 0x9c), &RC3, 4, NULL);

		}

		if (m_iRecoilUP == true) // succes
		{
			DWORD Reco = PointBlank + 0x00FBFD90;
			float RecoVALUE = 0;
			DWORD RC1, RC2, RC3;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x514), &RC2, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC2 + 0x9c), &RC3, 4, NULL);

		}


		if (m_iRecoilUP == false) // succes
		{
			DWORD Reco = PointBlank + 0x00FBFD90;
			float RecoVALUE = 0;
			DWORD RC1, RC2, RC3;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x514), &RC2, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC2 + 0x9c), &RC3, 4, NULL);

		}

		if (m_iRecoil == true) // succes
		{
			DWORD WallHacks = PointBlank + 0x4DC5F2;
			DWORD WallValue = 2215575720;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}
		if (m_iRecoil == false) // succes
		{
			DWORD WallHacks = PointBlank + 0x4DC5F2;
			DWORD WallValue = 2215624836;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// );
		}
		
			if (m_iRepidFire == true) // succes
			{
				DWORD Reco = PointBlank + 0x00FBFD90;
				float RecoVALUE = 0;
				DWORD RC1, RC2;
				ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
				ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x4FC), &RC2, 4, NULL);

			}
		if (m_iRepidFire == false) // succes
		{
			DWORD Reco = PointBlank + 0x00FBFD90;
			float RecoVALUE = 2;
			DWORD RC1, RC2;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x4FC), &RC2, 4, NULL);

		}
		if (m_iRecoil == false) // succes
		{
			DWORD Reco = PointBlank + 0x00F57C74;
			float RecoVALUE = 69;
			DWORD RC1, RC2;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x38), &RC2, 4, NULL);
			WriteProcessMemory(MEMORY, (LPVOID)(RC2 + 0x380), &RecoVALUE, 4, NULL);
		}
	
			if (m_iWeaponFast == true) // succes
			{
				DWORD ADRES15 = PointBlank + 0x0F57C74;
				DWORD VALUE15 = 0;
				DWORD TMP16;
				ReadProcessMemory(MEMORY, (LPVOID)(ADRES15), &TMP16, 4, NULL);
				WriteProcessMemory(MEMORY, (LPVOID)(TMP16 + 0x2F4), &VALUE15, 4, NULL);
			}
		if (m_iWeaponFast == false) // succes
		{
			DWORD ADRES15 = PointBlank + 0x0F57C74;
			DWORD VALUE15 = 0;
			DWORD TMP16;
			ReadProcessMemory(MEMORY, (LPVOID)(ADRES15), &TMP16, 4, NULL);
			WriteProcessMemory(MEMORY, (LPVOID)(TMP16 + 0x2F4), &VALUE15, 4, NULL);
		}
	
			if (m_iInfAmmo == true) // succes knk hepsi true :D
			{
				DWORD Reco = PointBlank + 0x00F57C74;
				float RecoVALUE = 45;
				DWORD RC1, RC2;
				ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
				ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x38), &RC2, 4, NULL);
				WriteProcessMemory(MEMORY, (LPVOID)(RC2 + 0x2FC), &RecoVALUE, 4, NULL);
			}
		if (m_iInfAmmo == false) // succes
		{
			DWORD Reco = PointBlank + 0x00F57C74;
			float RecoVALUE = 45; //
			DWORD RC1, RC2;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x38), &RC2, 4, NULL);
			WriteProcessMemory(MEMORY, (LPVOID)(RC2 + 0x2FC), &RecoVALUE, 4, NULL);
		}

		if (m_iHealthBlue == true)
		{
			DWORD WallHacks = PointBlank + 0x00938552;
			DWORD WallValue = 3397717761; // dword
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}
		if (m_iHealthBlue == false)
		{
			DWORD WallHacks = PointBlank + 0x00938552;
			DWORD WallValue = 3397717760;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}
	
			if (m_iHealthRed == true)
			{
				DWORD WallHacks = PointBlank + 0x4D9509;
				DWORD WallValue = 202444787; // dword
				WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
			}
		if (m_iHealthRed == false)
		{
			DWORD WallHacks = PointBlank + 0x4D9509;
			DWORD WallValue = 68227059;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}
		
			if (m_iCharacterSize == true)
			{
				DWORD WallHacks = PointBlank + 0x3F2E40;// 
				DWORD WallValue = 2415978627; // dword
				WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
			}
		if (m_iCharacterSize == false)
		{
			DWORD WallHacks = PointBlank + 0x3F2E40;
			DWORD WallValue = 2418075779;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}
	
			if (m_iDuelRecoil == true)
			{
				DWORD Reco = PointBlank + 0xF57C74;;
				DWORD RecoVALUE = 0;
				DWORD RC1, RC2, RC3;
				ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
				ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x38), &RC2, 4, NULL);
				ReadProcessMemory(MEMORY, (LPVOID)(RC2 + 0x30), &RC3, 4, NULL);
				WriteProcessMemory(MEMORY, (LPVOID)(RC3 + 0x338), &RecoVALUE, 4, NULL);
			}
		if (m_iDuelRecoil == false)
		{
			DWORD Reco = PointBlank + 0xF57C74;;
			DWORD RecoVALUE = 0;
			DWORD RC1, RC2, RC3;
			ReadProcessMemory(MEMORY, (LPVOID)(Reco), &RC1, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC1 + 0x38), &RC2, 4, NULL);
			ReadProcessMemory(MEMORY, (LPVOID)(RC2 + 0x30), &RC3, 4, NULL);
			WriteProcessMemory(MEMORY, (LPVOID)(RC3 + 0x338), &RecoVALUE, 4, NULL);
		}
		if (m_iRecoilFully == true)
		{
			DWORD WallHacks = PointBlank + 0x4DC5F2;
			DWORD WallValue = 2215575720;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}
		if (m_iRecoilFully == false)
		{
			DWORD WallHacks = PointBlank + 0x4DC5F2;
			DWORD WallValue = 2215624836;
			WriteProcessMemory(MEMORY, (LPVOID)WallHacks, &WallValue, 4, NULL);// 
		}
	}

}


HRESULT APIENTRY DrawIndexedPrimitive_hook(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	if (m_iChams == true) // bitti
	{
		if (Stride == 44) //
		{
			pDevice->SetTexture(0, Green);
			pDevice->SetTexture(1, Green);
		}
	}




	if (m_iChamsBlue == true) // bitti
	{
		if (Stride == 44) //
		{
			pDevice->SetTexture(0, Blue);
			pDevice->SetTexture(1, Blue);
		}
	}





	



	if (m_iredchams == true) // bitti
	{
		if (Stride == 44) // 
		{
			pDevice->SetTexture(0, Red);
			pDevice->SetTexture(1, Red);
		}
	}



	if (wallhack == true)
	{
		if (Stride == 44) // 
		{
			/*WIREFRAME: pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);*/
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);
		}
		
			

	}




	if (wriframe == true)
	{
		if (Stride == 44) // 
		{
			WIREFRAME: pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);
		}

	}






	return DrawIndexedPrimitive_orig(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}



//=====================================================================================================================



HRESULT APIENTRY EndScene_hook(IDirect3DDevice9* pDevice)
{


	if (pDevice == nullptr) return EndScene_orig(pDevice);


	if (!is_imgui_initialised)
	{
		//get viewport
		pDevice->GetViewport(&Viewport);
		ScreenCX = (float)Viewport.Width / 2.0f;
		ScreenCY = (float)Viewport.Height / 2.0f;

		
		GenerateTexture(pDevice, &Red, D3DCOLOR_ARGB(255, 255, 0, 0));
		GenerateTexture(pDevice, &Green, D3DCOLOR_RGBA(0, 255, 0, 255));
		GenerateTexture(pDevice, &Blue, D3DCOLOR_ARGB(255, 0, 0, 255));
		GenerateTexture(pDevice, &Yellow, D3DCOLOR_ARGB(255, 255, 255, 0));

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard; //control menu with mouse
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

		// Find a handle to the first top-level window belonging to the game process.
		//EnumWindows(find_game_hwnd, GetCurrentProcessId());

		// Window handle to which focus belongs for this Direct3D device (similar to EnumWindows.. above)
		D3DDEVICE_CREATION_PARAMETERS d3dcp;
		pDevice->GetCreationParameters(&d3dcp);
		game_hwnd = d3dcp.hFocusWindow;

		if (game_hwnd != NULL)
		{
			// Swap out the window message handler for our own, allowing us to intercept input events
			game_wndproc = (WNDPROC)SetWindowLongPtr(game_hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

			// Perform final ImGui setup tasks and..
			ImGui_ImplWin32_Init(game_hwnd);
			ImGui_ImplDX9_Init(pDevice);
			ImGui::GetIO().ImeWindowHandle = game_hwnd;

			is_imgui_initialised = true;
		}
	}


	/*
	//may have to delay shitproc for 2 sec in a few games
	if(!is_wndproc_initialised)
	if (timeGetTime() - wndproctime >= 2000)
	{
		is_wndproc_initialised = true;
		//Log("is_wndproc_initialised");
		// Swap out the window message handler for our own, allowing us to intercept input events.
		game_wndproc = (WNDPROC)SetWindowLongPtr(game_hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
		wndproctime = timeGetTime();
	}
	*/

	//info
	//if (info)
	//{
	//	ImGui::SetNextWindowSize(ImVec2(560.0f, 20.0f)); //size
	//	ImVec4 Bgcol = ImColor(0.19f, 0.19f, 0.19f, 0.29f);
	//	ImGui::PushStyleColor(ImGuiCol_WindowBg, Bgcol);
	//	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.2f, 0.8f));

	//	ImGui::Begin("RuzyHackers Free Cheats", &info, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);
	//	ImGui::Text("INSERT to Open the Menu");
	//	ImGui::End();

	//	static DWORD lastTime = timeGetTime();
	//	DWORD timePassed = timeGetTime() - lastTime;
	//	if (timePassed > 9000)
	//	{
	//		info = false;
	//		lastTime = timeGetTime();
	//	}
	//}

	//menu key
	//GetAsyncKeyState & 1 may not work in every game
	static auto is_down = false;
	static auto is_clicked = false;
	if (GetAsyncKeyState(VK_INSERT))
	{
		is_clicked = false;
		is_down = true;
	}
	else if (!GetAsyncKeyState(VK_INSERT) && is_down)
	{
		is_clicked = true;
		is_down = false;
	}
	else {
		is_clicked = false;
		is_down = false;
	}

	//show menu
	if (is_clicked)
	{
		ShowMenu = !ShowMenu;
		//save settings
		SaveCfg();
	}

	//mouse cursor on in menu
	//ImGuiIO& io = ImGui::GetIO();
	if (ShowMenu)
		ImGui::GetIO().MouseDrawCursor = 1;
	//io.MouseDrawCursor = true;
	else
		ImGui::GetIO().MouseDrawCursor = 0;
	//io.MouseDrawCursor = false;

//draw menu
	if (ShowMenu)
	{
		

		
			ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame(); 
		ImGui::NewFrame();
		
		
		//ImGui::SetNextWindowPos(ImVec2(50.0f, 400.0f)); //pos
		ImGui::SetNextWindowSize(ImVec2(371.1f, 590.1f)); //size
		ImVec4 Bgcol = ImColor(0.06f, 0.05f, 0.07f, 1.00f); //bg color
		ImGui::PushStyleColor(ImGuiCol_WindowBg, Bgcol);
		ImGui::PushStyleColor(ImGuiCol_Border, Bgcol);

		

		ImGui::Begin("RuzyHackers PointBlank Cheats");
		bool status = false;
		void my_display_code();
		{
			
			if (ImGui::CollapsingHeader("DirectX"))
			{
				ImGui::Text("Red Chams:   ");
				ImGui::SameLine(135);
				ToggleButton("button1   ", &m_iredchams);
				
				ImGui::Text("Green Chams:   ");
				ImGui::SameLine(135);
				ToggleButton("button2   ", &m_iChams);

				ImGui::Text("Blue Chams:   ");
				ImGui::SameLine(135);
				ToggleButton("button3   ", &m_iChamsBlue);
			
				ImGui::Text("WallHack:   ");
				ImGui::SameLine(135);
				ToggleButton("button4   ", &wallhack);

				ImGui::Text("WallHack 2:   ");
				ImGui::SameLine(135);
				ToggleButton("button4   ", &wallhack2);
				
				ImGui::Text("Wire Frame:   ");
				ImGui::SameLine(135);
				ToggleButton("button5   ", &wriframe);
			
			}

			if (ImGui::CollapsingHeader("Weapon"))
			{
				
				ImGui::Text("Unlimited Ammo:   ");
				ImGui::SameLine(135);
				ToggleButton("button1   ", &m_iunlimitedammo);
				ImGui::Text("No Recoil:   ");
				ImGui::SameLine(135);
				ToggleButton("button6   ", &m_iRecoil);
				ImGui::Text("Fast Crashing:   ");
				ImGui::SameLine(135);
				ToggleButton("button7   ", &m_ifastcrashing);
				ImGui::Text("Recoil Up:   ");
				ImGui::SameLine(135);
				ToggleButton("button8   ", &m_iRecoilUP);
				ImGui::Text("Right Left Recoil:   ");
				ImGui::SameLine(135);
				ToggleButton("button9   ", &m_iRecoilRight);
				ImGui::Text("Fast Weapon:   ");
				ImGui::SameLine(135);
				ToggleButton("button10   ", &m_iRepidFire);
				ImGui::Text("No Reload:   ");
				ImGui::SameLine(135);
				ToggleButton("button11   ", &m_inorelaod);
				ImGui::Text("Fast Gun:   ");
				ImGui::SameLine(135);
				ToggleButton("button12  ", &m_ifastgun);
				ImGui::ShowStyleEditor;
			
				
			}


			if (ImGui::CollapsingHeader("Player"))
			{

			
				ImGui::Text("God Mode (Bots):   ");
				ImGui::SameLine(135);
				ToggleButton("button155   ", &m_ihpbot);

				
				
				
				ImGui::Text("Ýnvisible Helmet:   ");
				ImGui::SameLine(135);
				ToggleButton("button13   ", &m_invisiblehelmet);


				ImGui::Text("Fast Respawn:   ");
				ImGui::SameLine(135);
				ToggleButton("button14   ", &m_ifastrespasn);


				ImGui::Text("NoFallDamage:   ");
				ImGui::SameLine(135);
				ToggleButton("button16   ", &m_inofalldamage);


				ImGui::Text("Hide Name:   ");
				ImGui::SameLine(135);
				ToggleButton("button15   ", &m_ihidename);


				


			}
		
		}
		// Rendering
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
	
		/*ImGui::Text("[+ Undetected]");
		ImGui::PopStyleColor();
		ImGui::End();*/
		
		
	
	}


	
	bool status = false;
	void my_display_code();

	//Shift|RMouse|LMouse|Ctrl|Alt|Space|X|C
	if (aimkey == 0) Daimkey = 0;
	if (aimkey == 1) Daimkey = VK_SHIFT;
	if (aimkey == 2) Daimkey = VK_RBUTTON;
	if (aimkey == 3) Daimkey = VK_LBUTTON;
	if (aimkey == 4) Daimkey = VK_CONTROL;
	if (aimkey == 5) Daimkey = VK_MENU;
	if (aimkey == 6) Daimkey = VK_SPACE;
	if (aimkey == 7) Daimkey = 0x58; //X
	if (aimkey == 8) Daimkey = 0x43; //C

	//do esp
	if (ModelEspInfo.size() != NULL)
	{
		for (unsigned int i = 0; i < ModelEspInfo.size(); i++)
		{
			if (ModelEspInfo[i].pOutX > 1.0f && ModelEspInfo[i].pOutY > 1.0f)
			{
				ImGui::Begin("Transparent", reinterpret_cast<bool*>(true), ImVec2(0, 0), 0.0f, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);
				ImGui::SetWindowPos(ImVec2(0, 0));
				ImGui::SetWindowSize(ImVec2(GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN)));
				ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(ModelEspInfo[i].pOutX, ModelEspInfo[i].pOutY), ImColor(255, 255, 0, 255), "Model", 0, 0.0f, 0);
				//int mfps = ImGui::GetIO().Framerate;
				//ImDrawText(string("FPS [" + to_string(mfps) + "]"), ImVec2(0, 0), 18.0f, (mfps > 25.0f) ? ImColor(0, 255, 0) : ImColor(255, 0, 0));
				ImGui::End();
			}
		}
	}

	//do aim
	if (ModelEspInfo.size() != NULL)
	{
		UINT BestTarget = -1;
		DOUBLE fClosestPos = 99999;

		for (unsigned int i = 0; i < ModelEspInfo.size(); i++)
		{
			//aimfov
			float radiusx = (aimfov*5.0f) * (ScreenCX / 100.0f);
			float radiusy = (aimfov*5.0f) * (ScreenCY / 100.0f);

			if (aimfov == 0)
			{
				radiusx = 5.0f * (ScreenCX / 100.0f);
				radiusy = 5.0f * (ScreenCY / 100.0f);
			}

			//get crosshairdistance
			ModelEspInfo[i].CrosshairDistance = GetDistance(ModelEspInfo[i].pOutX, ModelEspInfo[i].pOutY, ScreenCX, ScreenCY);

			//if in fov
			if (ModelEspInfo[i].pOutX >= ScreenCX - radiusx && ModelEspInfo[i].pOutX <= ScreenCX + radiusx && ModelEspInfo[i].pOutY >= ScreenCY - radiusy && ModelEspInfo[i].pOutY <= ScreenCY + radiusy)

				//get closest/nearest target to crosshair
				if (ModelEspInfo[i].CrosshairDistance < fClosestPos)
				{
					fClosestPos = ModelEspInfo[i].CrosshairDistance;
					BestTarget = i;
				}
		}


		//if nearest target to crosshair
		if (BestTarget != -1)
		{
			double DistX = ModelEspInfo[BestTarget].pOutX - ScreenCX;
			double DistY = ModelEspInfo[BestTarget].pOutY - ScreenCY;

			DistX /= (1.0f + (float)aimsens*1.0f);
			DistY /= (1.0f + (float)aimsens*1.0f);

			//aim
			if (GetKeyState(Daimkey) & 0x8000)
				mouse_event(MOUSEEVENTF_MOVE, (float)DistX, (float)DistY, 0, NULL);

			//autoshoot on
			if ((!GetAsyncKeyState(VK_LBUTTON) && (autoshoot == 1) && (GetAsyncKeyState(Daimkey) & 0x8000)))
			{
				if (autoshoot == 1 && !IsPressed)
				{
					IsPressed = true;
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				}
			}
		}
	}
	ModelEspInfo.clear();

	//autoshoot off
	if (autoshoot == 1 && IsPressed)
	{
		if (timeGetTime() - astime >= asdelay)
		{
			IsPressed = false;
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			astime = timeGetTime();
		}
	}




	return EndScene_orig(pDevice);
}

//==========================================================================================================================

HRESULT APIENTRY Reset_hook(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	HRESULT ResetReturn = Reset_orig(pDevice, pPresentationParameters);

	ImGui_ImplDX9_CreateDeviceObjects();

	return ResetReturn;
}

//==========================================================================================================================

HRESULT APIENTRY SetStreamSource_hook(LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT sStride)
{
	if (StreamNumber == 0)
		Stride = sStride;

	return SetStreamSource_orig(pDevice, StreamNumber, pStreamData, OffsetInBytes, sStride);
}

//==========================================================================================================================

HRESULT APIENTRY SetVertexDeclaration_hook(LPDIRECT3DDEVICE9 pDevice, IDirect3DVertexDeclaration9* pdecl)
{
	if (pdecl != NULL)
	{
		pdecl->GetDeclaration(decl, &numElements);
	}

	return SetVertexDeclaration_orig(pDevice, pdecl);
}

//==========================================================================================================================

HRESULT APIENTRY SetVertexShaderConstantF_hook(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, const float *pConstantData, UINT Vector4fCount)
{
	if (pConstantData != NULL)
	{
		mStartregister = StartRegister;
		mVectorCount = Vector4fCount;
	}

	return SetVertexShaderConstantF_orig(pDevice, StartRegister, pConstantData, Vector4fCount);
}

//==========================================================================================================================

HRESULT APIENTRY SetVertexShader_hook(LPDIRECT3DDEVICE9 pDevice, IDirect3DVertexShader9 *veShader)
{
	if (veShader != NULL)
	{
		vShader = veShader;
		vShader->GetFunction(NULL, &vSize);
	}
	return SetVertexShader_orig(pDevice, veShader);
}

//==========================================================================================================================

HRESULT APIENTRY SetPixelShader_hook(LPDIRECT3DDEVICE9 pDevice, IDirect3DPixelShader9 *piShader)
{
	if (piShader != NULL)
	{
		pShader = piShader;
		pShader->GetFunction(NULL, &pSize);
	}
	return SetPixelShader_orig(pDevice, piShader);
}

//==========================================================================================================================

HRESULT APIENTRY DrawPrimitive_hook(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	//in some games hp bars are drawn here

	return DrawPrimitive_orig(pDevice, PrimitiveType, StartVertex, PrimitiveCount);
}

//==========================================================================================================================

HRESULT APIENTRY SetTexture_hook(LPDIRECT3DDEVICE9 pDevice, DWORD Sampler, IDirect3DBaseTexture9 *pTexture)
{
	/*
	//get texture/desc size (usually not needed, decreases fps)
	texture = static_cast<IDirect3DTexture9*>(pTexture);
	if (texture)
	{
		if (FAILED(texture->GetLevelDesc(0, &sDesc)))
			goto out;

		if (sDesc.Pool == D3DPOOL_MANAGED && texture->GetType() == D3DRTYPE_TEXTURE)
		{
			HRESULT hr = texture->LockRect(0, &pLockedRect, NULL, D3DLOCK_DONOTWAIT | D3DLOCK_READONLY | D3DLOCK_NOSYSLOCK);

			if (SUCCEEDED(hr))
			{
				if (pLockedRect.pBits != NULL)
					qCRC = QuickChecksum((DWORD*)pLockedRect.pBits, pLockedRect.Pitch); //get crc
					//qCRC = QuickChecksum((DWORD*)pLockedRect.pBits, 12);
			}
			texture->UnlockRect(0);
		}
	}
out:
	*/
	return SetTexture_orig(pDevice, Sampler, pTexture);
}

//==========================================================================================================================

HRESULT APIENTRY Present_hook(IDirect3DDevice9* pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion)
{
	return Present_orig(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

//=====================================================================================================================

DWORD WINAPI D3Dimgui(LPVOID lpParameter)
{
	while (!GetModuleHandleA("d3d9.dll")) {
		Sleep(200);
	}

	IDirect3D9* d3d = NULL;
	IDirect3DDevice9* d3ddev = NULL;

	HWND tmpWnd = CreateWindowA("BUTTON", "DX", WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, NULL, NULL, Hand, NULL);
	if (tmpWnd == NULL)
	{
		//Log("[DirectX] Failed to create temp window");
		return 0;
	}

	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL)
	{
		DestroyWindow(tmpWnd);
		//Log("[DirectX] Failed to create temp Direct3D interface");
		return 0;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = tmpWnd;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	HRESULT result = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, tmpWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
	if (result != D3D_OK)
	{
		d3d->Release();
		DestroyWindow(tmpWnd);
		MessageBox(game_hwnd, "Run the game first and inject dll later", "Failed to create temp Direct3D device.", MB_OK);
		//MessageBox(game_hwnd, L"Run the game first and inject dll later", L"Failed to create temp Direct3D device.", MB_OK);
		//Log("[DirectX] Failed to create temp Direct3D device. Run the game first and inject dll later"); 
		return 0;
	}

	// We have the device, so walk the vtable to get the address of all the dx functions in d3d9.dll
#if defined _M_X64
	DWORD64* dVtable = (DWORD64*)d3ddev;
	dVtable = (DWORD64*)dVtable[0];
#elif defined _M_IX86
	DWORD* dVtable = (DWORD*)d3ddev;
	dVtable = (DWORD*)dVtable[0]; // == *d3ddev
#endif
	//Log("[DirectX] dVtable: %x", dVtable);

	//for(int i = 0; i < 95; i++)
	//{
			//Log("[DirectX] vtable[%i]: %x, pointer at %x", i, dVtable[i], &dVtable[i]);
	//}

	// Detour functions x86 & x64
	SetStreamSource_orig = (SetStreamSource)dVtable[100];
	SetVertexDeclaration_orig = (SetVertexDeclaration)dVtable[87];
	SetVertexShaderConstantF_orig = (SetVertexShaderConstantF)dVtable[94];
	SetVertexShader_orig = (SetVertexShader)dVtable[92];
	SetPixelShader_orig = (SetPixelShader)dVtable[107];

	DrawIndexedPrimitive_orig = (DrawIndexedPrimitive)dVtable[82];
	DrawPrimitive_orig = (DrawPrimitive)dVtable[81];
	SetTexture_orig = (SetTexture)dVtable[65];
	Present_orig = (Present)dVtable[17];
	EndScene_orig = (EndScene)dVtable[42];
	Reset_orig = (Reset)dVtable[16];
	
	Sleep(2000);//required in a few games

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(LPVOID&)SetStreamSource_orig, (PBYTE)SetStreamSource_hook);
	DetourAttach(&(LPVOID&)SetVertexDeclaration_orig, (PBYTE)SetVertexDeclaration_hook);
	DetourAttach(&(LPVOID&)SetVertexShaderConstantF_orig, (PBYTE)SetVertexShaderConstantF_hook);
	DetourAttach(&(LPVOID&)SetVertexShader_orig, (PBYTE)SetVertexShader_hook);
	DetourAttach(&(LPVOID&)SetPixelShader_orig, (PBYTE)SetPixelShader_hook);

	DetourAttach(&(LPVOID&)DrawIndexedPrimitive_orig, (PBYTE)DrawIndexedPrimitive_hook);
	DetourAttach(&(LPVOID&)DrawPrimitive_orig, (PBYTE)DrawPrimitive_hook);
	DetourAttach(&(LPVOID&)SetTexture_orig, (PBYTE)SetTexture_hook);
	DetourAttach(&(LPVOID&)Present_orig, (PBYTE)Present_hook);
	DetourAttach(&(LPVOID&)EndScene_orig, (PBYTE)EndScene_hook);
	DetourAttach(&(LPVOID&)Reset_orig, (PBYTE)Reset_hook);
	DetourTransactionCommit();
	
	//Log("[Detours] Detours attached\n");

	d3ddev->Release();
	d3d->Release();
	DestroyWindow(tmpWnd);

	return 1;
}

//==========================================================================================================================

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Hand = hModule;
		DisableThreadLibraryCalls(hModule); //disable unwanted thread notifications to reduce overhead
		GetModuleFileNameA(hModule, dlldir, 512);
		for (int i = (int)strlen(dlldir); i > 0; i--)
		{
			if (dlldir[i] == '\\')
			{
				dlldir[i + 1] = 0;
				break;
			}
		}
		CreateThread(0, 0, D3Dimgui, 0, 0, 0); //init our hooks
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));

		//AllocConsole(); // Enables the console
		//freopen("CONIN$", "r", stdin); 
		//freopen("CONOUT$", "w", stdout);
		break;
	case DLL_PROCESS_DETACH:
		// Uninitialize wndProc
		//SetWindowLongPtr(game_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG>(WndProc));
		// Uninitialize ImGui
		//ImGui_ImplWin32_Shutdown();
		//ImGui_ImplDX9_Shutdown();
		//ImGui::DestroyContext();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}
