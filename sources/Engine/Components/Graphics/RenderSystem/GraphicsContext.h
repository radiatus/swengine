#pragma once

#include <Engine\Components\GUI\Window.h>
#include <Engine\Components\Math\types.h>

class GraphicsContext {
public:
	enum class DepthFunction {
		Less, LessEqual
	};

	enum class FaceCullingMode {
		Front, Back, FrontBack
	};

public:
	GraphicsContext(Window* window, unsigned int viewportWidth, unsigned int viewportHeight);
	virtual ~GraphicsContext();

	virtual void enableDepthTest() = 0;
	virtual void disableDepthTest() = 0;

	virtual void setDepthTestFunction(DepthFunction function) = 0;

	virtual void enableFaceCulling() = 0;
	virtual void disableFaceCulling() = 0;

	virtual void setFaceCullingMode(FaceCullingMode mode) = 0;

	virtual void clear(const vector3& color) = 0;
	virtual void swapBuffers() = 0;

	unsigned int getViewportWidth() const;
	unsigned int getViewportHeight() const;

protected:
	Window* m_window;

	unsigned int m_viewportWidth;
	unsigned int m_viewportHeight;
};