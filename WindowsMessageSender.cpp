#include <windows.h>
#include <string>

#include "source/KrCore.h"
#include "WindowsMessageSender.h"
using namespace KrUI;


void Edit_MsgInput_Change(KrMessageHandler* pKrMessageHandler, WPARAM wParam, LPARAM lParam);
void Btn_Send_Click(KrMessageHandler* kmh, WPARAM wp, LPARAM lp);

MSGNOTE* pMsgNote=nullptr;
KrWindow* pWnd = nullptr;
KrEdit* pEdit_MsgInput = nullptr;
KrEdit* pEdit_ClassName = nullptr;
KrEdit* pEdit_WindowName = nullptr;
KrLabel* pLabel_MsgDescription = nullptr;
KrButton* pBtn_Send = nullptr;

KrUIManager* pui = KrUIManager::GetpKrUIManager();
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	pui->Initialize(hInstance);
	int cx = GetSystemMetrics(SM_CXSCREEN), cy = GetSystemMetrics(SM_CYSCREEN);
	int width = 400, height = 225;
	pWnd = pui->AddWindow(L"WindowsMessageSender(author:cildhdi)", (cx - width) / 2, (cy - height) / 2, width, height);
	pWnd->AddControl(KrLabel_t, L"������ֱ����봰�ڵ������ͱ��⣨��ͨ��spy++��ã���",10, 10 + pWnd->GetCaptionHeight(), 400, 25);
	pEdit_ClassName = dynamic_cast<KrEdit*>(pWnd->AddControl(KrEdit_t, L"", 10, 40 + pWnd->GetCaptionHeight(), 150, 25));
	pEdit_WindowName = dynamic_cast<KrEdit*>(pWnd->AddControl(KrEdit_t, L"", 180, 40 + pWnd->GetCaptionHeight(), 150, 25));
	pWnd->AddControl(KrLabel_t, L"������Ϣ����:", 10, 80 + pWnd->GetCaptionHeight(), 95, 25);
	pEdit_MsgInput = dynamic_cast<KrEdit*>(pWnd->AddControl(KrEdit_t, L"", 110, 80 + pWnd->GetCaptionHeight(), 150, 25));
	pEdit_MsgInput->RegMsg(KM_TEXTCHANGE, reinterpret_cast<MSGPROC>(Edit_MsgInput_Change));
	(pWnd->AddControl(KrLabel_t, L"��Ϣ����:", 10, 120 + pWnd->GetCaptionHeight(), 100, 25))->SetLineAlignment(Gdiplus::StringAlignmentNear);
	pLabel_MsgDescription = dynamic_cast<KrLabel*>(pWnd->AddControl(KrLabel_t, L":", 80, 120 + pWnd->GetCaptionHeight(), 400, 25));
	pLabel_MsgDescription->SetLineAlignment(Gdiplus::StringAlignmentNear);
	pBtn_Send = dynamic_cast<KrButton*>(pWnd->AddControl(KrButton_t, L"Send", 300, 180, 80, 25));
	pBtn_Send->RegMsg(KM_CLICK, reinterpret_cast<MSGPROC>(Btn_Send_Click));
	return pui->MessageLoop();
}

void Edit_MsgInput_Change(KrMessageHandler* pKrMessageHandler, WPARAM wParam, LPARAM lParam)
{
	if (pEdit_MsgInput == nullptr || pLabel_MsgDescription == nullptr) return;
	//pLabel_MsgDescription->SetName(pEdit_MsgInput->GetText().c_str()); return;
	std::wstring str = pEdit_MsgInput->GetText();
	bool bChanged = false;
	for (auto& msgnote : msglist)
	{
		if (msgnote.szMsgText == nullptr)continue;
		if (std::wstring(msgnote.szMsgText) == str)
		{
			pLabel_MsgDescription->SetName(msgnote.szNote);
			bChanged = true;
			pMsgNote = new MSGNOTE(msgnote);
			break;
		}
	}
	if (!bChanged)
	{
		pLabel_MsgDescription->SetName(L"δ�ҵ���Ϣ��");
		delete pMsgNote;
		pMsgNote = nullptr;
	}
}

void Btn_Send_Click(KrMessageHandler* kmh, WPARAM wp, LPARAM lp)
{
	if (pMsgNote==nullptr)
	{
		::MessageBox(pWnd->GetHWND(), L"�����ڴ���Ϣ��", L"����:", MB_OK);
		return;
	} 
	if (pEdit_ClassName != nullptr&&pEdit_WindowName != nullptr)
	{
		if (HWND hwnd = ::FindWindow(pEdit_ClassName->GetText().c_str(), pEdit_WindowName->GetText().c_str()))
		{
			::PostMessage(hwnd, pMsgNote->dwMsg, NULL, NULL);
		}
		else
		{
			::MessageBox(pWnd->GetHWND(), L"δ�ҵ����ڣ�", L"����:", MB_OK);
		}
	}
}
