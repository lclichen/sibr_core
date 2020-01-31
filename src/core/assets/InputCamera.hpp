
#ifndef __SIBR_ASSETS_INPUTCAMERA_HPP__
# define __SIBR_ASSETS_INPUTCAMERA_HPP__

# include "core/graphics/Config.hpp"
# include "core/graphics/Camera.hpp"
# include "core/assets/Config.hpp"

namespace sibr
{
	/** Input camera parameters. Inherits all basic camera functionality from Camera
	*  and adds functions for depth samples from multi-view stereo.
	*
	* \sa Camera, NovelCamera
	* \ingroup sibr_assets
	*/
	class SIBR_ASSETS_EXPORT InputCamera : public Camera 
	{
	public:

		/** Near/far plane representation. */
		struct Z {
			Z() {}
			float far;
			float near;
			Z(float f, float n) : far(f), near(n) {}
		};

		/** Default constructor. */
		InputCamera() :
			_focal(0.f), _k1(0.f), _k2(0.f), _w(0), _h(0), _id(0), _active(true)
		{ }

		/** Partial constructor
		* \param f focal length in mm
		* \param k1 first distortion parameter
		* \param k2 second distortion parameter
		* \param w  width of input image
		* \param h  height of input image
		* \param id ID of input image
		*/
		InputCamera(float f, float k1, float k2, int w, int h, int id);

		/** Constructor, initialize the input camera.
		* \param id ID of input image
		* \param w  width of input image
		* \param h  height of input image
		* \param position camera position
		* \param rotation camera rotation
		* \param focal focal length in mm
		* \param k1 first distortion parameter
		* \param k2 second distortion parameter
		* \param active  input image active or not
		*/
		InputCamera(int id, int w, int h, sibr::Vector3f & position, sibr::Matrix3f & rotation, float focal, float k1, float k2, bool active);

		/** Constructor, initialize the input camera.
		* \param id ID of input image
		* \param w  width of input image
		* \param h  height of input image
		* \param m  camera parameters read from Bundler output file
		* \param active  input image active or not
		* \sa Bundler: http://phototour.cs.washington.edu/bundler/
		* \warn Avoid using this legacy constructor.
		*/
		InputCamera(int id, int w, int h, sibr::Matrix4f m, bool active);

		/** Constructor from a basic Camera.
		 * \param c camera
		 * \param w image width
		 * \param h image height
		 */
		InputCamera(const Camera& c, int w, int h);

		/** Copy constructor. */
		InputCamera(const InputCamera&) = default;

		/** Move constructor. */
		InputCamera(InputCamera&&) = default;

		/** Copy operator. */
		InputCamera&	operator =(const InputCamera&) = default;

		/** Move operator. */
		InputCamera&	operator =(InputCamera&&) = default;

		/** Input image width
		* \returns width of input image
		*/
		uint w(void) const;

		/** Input image height
		* \returns height of input image
		*/
		uint h(void) const;

		/** Check if the input camera active or inactive,
		* camera is completely ignored if set to inactive.
		* \returns true if active, false otherwise
		*/
		bool isActive(void) const;

		/** Set camera active status
	     */
		void setActive(bool active) { _active = active ; }

		/** Input image name */
		inline const std::string&	name(void) const { return _name; }

		/** Set camera name */
		inline void					name( const std::string& s ) { _name = s; }

		/** Update image dimensions. Calls \a update() after changing image width and height
		* \param w image width
		* \param h image height
		*/
		void size( uint w, uint h );

		/** Returns camera id */
		uint id() const { return _id; }

		/** project into screen space */
		Vector3f projectScreen( const Vector3f& pt ) const;

		/** focal length */
		float focal() const;

		/** k1 distorsion param*/
		float k1() const;

		/** k2 distorsion param*/
		float k2() const;

		/** Back-project pixel coordinates and depth.
		* \param pixelPos pixel coordinates p[0],p[1] in [0,w-1]x[0,h-1] 
		* \param depth d in [-1,1]
		* \returns 3D point
		*/
		Vector3f			unprojectImgSpaceInvertY( const sibr::Vector2i & pixelPos, const float & depth ) const;

		/** Project 3D point using perspective projection.
		* \param point3d 3D point
		* \returns pixel coordinates in [0,w-1]x[0,h-1] and depth d in [-1,1]
		*/
		Vector3f			projectImgSpaceInvertY( const Vector3f& point3d  ) const;

		/** Load from internal binary representation.
		 * \param filename file path
		 * \return success boolean
		 */
		bool				loadFromBinary( const std::string& filename );

		/** Save to disk using internal binary representation.
		 * \param filename file path
		 */
		void				saveToBinary( const std::string& filename ) const;

		/** Save a file in the IBR TopView format.
		* \param outfile the destination file
		*/
		void				writeToFile(std::ostream& outfile) const;

		/** Load a file in the IBR TopView format.
		* \param infile the input file
		*/
		void				readFromFile(std::istream& infile);

