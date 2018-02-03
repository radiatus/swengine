#pragma once

#include <Engine\Components\GUI\Window.h>
#include <Engine\Components\Graphics\RenderSystem\Renderer.h>

enum class GraphicsDriver {
	OpenGL33
};

class GraphicsManager {
public:
	GraphicsManager(Window* window, GraphicsDriver driver);
	~GraphicsManager();

	Renderer* getRenderer() const;

	Material* createMaterial();
	Texture* createTexture(Texture::Type type);

	GpuProgram* createGpuProgram(const std::string& source);
	Sprite* createSprite(Texture* texture, GpuProgram* gpuProram);

	Light* createLight(LightType type);

	Framebuffer* createFramebuffer();
	HardwareBuffer* createHardwareBuffer();
private:
	Window* m_window;
	Renderer* m_renderer;
};