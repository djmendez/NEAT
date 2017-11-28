/*
 * gfxMgr.h
 *
 *  Created on: Feb 20, 2012
 *      Author: sushil
 */

#ifndef GFXMGR_H_
#define GFXMGR_H_


namespace OgreGFX {
	class GraphicsInteractionManager;

	class GFXMgr {
	public:
		GraphicsInteractionManager *gfx;
		GFXMgr(GraphicsInteractionManager *gim) {
				gfx = gim;
		}
		virtual void initialize(){};
	};
}

#endif /* GFXMGR_H_ */
