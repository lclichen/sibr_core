#include "RenderTargetTextures.hpp"

namespace sibr {

	void RTTextureSize::initSize(uint w, uint h)
	{
		float aspect;
		if (_width == 0) { // use full resolution
			_width = w;
			_height = h;
		} else { // use constrained resolution
			
			if (w >= h) {
				aspect = float(w) / float(h);
				_height = uint(floor(float(_width) / aspect));
			}
			else {
				_height = _width;
				aspect = float(w) / float(h);
				_width = uint(floor(float(_height) * aspect));
			}
			
		}

		SIBR_LOG << "Rendering resolution: (" << _width << "," << _height << ")" << std::endl;
		_isInit = true;
	}

	bool RTTextureSize::isInit() const
	{
		return _isInit;
	}

	const std::vector<RenderTargetRGBA32F::Ptr>& RGBDInputTextures::inputImagesRT() const
	{
		return _inputRGBARenderTextures;
	}

	void RGBDInputTextures::initializeImageRenderTargets(CalibratedCameras::Ptr cams, InputImages::Ptr imgs)
	{
		SIBR_LOG << "Initializing input image RTs " << std::endl;

		if (!isInit()) {
			initSize(cams->inputCameras()[_initActiveCam].w(), cams->inputCameras()[_initActiveCam].h());
		}
		
		_inputRGBARenderTextures.resize(imgs->inputImages().size());

		GLShader textureShader;
		textureShader.init("Texture",
			loadFile(Resources::Instance()->getResourceFilePathName("texture.vp")),
			loadFile(Resources::Instance()->getResourceFilePathName("texture.fp")));
		uint interpFlag = (SIBR_SCENE_LINEAR_SAMPLING & SIBR_SCENE_LINEAR_SAMPLING) ? SIBR_GPU_LINEAR_SAMPLING : 0; // LINEAR_SAMPLING Set to default

		for (uint i = 0; i < imgs->inputImages().size(); i++) {
			if (cams->inputCameras()[i].isActive()) {
				ImageRGB img = std::move(imgs->inputImages()[i]->clone());
				img.flipH();

				std::shared_ptr<Texture2DRGB> rawInputImage(new Texture2DRGB(img, interpFlag));

				glViewport(0, 0, _width, _height);
				_inputRGBARenderTextures[i].reset(new RenderTargetRGBA32F(_width, _height, interpFlag));
				_inputRGBARenderTextures[i]->clear();
				_inputRGBARenderTextures[i]->bind();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, rawInputImage->handle());

				glDisable(GL_DEPTH_TEST);
				textureShader.begin();
				RenderUtility::renderScreenQuad();
				textureShader.end();
				_inputRGBARenderTextures[i]->unbind();
			}
		}
	}

	void RGBDInputTextures::initializeDepthRenderTargets(CalibratedCameras::Ptr cams, ProxyMesh::Ptr proxies, bool facecull)
	{
		if (!isInit()) {
			initSize(cams->inputCameras()[_initActiveCam].w(), cams->inputCameras()[_initActiveCam].h());
		}

		GLParameter size;
		GLParameter proj;

		GLShader depthShader;
		depthShader.init("Depth",
			loadFile(Resources::Instance()->getResourceFilePathName("depth.vp")),
			loadFile(Resources::Instance()->getResourceFilePathName("depth.fp")));

		proj.init(depthShader, "proj"); // [SP]: ??
		size.init(depthShader, "size"); // [SP]: ??
		for (uint i = 0; i < cams->inputCameras().size(); i++) {
			if (cams->inputCameras()[i].isActive()) {
				_inputRGBARenderTextures[i]->bind();
				glEnable(GL_DEPTH_TEST);
				glClear(GL_DEPTH_BUFFER_BIT);
				glDepthMask(GL_TRUE);
				glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);

				if (!proxies->proxy().triangles().empty())
				{

					const uint w = _inputRGBARenderTextures[i]->w();
					const uint h = _inputRGBARenderTextures[i]->h();

					depthShader.begin();
					size.set((float)w, (float)h);
					proj.set(cams->inputCameras()[i].viewproj());
					proxies->proxy().render(true, facecull);

					depthShader.end();
				}
				_inputRGBARenderTextures[i]->unbind();
			}
		}
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}

	void DepthInputTextureArray::initDepthTextureArrays(CalibratedCameras::Ptr cams, ProxyMesh::Ptr proxies, bool facecull, int flags)
	{

		if (!isInit()) {
			initSize(cams->inputCameras()[_initActiveCam].w(), cams->inputCameras()[_initActiveCam].h());
		}

		if (!proxies->hasProxy()) {
			SIBR_WRG << " Cannot init DepthTextureArrays without proxy." << std::endl;
			return;
		}

		SIBR_LOG << "Depth vertex shader location: " << Resources::Instance()->getResourceFilePathName("depthonly.vp") << std::endl;
		SIBR_LOG << "Depth fragment shader location: " << Resources::Instance()->getResourceFilePathName("depthonly.fp") << std::endl;

		GLShader depthOnlyShader;
		depthOnlyShader.init("DepthOnly",
			loadFile(Resources::Instance()->getResourceFilePathName("depthonly.vp")),
			loadFile(Resources::Instance()->getResourceFilePathName("depthonly.fp")));

		const uint interpFlag = (flags & SIBR_SCENE_LINEAR_SAMPLING) ? SIBR_GPU_LINEAR_SAMPLING : 0;

		RenderTargetLum32F depthRT(_width, _height, interpFlag);

		GLParameter proj;
		proj.init(depthOnlyShader, "proj");


		const uint numCams = (uint)cams->inputCameras().size();
		_inputDepthMapArrayPtr.reset(new Texture2DArrayLum32F(_width, _height, numCams, flags));

		for (uint i = 0; i < numCams; i++) {
			glViewport(0, 0, _width, _height);

			depthRT.bind();
			glEnable(GL_DEPTH_TEST);
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			glDepthMask(GL_TRUE);

			depthOnlyShader.begin();
			proj.set(cams->inputCameras()[i].viewproj());
			proxies->proxy().render(true, facecull);
			depthOnlyShader.end();

			depthRT.unbind();

			glCopyImageSubData(
				depthRT.handle(), GL_TEXTURE_2D, 0, 0, 0, 0,
				_inputDepthMapArrayPtr->handle(), GL_TEXTURE_2D_ARRAY, 0, 0, 0, i,
				_width, _height, 1);
			CHECK_GL_ERROR;
		}
		CHECK_GL_ERROR;
	}

	const Texture2DArrayLum32F::Ptr & DepthInputTextureArray::getInputDepthMapArrayPtr() const
	{
		return _inputDepthMapArrayPtr;
	}

	void RGBInputTextureArray::initRGBTextureArrays(InputImages::Ptr imgs, int flags)
	{
		if (!isInit()) {
			initSize(imgs->inputImages()[_initActiveCam]->w(), imgs->inputImages()[_initActiveCam]->h());
		}

		_inputRGBArrayPtr.reset(new Texture2DArrayRGB(imgs->inputImages(), _width, _height, flags));
	}

	const Texture2DArrayRGB::Ptr & RGBInputTextureArray::getInputRGBTextureArrayPtr() const
	{
		return _inputRGBArrayPtr;
	}

	void RenderTargetTextures::initializeDefaultRenderTargets(CalibratedCameras::Ptr cams, InputImages::Ptr imgs, ProxyMesh::Ptr proxies)
	{
		if (!isInit()) {
			initRenderTargetRes(cams);
			
		}
		initializeImageRenderTargets(cams, imgs);
		initializeDepthRenderTargets(cams, proxies, true);
	}

	void RenderTargetTextures::initRenderTargetRes(CalibratedCameras::Ptr cams)
	{
		// Find the first active camera and use it's reolution to init Rendertargets
		for (int i = 0; i < cams->inputCameras().size(); i++) {
			if (cams->inputCameras()[i].isActive()) {
				_initActiveCam = i;
				return;
			}
		}
		SIBR_ERR << "No cameras active! Fail to initialize RenderTarget!!" << std::endl;
	}

	void RenderTargetTextures::initRGBandDepthTextureArrays(CalibratedCameras::Ptr cams, InputImages::Ptr imgs, ProxyMesh::Ptr proxies, int textureFlags, int texture_width, bool faceCull)
	{
		_width = texture_width;
		initRGBandDepthTextureArrays(cams, imgs, proxies, textureFlags, faceCull);
	}

	void RenderTargetTextures::initRGBandDepthTextureArrays(CalibratedCameras::Ptr cams, InputImages::Ptr imgs, ProxyMesh::Ptr proxies, int textureFlags, bool faceCull)
	{
		if (!isInit()) {
			initRenderTargetRes(cams);
		}
		initRGBTextureArrays(imgs, textureFlags);
		initDepthTextureArrays(cams, proxies, faceCull);
	}
}