		/** Conver to Bundle string.
		 * \param negativeZ should the Z axis be flipped
		 * \return a string that can be used to create a bundle file from this camera
		*/
		std::string toBundleString(bool negativeZ = false) const;


		/** \return vector of 4 Vector2i corresponding to the pixels at the camera corners
		*/
		std::vector<sibr::Vector2i> getImageCorners() const;


		/** Save a vector of cameras as a bundle file.
		 *\param cams the cameras
		 * \param fileName output bundle file path
		 * \param negativeZ should the Z axis be flipped
		 * \param exportImages should empty images with the proper dimensions be saved in a visualize subdirectory
		*/
		static void saveAsBundle(const std::vector<sibr::InputCamera> & cams, const std::string & fileName, bool negativeZ = false, bool exportImages = false);

		/** Load cameras from a bundler file.
		 *\param datasetPath path to the root of the dataset, should contain bundle.out, list_images.txt and optionally clipping_planes.txt 
		 * \param zNear default near-plane value to use if the clipping_planes.txt file doesn't exist
		 * \param zFar default far-plane value to use if the clipping_planes.txt file doesn't exist
		 * \param bundleName name of the bundle file
		 * \param listName name of the list images file
		 * \returns the loaded cameras
		 */
		static std::vector<InputCamera> load( const std::string& datasetPath, float zNear = 0.01f, float zFar = 1000.0f, const std::string & bundleName = "bundle.out", const std::string & listName = "list_images.txt");

		/** Load cameras from a NVM file.
		*\param nvmPath path to the NVM file
		* \param zNear default near-plane value to use
		* \param zFar default far-plane value to use.
		* \param wh will contain the sizes of each camera image
		* \returns the loaded cameras
		*/
		static std::vector<InputCamera> loadNVM(const std::string& nvmPath, float zNear = 0.01f, float zFar = 1000.0f, std::vector<sibr::Vector2u> wh = std::vector<sibr::Vector2u>());

		/** Load cameras from a .lookat file generated by our Blender plugin.
		* \param lookatPath path to the lookAt file
		* \param wh will contain the sizes of each camera image
		* \returns the loaded cameras
		*/
		static std::vector<InputCamera> loadLookat(const std::string& lookatPath, const std::vector<sibr::Vector2u>& wh= std::vector<sibr::Vector2u>());

		/** Load cameras from a Colmap txt file.
		* \param colmapSparsePath path to the Colmap sparse directory, should contains cameras.txt and images.txt
		* \param zNear default near-plane value to use
		* \param zFar default far-plane value to use.
		* \returns the loaded cameras
		* \note the camera frame is internally transformed to be consistent with fribr and RC.
		*/
		static std::vector<InputCamera> loadColmap(const std::string& colmapSparsePath,const float zNear = 0.01f, const float zFar = 1000.0f);

		/** Load cameras from a bundle file.
		* \param bundlerPath path to the bundle file.
		* \param zNear default near-plane value to use
		* \param zFar default far-plane value to use.
		* \param listImagePath path to the list_images.txt file. Will default to a file in the same directory as the bundle.out file.
		* \returns the loaded cameras
		*/
		static std::vector<InputCamera> loadBundle(const std::string& bundlerPath,  float zNear = 0.01f, float zFar = 1000.0f, const std::string & listImagePath = "");

		/** Load cameras from a bundle file.
		* \param bundlerPath path to the bundle file.
		* \param zNear default near-plane value to use
		* \param zFar default far-plane value to use.
		* \param listImagePath path to the list_images.txt file. Will default to a file in the same directory as the bundle.out file.
		* \returns the loaded cameras
		*/
		static std::vector<InputCamera> loadBundleFRIBR(const std::string& bundlerPath, float zNear = 0.01f, float zFar = 1000.0f, const std::string & listImagePath = "");

		/** Load cameras from a Meshrrom SFM cameras.sfm txt file.
		* \param meshroomSFMPath path to the Meshroom StructureFromMotion/{dd63cea98bda0e3b53ec76f17b0753b3e4dde589}/ directory, should contains cameras.sfm 
		* \param zNear default near-plane value to use
		* \param zFar default far-plane value to use.
		* \returns the loaded cameras
		* \note the camera frame is internally transformed to be consistent with fribr and RC.
		*/
		static std::vector<InputCamera> loadMeshroom(const std::string& meshroomSFMPath, const float zNear = 0.01f, const float zFar = 1000.0f);


	protected:
		/** focal length */
		float _focal;

		/** K1 bundler distorsion parameter */
		float _k1;

		/** K2 bundler dist parame*/
		float _k2;

		/** Image width */
		uint _w;

		/** Image height */
		uint _h;

		/** Input camera id */
		uint _id;

		/** Input image name */
		std::string _name;

		/** Image allowed to be used for image-based rendering */
		bool _active;
	};

} // namespace sibr

#endif // __SIBR_ASSETS_INPUTCAMERA_HPP__
