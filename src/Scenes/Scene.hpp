/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scene.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:38:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/30 11:29:10 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_HPP
# define SCENE_HPP

# include "libs.hpp"
# include "Camera.hpp"
# include "InterfaceManager.hpp"

class	Scene
{
	public:
		Scene()
		{
			_interfaceManager = new InterfaceManager();
		}
		virtual ~Scene()
		{
			delete _interfaceManager;
		}
		void	open()
		{
			if (_onOpen)
				_onOpen(this);
		}
		void	close()
		{
			if (_onClose)
				_onClose(this);
		}
		void	render()
		{
			if (_onRender)
				_onRender(this);
		}
		void	update()
		{
			if (_onUpdate)
				_onUpdate(this);
		}
		void	keyHook(int key, int action)
		{
			if (_keyHook)
				_keyHook(this, key, action);
		}
		void	charHook(uint key)
		{
			if (_charHook)
				_charHook(this, key);
		}
		void	moveMouseHook(double xpos, double ypos)
		{
			if (_moveMouseHook)
				_moveMouseHook(this, xpos, ypos);
		}
		void	mouseBtnHookFunc(int button, int action, int mod)
		{
			if (_mouseBtnHookFunc)
				_mouseBtnHookFunc(this, button, action, mod);
		}
		void	mouseScrollHook(double xoffset, double yoffset)
		{
			if (_mouseScrollHookFunc)
				_mouseScrollHookFunc(this, xoffset, yoffset);
		}
		void	setOpen(std::function<void(Scene*)> openFunc) {this->_onOpen = openFunc;}
		void	setClose(std::function<void(Scene*)> closeFunc) {this->_onClose= closeFunc;}
		void	setRender(std::function<void(Scene*)> renderFunc) {this->_onRender= renderFunc;}
		void	setUpdate(std::function<void(Scene*)> updateFunc) {this->_onUpdate = updateFunc;}

		void	setKeyHook(std::function<void(Scene*, int, int)> keyHook) {this->_keyHook = keyHook;}
		void	setCharHook(std::function<void(Scene*, uint)> charHook) {this->_charHook = charHook;}
		void	setMoveMouseHook(std::function<void(Scene*, double, double)> moveMouseHook) {this->_moveMouseHook = moveMouseHook;}
		void	setMouseBtnHookFunc(std::function<void(Scene*, int, int, int)> mouseBtnHookFunc) {this->_mouseBtnHookFunc = mouseBtnHookFunc;}
		void	setMouseScrollHookFunc(std::function<void(Scene*, double, double)> mouseScrollFunc) {this->_mouseScrollHookFunc = mouseScrollFunc;}

		InterfaceManager	*getInterfaceManager() {return (this->_interfaceManager);}
	private:
		std::function<void(Scene*)>					_onRender = NULL;
		std::function<void(Scene*)>					_onUpdate = NULL;
		std::function<void(Scene*)>					_onOpen = NULL;
		std::function<void(Scene*)>					_onClose = NULL;
		std::function<void(Scene*, int, int)>		_keyHook = NULL;
		std::function<void(Scene*, uint)>			_charHook = NULL;
		std::function<void(Scene*, double, double)>	_moveMouseHook = NULL;
		std::function<void(Scene*, int, int, int)>	_mouseBtnHookFunc = NULL;
		std::function<void(Scene*, double, double)>	_mouseScrollHookFunc = NULL;
		InterfaceManager							*_interfaceManager = NULL;
};

#endif
