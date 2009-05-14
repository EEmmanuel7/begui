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

#pragma once

#include "common.h"
#include "Component.h"
#include "ResourceManager.h"

namespace begui {

class Button : public Component
{
public:
	enum State {
		DOWN = 0,
		UP,
		INACTIVE,
		MOUSE_OVER,
		STATES_NUM
	};
	enum IconPlacement {
		FAR_LEFT,
		NEAR_LEFT,
		FAR_RIGHT,
		NEAR_RIGHT,
		TOP,
		BOTTOM,
		CUSTOM
	};

private:
	std::string		m_title;
	int				m_id;
	Functor1<int>	m_onClick;	// arg1: the id of the button
	State			m_status;
	bool			m_bAutoSzX, m_bAutoSzY;				// true to determine the button size automatically

	ResourceManager::ImageRef	m_faces[STATES_NUM];	// the faces corresponding to each state of the button
	ResourceManager::ImageRef	m_icon;
	int							m_iconSzX, m_iconSzY;

	IconPlacement	m_iconPlacement;
	Rect<int>		m_activeArea, m_resizableArea;

public:
	Button();

	void create(int x, int y, const std::string &title, int id, Functor1<int> &callback = Functor1<int>(), 
				const std::string &style = "std");
	void create(int x, int y, int w, int h, const std::string &title, int id, 
				Functor1<int> &callback = Functor1<int>(), 
				const std::string &style = "std");

	virtual void onUpdate();
	virtual void onRender();

	void	setState(State state)	{ m_status = state; }
	State	getState() const		{ return m_status; }
	void	setTitle(const std::string& title)	{ m_title = title; }
	void	setFace(State state, const ResourceManager::ImageRef &img);
	void	setIcon(const ResourceManager::ImageRef &icon, IconPlacement place = NEAR_LEFT, int x_sz=0, int y_sz=0);
	void	setResizableArea(const Rect<int> &resizable_area);
	
	Rect<int>	getActiveBorders() const	{ return Rect<int>(m_activeArea.left, m_activeArea.top, m_faces[UP].m_width-m_activeArea.right, m_faces[UP].m_height-m_activeArea.bottom); }
	Rect<int>	getActiveArea() const;

	virtual bool onMouseDown(int x, int y, int button);
	virtual bool onMouseMove(int x, int y, int prevx, int prevy);
	virtual bool onMouseUp(int x, int y, int button);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);
};

};