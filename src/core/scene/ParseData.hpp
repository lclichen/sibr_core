#pragma once

#include "core/scene/IParseData.hpp"


namespace sibr{

	/**
	* Class used to store the data required for defining an IBR Scene
	* 
	*
	* Members:
	* - _basePathName: Base dataset directory path.
	* - _camInfos: Vector of sibr::InputCamera holding all data attached with the scene cameras.
	* - _meshPath: Filepath of the mesh associated to the scene.
	* - _imgInfos: Vector of sibr::ImageListFile::Infos holding filename, width, height, and id of the input images.
	* - _imgPath: Path to the calibrated images directory.
	* - _activeImages: Vector of bools storing active state of the camera.
	* - _numCameras: Number of cameras associated with the dataset
	* - _datasetType: Type if dataset being used. Currently supported: COLMAP, SIBR_BUNDLER, NVM, MESHROOM
	*
	* \ingroup sibr_scene
	*/

	class SIBR_SCENE_EXPORT ParseData: public IParseData {
		
	public:

		/**
		* \brief Pointer to the instance of class sibr::ParseData.
		*/
		typedef std::shared_ptr<ParseData>				Ptr;

		/**
		* \brief Function to parse data from a template dataset path.
		* \param dataset_path Path to the folder containing data
		* \param customPath Path to algorithm specific data
		* \param scene_metadata_filename Specify the filename of the Scene Metadata file to load specific scene
		*/
		void  getParsedBundlerData(const std::string & dataset_path, const std::string & customPath, const std::string & scene_metadata_filename) override;

		/**
		* \brief Function to parse data from a template dataset path.
		* \param dataset_path Path to the folder containing data
		* \param customPath Path to algorithm specific data
		*/
		void  getParsedMeshroomData(const std::string & dataset_path, const std::string & customPath = "") override;


		/**
		* \brief Function to parse data from a colmap sparse dataset path.
		* \param dataset_path Path to the colmap dataset sparse folder containing data
		*
		* The function takes in a colmap dataset sparse folder path and populates ParseData members with data.
		* This function can be used for direct compatibility with colmap data in SIBR.
		* The function automatically computes the intrinsic and extrinsic parameters of the camera, input images filename, widht and height etc.
		* Colmap uses LHS coordinate system while SIBR uses RHS coordinate system. The function applies appropriate transformation to handle this case.
		* 
		* For further compatibility with FrIBR, which enforces a Y-up RHS coordinate system, we need to apply an extra conversion to the rotation matrix, to 'flip back' from y-down to y-up.
		* \note Note: when applying the above mentioned conversion, the mesh needs to be converted by the same converter matrix
		* \brief Function to parse data from a colmap dataset path.
		* \param dataset_path Path to the folder containing data
		*/
		void  getParsedColmapData(const std::string & dataset_path) override;

		/**
		* \brief Function to parse data from a template dataset path.
		* \param dataset_path Path to the folder containing data
		* \param customPath Path to algorithm specific data
		* \param nvm_path Specify the filename of the NVM path.
		*/
		void  getParsedNVMData(const std::string & dataset_path, const std::string & customPath, const std::string & nvm_path) override;

		/**
		* \brief Function to parse data from a dataset path. Will automatically determine the type of dataset based on the files present.
		* \param myArgs Arguments containing the dataset path and other infos
		* \param customPath additional data path
		*/
		void  getParsedData(const BasicIBRAppArgs & myArgs, const std::string & customPath = "") override;

		/**
		* \brief Function to parse the scene metadata file to read image data.
		*
		*/
		bool									parseSceneMetadata(const std::string & scene_metadata_path) override;

	protected:
		
		/**
		* \brief Function to parse the camera calibration files to read camera properties (camera matrix etc.).
		*
		*/
		bool parseBundlerFile(const std::string & bundler_file_path);

		
		/**
		* \brief Function to populate scene info from camera infos to appropriate location.
		*
		*/
		void populateFromCamInfos();
		
	};

}