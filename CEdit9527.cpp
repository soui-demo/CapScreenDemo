#include "stdafx.h"
#include "CEdit9527.h"

namespace SOUI
{
	CEdit9527::CEdit9527() :m_bDraging(FALSE)
	{
	}

	CEdit9527::~CEdit9527()
	{
	}

	void CEdit9527::PaintToDC(HDC hdc)
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		RECTL rcL = { rcClient.left,rcClient.top,rcClient.right,rcClient.bottom };
		int nOldMode = ::SetGraphicsMode(hdc, GM_COMPATIBLE);
		m_pTxtHost->GetTextService()->TxDraw(
			DVASPECT_CONTENT,          // Draw Aspect
			/*-1*/0,                        // Lindex
			NULL,                    // Info for drawing optimazation
			NULL,                    // target device information
			hdc,            // Draw device HDC
			NULL,                        // Target device HDC
			&rcL,            // Bounding client rectangle
			NULL,             // Clipping rectangle for metafiles
			&rcClient,        // Update rectangle
			NULL,                        // Call back function
			NULL,                    // Call back parameter
			TXTVIEW_ACTIVE);
		::SetGraphicsMode(hdc, nOldMode);
	}

	LRESULT CEdit9527::OnCreate(LPVOID)
	{
		LRESULT bRet = __super::OnCreate(NULL);
		if (bRet == 0)
		{
			SSendMessage(EM_SETEVENTMASK, 0, ENM_CHANGE);
			GetEventSet()->subscribeEvent(EVT_RE_NOTIFY, Subscriber(&CEdit9527::OnEditNotify, this));
			SSendMessage(EM_SETTARGETDEVICE, 0, 0);
			GetEventSet()->subscribeEvent(EVT_KILLFOCUS, Subscriber(&CEdit9527::OnKillFocus, this));
		}
		return bRet;
	}

	bool CEdit9527::OnKillFocus(EventArgs * e)
	{
		GetEventSet()->setMutedState(true);
		__super::OnKillFocus(m_swnd);
		if (GetWindowText().IsEmpty())
		{
			GetParent()->DestroyChild(this);
		}
		GetEventSet()->setMutedState(false);
		return true;
	}

	bool CEdit9527::OnEditNotify(EventArgs * e)
	{
		EventRENotify *pEvtNotify = sobj_cast<EventRENotify>(e);		
		if (pEvtNotify->iNotify == EN_CHANGE)
		{		
			if (HasScrollBar(TRUE))
			{
				int iMin, iMax;
				GetScrollRange(TRUE, &iMin, &iMax);
				SStringT height;
				height.Format(L"%d", iMax + 6);
				SetAttribute(L"height", height);
			}
			else
			{

			}
		}
		return true;
	}

	BOOL CEdit9527::OnSetCursor(const CPoint & pt)
	{
		if (!__super::OnSetCursor(pt))
		{
			SetCursor(GETRESPROVIDER->LoadCursor(IDC_SIZEALL));
		}
		return TRUE;
	}

	void CEdit9527::OnNcLButtonDown(UINT nHitTest, CPoint point)
	{
		__super::OnNcLButtonDown(nHitTest, point);
		SetCapture();
		m_bDraging = TRUE;
		m_ptClick = point;
		BringWindowToTop();
	}

	void CEdit9527::OnNcMouseMove(UINT nHitTest, CPoint point)
	{
		if (m_bDraging)
		{
			CRect rcWnd = GetWindowRect();
			CPoint pt = rcWnd.TopLeft() + (point - m_ptClick);
			rcWnd.MoveToXY(pt);
			Move(rcWnd);
			m_ptClick = point;
			GetParent()->Invalidate();
		}
	}

	void CEdit9527::OnNcLButtonUp(UINT nHitTest, CPoint point)
	{
		m_bDraging = FALSE;
		ReleaseCapture();
		CRect parentRc = GetParent()->GetClientRect();
		CRect rcWnd = GetWindowRect();
		CPoint relpos = rcWnd.TopLeft() - parentRc.TopLeft();
		SStringT pos;
		pos.Format(L"%d,%d", relpos.x, relpos.y);
		SetAttribute(L"pos", pos);
		Move(NULL);
	}

	void CEdit9527::OnNcPaint(IRenderTarget * pRT)
	{
		if (!IsVisible(TRUE)) return;
		if (!m_style.GetMargin().IsRectNull() && IsFocused())
		{
			CAutoRefPtr<IPen> pen, oldpen;
			pRT->CreatePen(PS_DASHDOT, RGBA(0, 0, 0, 255), 1, &pen);
			pRT->SelectObject(pen, (IRenderObj**)&oldpen);
			CRect rcWindow = GetWindowRect();
			//CRect rcClient = GetClientRect();
			rcWindow.InflateRect(0, 0, 1, 1);
			pRT->DrawRectangle(rcWindow);
			pRT->SelectObject(oldpen, NULL);
		}
	}

	BOOL CEdit9527::OnEraseBkgnd(IRenderTarget * pRT)
	{
		return TRUE;
	}

	void CEdit9527::OnMouseHover(WPARAM wParam, CPoint ptPos)
	{
		SetFocus();
	}

	void CEdit9527::OnMouseLeave()
	{
		if (!m_bDraging)
			KillFocus();
	}
}