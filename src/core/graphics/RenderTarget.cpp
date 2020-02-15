
#include "core/graphics/RenderTarget.hpp"

namespace sibr
{
	void			blit(const IRenderTarget& src, const IRenderTarget& dst, GLbitfield mask, GLenum filter)
	{
		glBlitNamedFramebuffer(
			src.fbo(), dst.fbo(),
			0, 0, src.w(), src.h(),
			0, 0, dst.w(), dst.h(),
			mask, filter);
	}

	void			blit_and_flip(const IRenderTarget& src, const IRenderTarget& dst, GLbitfield mask, GLenum filter)
	{
		glBlitNamedFramebuffer(
			src.fbo(), dst.fbo(),
			0, 0, src.w(), src.h(),
			0, dst.h(), dst.w(), 0,
			mask, filter);
	}

	
} // namespace sibr
