/* 
// Copyright 2007 Alexandros Panagopoulos
//
// This software is distributed under the terms of the GNU Lesser General Public Licence
//
// This file is part of BeGUI library.
//
//    BeGUI is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    BeGUI is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with BeGUI.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FrameWindow.h"
#include "ResourceManager.h"

using namespace begui;

// Singleton instance
FrameWindow *FrameWindow::m_pInstance;

FrameWindow::FrameWindow() : 
	m_pMenu(0), 
	m_pModalDialog(0),
	m_style(MULTIPLE_SOLID)
{
}

FrameWindow::~FrameWindow()
{
	SAFE_DELETE(m_pMenu);
}

bool FrameWindow::create(int width, int height)
{
	m_left = 0;
	m_top = 0;
	m_right = width;
	m_bottom = height;

	m_children.clear();

	// create the main menu
	//m_menu.createMainMenu();
	m_pMenu = new Menu();
	m_pMenu->createMainMenu();
	addComponent(m_pMenu);

	return true;
}

void FrameWindow::resize(int width, int height)
{
	m_right = m_left + width;
	m_bottom = m_top + height;
}

void FrameWindow::resetViewport()
{
	// Setup and orthogonal, pixel-to-pixel projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(m_left, m_right, m_bottom, m_top, 0.0, 1.0);

	// Setup the viewport
	glViewport(m_left, m_top, getWidth(), getHeight());
}

void FrameWindow::frameRender()
{
	resetViewport();

	Container::frameRender();

	// Render the modal dialog, if any
	if (m_pModalDialog)
	{
		// Render a blended fs quad to darken the bg
		glEnable(GL_BLEND);
		glColor4f(0.3,0.3,0.31,0.6);
		glBegin(GL_QUADS);
			glVertex3f(m_left, m_top, 0);
			glVertex3f(m_right, m_top, 0);
			glVertex3f(m_right, m_bottom, 0);
			glVertex3f(m_left, m_bottom, 0);
		glEnd();
		glDisable(GL_BLEND);

		m_pModalDialog->frameRender();
	}
}

void FrameWindow::onRender()
{
	switch (m_style) {
		case MULTIPLE_SOLID:
			renderBackground();
			break;
		case MULTIPLE_SOLID_OWNDRAW:
			{
				// render the window background
				renderBackground();

				// render the window borders
				renderBorders();
				break;
			}
		case MULTIPLE_TRANSPARENT:
			break;
		case SINGLE:
			renderBackground();
			break;
		case SINGLE_OWNDRAW:
			{
				// render the window background
				renderBackground();

				// render the window borders
				renderBorders();
				break;
			}
	}
}


void FrameWindow::onMouseDownEx(int x, int y)
{
}

void FrameWindow::onMouseMoveEx(int x, int y, int prevx, int prevy)
{
}

void FrameWindow::onMouseUpEx(int x, int y)
{
}

void FrameWindow::onMouseDown(int x, int y, int button)
{
	if (m_pModalDialog)
	{
		m_pModalDialog->onMouseDown(x,y, button);
		return;
	}
	Container::onMouseDown(x,y, button);
}

void FrameWindow::onMouseMove(int x, int y, int prevx, int prevy)
{
	if (m_pModalDialog)
	{
		if (m_pModalDialog->isPtInside(x,y))
			m_pModalDialog->onMouseMove(x,y,prevx,prevy);
		return;
	}
	Container::onMouseMove(x,y,prevx,prevy);
}

void FrameWindow::onMouseUp(int x, int y, int button)
{
	if (m_pModalDialog)
	{
		m_pModalDialog->onMouseUp(x,y,button);
		return;
	}
	Container::onMouseUp(x,y,button);
}

void FrameWindow::onKeyDown(int key)
{
	if (m_pModalDialog)
	{
		m_pModalDialog->onKeyDown(key);
		return;
	}
	Container::onKeyDown(key);
}

void FrameWindow::onKeyUp(int key)
{
	if (m_pModalDialog)
	{
		m_pModalDialog->onKeyUp(key);
		return;
	}
	Container::onKeyUp(key);
}

void FrameWindow::showModalDialog(Dialog *dlg)
{
	ASSERT(dlg);
	m_pModalDialog = dlg;
}

void FrameWindow::closeModalDialog()
{
	m_pModalDialog = 0;
}

void FrameWindow::renderBackground()
{
	glDisable(GL_BLEND);

	switch (m_style) {
		case MULTIPLE_SOLID:
		case MULTIPLE_SOLID_OWNDRAW:
			{
				// set the texture of the bg
				Texture *pTex = ResourceManager::inst()->getStockMap(ResourceManager::FRAME_BACKGROUND);
				pTex->set();
				glColor4f(1,1,1,1);
				glBegin(GL_QUADS);
					glTexCoord2f((double)m_left/pTex->getWidth(), (double)m_top/pTex->getHeight());
					glVertex3f(m_left, m_top, 0);
					glTexCoord2f((double)m_right/pTex->getWidth(), (double)m_top/pTex->getHeight());
					glVertex3f(m_right, m_top, 0);
					glTexCoord2f((double)m_right/pTex->getWidth(), (double)m_bottom/pTex->getHeight());
					glVertex3f(m_right, m_bottom, 0);
					glTexCoord2f((double)m_left/pTex->getWidth(), (double)m_bottom/pTex->getHeight());
					glVertex3f(m_left, m_bottom, 0);
				glEnd();
				break;
			}
		case SINGLE:
			{
				Texture *pTex = ResourceManager::inst()->getStockMap(ResourceManager::WINDOW_RES);
				pTex->set();
				glColor4f(1,1,1,1);
				double tW = 512;	// texture width;
				double tH = 512;	// texture width;
				int wtL = 0;		// window left in pixels in texture
				int wtR = 511;		// window right
				int wtT = 76;		// top
				int wtB = 504;		// bottom
				int offs = 32;	// the size of the border
				glBegin(GL_QUADS);
					glTexCoord2f((double)(wtL+offs)/tW, (double)(wtT+offs)/tH);
					glVertex3f(m_left, m_top, 0);
					glTexCoord2f((double)(wtR-offs)/tW, (double)(wtT+offs)/tH);
					glVertex3f(m_right, m_top, 0);
					glTexCoord2f((double)(wtR-offs)/tW, (double)(wtB-offs)/tH);
					glVertex3f(m_right, m_bottom, 0);
					glTexCoord2f((double)(wtL+offs)/tW, (double)(wtB-offs)/tH);
					glVertex3f(m_left, m_bottom, 0);
				glEnd();
				break;
			}
		case SINGLE_OWNDRAW:
			{
				Texture *pTex = ResourceManager::inst()->getStockMap(ResourceManager::WINDOW_RES);
				pTex->set();
				glColor4f(1,1,1,1);
				double tW = 512;	// texture width;
				double tH = 512;	// texture width;
				int wtL = 0;		// window left in pixels in texture
				int wtR = 511;		// window right
				int wtT = 76;		// top
				int wtB = 504;		// bottom
				int offs = 32;	// the size of the border
				Component::drawBorderedQuad(0, 0, getWidth(), getHeight(),
											offs, offs, getWidth()-offs, getHeight()-offs,
											wtL/tW, (wtL+offs)/tW, wtR/tW, (wtR-offs)/tW,
											wtT/tH, (wtT+offs)/tH, wtB/tH, (wtB-offs)/tH);
				break;
			}
	}
	
	// reset texture
	glDisable(GL_TEXTURE_2D);
}

void FrameWindow::renderBorders()
{
}