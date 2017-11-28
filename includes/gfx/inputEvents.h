/*
 * keyCombo.h
 *
 *  Created on: Jan 7, 2013
 *      Author: sushil
 */

#ifndef KEYCOMBO_H_
#define KEYCOMBO_H_

#include <set>

#include <OISEvents.h>

//#include <engine.h>
namespace OgreGFX {
	class KeySet {
	public:
		const std::set<OIS::KeyCode> *modifiersSet;
		OIS::KeyCode key;
		KeySet(const std::set<OIS::KeyCode> *oisModifiers, const OIS::KeyCode oisKey){
			modifiersSet = oisModifiers;
			key = oisKey;
		}
		std::string toString(){
			std::stringstream ss;
			for(std::set<OIS::KeyCode>::iterator it = modifiersSet->begin(); it != modifiersSet->end(); ++it )	{
				ss << *it;
			}
			ss << key;
			return ss.str();
		}
	};

	class MouseButtonKeySet {
	public:
		const std::set<OIS::KeyCode> *modifiersSet;
		OIS::MouseButtonID mouseButton;
		MouseButtonKeySet(const std::set<OIS::KeyCode> *oisModifiers, const OIS::MouseButtonID oisMouseButton){
			modifiersSet = oisModifiers;
			mouseButton = oisMouseButton;
		}

		std::string toString(){
			std::stringstream ss;
			for(std::set<OIS::KeyCode>::iterator it = modifiersSet->begin(); it != modifiersSet->end(); ++it )	{
				ss << *it;
			}
			ss << mouseButton;
			return ss.str();
		}

	};

}

#endif /* KEYCOMBO_H_ */
