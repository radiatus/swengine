#include "GUIBlock.h"

GUIBlock::GUIBlock()
{
}

GUIBlock::~GUIBlock()
{
}

const vector4 & GUIBlock::getColor() const
{
	return m_color;
}

void GUIBlock::setColor(const vector4 & color)
{
	m_color = color;
}

void GUIBlock::render(GeometryStore * quad, GpuProgram * program)
{
	program->setParameter("transform.localToWorld", getTransformationMatrix());

	program->setParameter("quad.color", m_color);
	program->setParameter("quad.useTexture", false);
	program->setParameter("quad.useFirstChannel", false);

	quad->drawArrays(GeometryStore::DrawType::Triangles, 0, 6);

}

void GUIBlock::update(const MousePosition & mousePosition)
{
}