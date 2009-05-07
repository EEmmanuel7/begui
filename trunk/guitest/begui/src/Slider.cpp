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

#include "Slider.h"
#include "Font.h"
#include "ResourceManager.h"

using namespace begui;

Slider::Slider() : m_pCallback(0),
	m_id(-1),
	m_min(0),
	m_max(1),
	m_curValue(0),
	m_nSteps(10),
	m_bShowSteps(true),
	m_bShowValue(true),
	m_bDragging(false),
	m_pBoundValue(0),
	m_bDispPercentage(false),
	m_valuePrintFormat("%.2f"),
	m_bIsEnabled(true)
{
}

void Slider::create(int x, int y, int width, int id, void (*callback)(int id))
{
	m_pCallback = callback;
	m_id = id;

	m_left = x;
	m_right = x + width;
	m_top = y;
	m_bottom = y+13;
}

void Slider::setBounds(double min, double max, double initVal, int nSteps)
{
	if (max < min)
		max = min;
	if (initVal < min)
		initVal = min;
	if (initVal > max)
		initVal = max;

	m_min = min;
	m_max = max;
	m_curValue = initVal;
	m_nSteps = nSteps;
}

void Slider::bindValue(double *val)
{
	m_pBoundValue = val;
}

void Slider::onUpdate()
{
	if (m_pBoundValue && !m_bDragging)
		m_curValue = *m_pBoundValue;
}

