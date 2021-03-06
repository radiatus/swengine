#pragma once

#include <string>
#include <Engine\Components\Graphics\RenderSystem\Camera.h>

using SceneObjectId = size_t;

class SceneObject {
public:
	SceneObject();
	virtual ~SceneObject();

	void onRegister(SceneObjectId id);

	SceneObjectId getId() const;
	
	void setName(const std::string& name);
	std::string getName() const;

protected:
	std::string m_name;

private:
	SceneObjectId m_id;
};