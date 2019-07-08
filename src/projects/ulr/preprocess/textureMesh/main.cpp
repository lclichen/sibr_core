#include "core/system/CommandLineArgs.hpp"
#include "core/assets/InputCamera.hpp"
#include "core/graphics/Image.hpp"
#include "core/graphics/Mesh.hpp"
#include "core/imgproc/MeshTexturing.hpp"
#include "core/view/BasicIBRScene.hpp"

using namespace sibr;


struct TexturingAppArgs : virtual BasicIBRAppArgs {
	RequiredArg<std::string> output_path = { "output" };
	Arg<int> output_size = { "size", 8192 };
	Arg<bool> flood_fill = { "flood" };
	Arg<bool> poisson_fill = { "poisson" };
};

int main(int ac, char** av) {

	// Parse Command-line Args
	sibr::CommandLineArgs::parseMainArgs(ac, av);

	TexturingAppArgs args;

	// Display help.
	if(!args.dataset_path.isInit() || !args.output_path.isInit()) {
		std::cout << "Usage: " << std::endl;
		std::cout << "\tRequired: --path path/to/dataset --output path/to/output/file.png" << std::endl;
		std::cout << "\tOptional: --size 8192 --flip (vertical flip) --flood (flood fill) --poisson (poisson fill)" << std::endl;
		return 0;
	}

	SIBR_LOG << "[Texturing] Loading data..." << std::endl;
	BasicIBRScene scene(args, true);

	MeshTexturing texturer(args.output_size);
	texturer.setMesh(scene.proxies()->proxyPtr());
	texturer.reproject(scene.cameras()->inputCameras(), scene.images()->inputImages());

	// Export options.
	// UVs start at the bottom of the image, we have to flip.
	uint options = MeshTexturing::FLIP_VERTICAL;
	if (args.flood_fill) {
		options = options | MeshTexturing::FLOOD_FILL;
	}
	if (args.poisson_fill) {
		options = options | MeshTexturing::POISSON_FILL;
	}

	sibr::ImageRGB::Ptr result = texturer.getTexture(options);
	result->save(args.output_path);

	return 0;
}