void Slider::onRender()
{
	int w = getWidth();
	int h = getHeight();

	// position of the marker
	double f = 0;
	if (m_max > m_min)
		f = (m_curValue - m_min)/(m_max - m_min);
	// sanitize
	if (f > 1) f = 1;
	if (f < 0) f = 0;
	int spos = f*w;
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// render the slider
	if (!m_bIsEnabled)
		glColor4f(1,1,1, 0.5);
	else
		glColor4f(1,1,1,1);
	
	int l = (int)(w*0.3);
	int r = (int)(w*0.7);

	if (m_bIsEnabled)
		glColor3f(0.78, 0.78, 0.74);
	else
		glColor4f(0.6, 0.6, 0.6, 0.5);
	glBegin(GL_QUADS);
		glVertex3f(0, 0, 0);
		glVertex3f(l, 0, 0);
		glVertex3f(l, h, 0);
		glVertex3f(0, h, 0);
	glEnd();
	
	if (m_bIsEnabled)
		glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
		glVertex3f(l, 0, 0);
		glVertex3f(r, 0, 0);
		glVertex3f(r, h, 0);
		glVertex3f(l, h, 0);
	glEnd();
	
	if (m_bIsEnabled)
		glColor3f(0.93, 0.85, 0.79);
	glBegin(GL_QUADS);
		glVertex3f(r, 0, 0);
		glVertex3f(w, 0, 0);
		glVertex3f(w, h, 0);
		glVertex3f(r, h, 0);
	glEnd();

	glColor4f(0.3, 0.3, 0.3, 0.6);
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(w, 0, 0);
		glVertex3f(w, 0, 0);
		glVertex3f(w, h, 0);
		glVertex3f(w, h, 0);
		glVertex3f(0, h, 0);
		glVertex3f(0, h, 0);
		glVertex3f(0, 0, 0);
		
		glVertex3f(spos, h-1, 0);
		glVertex3f(spos, 1, 0);
	glEnd();

	// if using steps, draw them
	if (m_nSteps > 0 && m_bShowSteps)
	{
		glColor4f(0.3, 0.3, 0.3, 0.2);
		glBegin(GL_LINES);
		int steps = m_nSteps;
		if (steps > 50)
			steps = 50;
		for (int i=0; i<steps; ++i)
		{
			int lx = i*w/steps;
			glVertex3f(lx, 1, 0);
			glVertex3f(lx, h-2, 0);
		}
		glEnd();
	}
	
	// render the min/max values
	glColor4f(0.3, 0.3, 0.3, 0.8);
	char valStr[64];
	if (m_bDispPercentage)
	{
		sprintf(valStr, m_valuePrintFormat.c_str(), m_min*100);
		Font::renderString(5, h-3, valStr);
		sprintf(valStr, m_valuePrintFormat.c_str(), m_max*100);
		Font::renderString(w - Font::stringLength(valStr)-5, h-3, valStr);
	}
	else
	{
		sprintf(valStr, m_valuePrintFormat.c_str(), m_min);
		Font::renderString(5, h-3, valStr);
		sprintf(valStr, m_valuePrintFormat.c_str(), m_max);
		Font::renderString(w - Font::stringLength(valStr)-5, h-3, valStr);
	}
	
	// render the marker
	if (m_bIsEnabled)
	{
		Texture *pTex = ResourceManager::inst()->getStockMap(ResourceManager::STD_CONTROLS);
		pTex->set();

		int t = - 12;
		glColor4f(1,1,1,1);
		glBegin(GL_QUADS);
			glTexCoord2f(496/512.0, 1/512.0);	glVertex3f(spos-4, t, 0);
			glTexCoord2f(505/512.0, 1/512.0);	glVertex3f(spos+5, t, 0);
			glTexCoord2f(505/512.0, 19/512.0);	glVertex3f(spos+5, t+18, 0);
			glTexCoord2f(496/512.0, 19/512.0);	glVertex3f(spos-4, t+18, 0);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	glDisable(GL_BLEND);
	
	// render the current value next to the slider
	if (m_bShowValue)
	{
		glColor4f(0.3, 0.3, 0.3, 0.8);
		sprintf(valStr, "%.3f", m_curValue);
		Font::renderString(w+5, h-3, valStr);
	}
}

bool Slider::onMouseDown(int x, int y, int button)
{
	if (!m_bIsEnabled)
		return false;

	// position of the marker
	double f = (m_curValue - m_min)/(m_max - m_min);
	int spos = (1-f)*m_left + f*m_right;
	int t = m_top - 12;

	// check if the marker was clicked
	if (x > spos-4 && x<spos+5 && y>t && y<t+18)
	{
		// start dragging the marker
		m_bDragging = true;
	}
	else
	{
		// else jump to the position of the click
		if (x >= m_left && x <= m_right)
		{
			// update the slider value
			double f = (double)(x - m_left)/(m_right - m_left);
			if (m_nSteps > 0)
			{
				int nf = (int)(f*m_nSteps + 0.5);
				m_curValue = (m_nSteps-nf)*m_min/m_nSteps + nf*m_max/m_nSteps;
			}
			else
				m_curValue = (1-f)*m_min + f*m_max;

			if (m_pBoundValue)
				*m_pBoundValue = m_curValue;
			
			// after initial jump, start dragging
			m_bDragging = true;
		}
		else
			return false;
	}

	return true;
}

bool Slider::onMouseMove(int x, int y, int prevx, int prevy)
{
	if (!m_bIsEnabled)
		return false;

	if (m_bDragging)
	{
		// check if x is inside the slider
		if (x >= m_left && x <= m_right)
		{
			// check relaxed bounds for y
			if (y < m_top - 50 || y > m_bottom + 50)
				return true;

			// update the slider value
			double f = (double)(x - m_left)/(m_right - m_left);
			if (m_nSteps > 0)
			{
				int nf = (int)(f*m_nSteps + 0.5);
				m_curValue = (m_nSteps-nf)*m_min/m_nSteps + nf*m_max/m_nSteps;
			}
			else
				m_curValue = (1-f)*m_min + f*m_max;
			
			if (m_pBoundValue)
				*m_pBoundValue = m_curValue;
		}
	}

	return true;
}

bool Slider::onMouseUp(int x, int y, int button)
{
	if (!m_bIsEnabled)
		return false;

	m_bDragging = false;

	if (m_pCallback && m_bActive)
		m_pCallback(m_id);
	if (m_pBoundValue)
		*m_pBoundValue = m_curValue;

	return true;
}

void Slider::onKeyDown(int key)
{
}

void Slider::onKeyUp(int key)
{
}

bool Slider::isPtInside(int x, int y)
{
	// check marker region
	double f = (m_curValue - m_min)/(m_max - m_min);
	int spos = (1-f)*m_left + f*m_right;
	int t = m_top - 12;
	if (x > spos-4 && x<spos+5 && y>t && y<t+18)
		return true;

	// check the bar
	if (x<m_left || x>m_right)
		return false;
	if (y<m_top || y>m_bottom)
		return false;
	return true;
}
