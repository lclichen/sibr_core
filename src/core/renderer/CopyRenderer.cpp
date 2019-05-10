
#include <core/renderer/CopyRenderer.hpp>

namespace sibr { 
	CopyRenderer::CopyRenderer(const std::string& vertFile, const std::string& fragFile)
	{
		_shader.init("CopyShader",
			sibr::loadFile(vertFile),
			sibr::loadFile(fragFile));
	}

	void	CopyRenderer::process( uint textureID, IRenderTarget& dst, bool disableTest )
	{
		if (disableTest)
			glDisable(GL_DEPTH_TEST);
		else
			glEnable(GL_DEPTH_TEST);

		_shader.begin();
		dst.clear();
		dst.bind();

		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, textureID );
		sibr::RenderUtility::renderScreenQuad();

		dst.unbind();
		_shader.end();
	}

	void	CopyRenderer::copyToWindow(uint textureID, Window& dst)
	{
		glDisable(GL_DEPTH_TEST);

		_shader.begin();

		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, textureID);
		sibr::RenderUtility::renderScreenQuad();

		_shader.end();
	}

} /*namespace sibr*/ 
