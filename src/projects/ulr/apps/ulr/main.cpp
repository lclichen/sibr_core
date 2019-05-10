#include <fstream>
#include <core/graphics/Window.hpp>
#include <core/view/MultiViewManager.hpp>
#include <projects/ulr/renderer/ULRView.hpp>
#include <core/view/BasicIBRScene.hpp>
#include <core/raycaster/Raycaster.hpp>
#include <core/view/SceneDebugView.hpp>

#define PROGRAM_NAME "sibr_ulr_app"
using namespace sibr;

const char* usage = ""
	"Usage: " PROGRAM_NAME " -path <dataset-path>"    	                                "\n"
	;


struct ULRAppArgs :
	virtual BasicIBRAppArgs {
	Arg<bool> whatever = { "whatever", true };
};

int main( int ac, char** av )
{
	{

		// Parse Commad-line Args
		CommandLineArgs::parseMainArgs(ac, av);
		ULRAppArgs myArgs;
		
		const bool doVSync = !myArgs.vsync;
		// rendering size
		uint rendering_width = myArgs.rendering_size.get()[0];
		uint rendering_height = myArgs.rendering_size.get()[1];
		// window size
		uint win_width = myArgs.win_width;
		uint win_height = myArgs.win_height;

		// Window setup
		sibr::Window        window(PROGRAM_NAME, sibr::Vector2i(50, 50), false, doVSync);

		// Setup IBR
		BasicIBRScene::Ptr		scene(new BasicIBRScene(myArgs));

		// check rendering size
		rendering_width = (rendering_width <= 0) ? scene->cameras()->inputCameras()[0].w() : rendering_width;
		rendering_height = (rendering_height <= 0) ? scene->cameras()->inputCameras()[0].h() : rendering_height;
		Vector2u usedResolution(rendering_width, rendering_height);

		const unsigned int sceneResWidth = usedResolution.x();
		const unsigned int sceneResHeight = usedResolution.y();

		ULRView::Ptr	ulrView(new ULRView(scene, sceneResWidth, sceneResHeight));
		ulrView->setNumBlend(50, 50);

		// Raycaster.
		std::shared_ptr<sibr::Raycaster> raycaster = std::make_shared<sibr::Raycaster>();
		raycaster->init();
		raycaster->addMesh(scene->proxies()->proxy());

		// Camera handler for main view.
		sibr::InteractiveCameraHandler::Ptr generalCamera(new InteractiveCameraHandler());
		generalCamera->setup(scene->cameras()->inputCameras(), Viewport(0, 0, (float)usedResolution.x(), (float)usedResolution.y()), raycaster);


		// Add views to mvm.
		MultiViewManager        multiViewManager(window, false);
		multiViewManager.addIBRSubView("ULR view", ulrView, usedResolution, ImGuiWindowFlags_ResizeFromAnySide);
		multiViewManager.addCameraForView("ULR view", generalCamera);

		// Top view
		const std::shared_ptr<sibr::SceneDebugView>    topView(new sibr::SceneDebugView(scene, multiViewManager.getViewport(), generalCamera, myArgs));
		multiViewManager.addSubView("Top view", topView);

		while (window.isOpened())
		{
			sibr::Input::poll();
			window.makeContextCurrent();
			if (sibr::Input::global().key().isPressed(sibr::Key::Escape))
				window.close();

			multiViewManager.onUpdate(sibr::Input::global());
			multiViewManager.onRender(window);
			window.swapBuffer();
			CHECK_GL_ERROR
		}

	}

	return EXIT_SUCCESS;
}
