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

namespace begui {

class Menu : public Component
{
private:
	std::vector<Menu*>	m_menuItems;
	Menu				*m_pParent;
	bool				m_itemOpen;
	int					m_activeItem;
	int					m_isMainMenu;

	std::string	m_title;
	int			m_id;
	void		(*m_pCallback)(int);
	bool		m_bSeparator;	// this menu item is a seperator
	bool		m_bChecked;		// a check mark is displayed next to the menu item

	int	m_left, m_right;
	int m_top, m_bottom;

	int m_contentWidth, m_contentHeight;

public:
	Menu();
	virtual ~Menu();

	void createMainMenu();
	void close();
	void clear();

	virtual void frameUpdate();
	virtual void frameRender();

	virtual void onMouseDown(int x, int y, int button);
	virtual void onMouseMove(int x, int y, int prevx, int prevy);
	virtual void onMouseUp(int x, int y, int button);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);
	virtual bool isPtInside(int x, int y);
			bool isPtInsideSubmenu(int x, int y);
	virtual void onDeactivate();

	Menu*	addMenuItem(const std::string &title, int id, void (*callback)(int), bool isSeparator = false);
	Menu*	getMenuItem(const std::string &title);
	Menu*	getMenuItem(int id);
	void	removeMenuItem(Menu*);

	void	setChecked(bool bChecked)	{ m_bChecked = bChecked; }
	bool	isChecked() const			{ return m_bChecked; }
	void	setTitle(const std::string& title);
};

};