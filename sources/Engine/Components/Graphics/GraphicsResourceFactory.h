#pragma once

#include "OpenGL3\OpenGL3GraphicsContext.h"
#include "OpenGL3\OpenGL3Texture.h"
#include "OpenGL3\OpenGL3GpuProgram.h"
#include "OpenGL3\OpenGL3GeometryStore.h"

#include "graphicsApiType.h"

class GraphicsResourceFactory {
public:
	GraphicsContext* createGraphicsContext(Window* window, unsigned int viewportWidth, unsigned int viewportHeight);

	GeometryStore* createGeometryStore();
	Texture* createTexture();
	GpuProgram* createGpuProgram();

public:
	static void initialize(GraphicsAPI api);
	static void destroy();

	static GraphicsResourceFactory* getInstance();

private:
	GraphicsResourceFactory(GraphicsAPI api);

private:
	static GraphicsResourceFactory* m_instance;

	GraphicsAPI m_api;
};