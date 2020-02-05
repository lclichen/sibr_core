
#ifndef __SIBR_ASSETS_CAMERARECORDER_HPP__
# define __SIBR_ASSETS_CAMERARECORDER_HPP__

# include <vector>
# include "core/assets/Config.hpp"
# include "core/graphics/Camera.hpp"

# define SIBR_CAMERARECORDER_DEFAULTFILE "camera-record.bytes"

namespace sibr
{
	/**
	This class handles the recording and replay of a stream of cameras.
	\ingroup sibr_assets
	*/
	class SIBR_ASSETS_EXPORT CameraRecorder
	{
	public:

		/**
		Default constructor.
		*/
		CameraRecorder(void) :
			_pos(0), _recording(false), _playing(false), _saving(false), _savingPath(""), _speed(1.0f), _interp(0.0f) {
				//load();
		}
		/**
		Default destructor.
		*/
		~CameraRecorder( void ) { 
		}

		/**
		This method supports two modes: if the recorder is currently recording, 
		the camera argument will be saved into the recording; else if the recorder 
		is playing, the camera argument will be set to the current replaid camera.
		\param cam A reference to the camera to record/update.
		*/
		void	use( Camera& cam );

		/**
		Start playing the recorded camera stream from the beginning, at a rate of one step for each "use" call.
		*/
		void	playback( void );
		
		/**
		Start recording a new camera stream, at a rate of one camera saved for each "use" call.
		*/
		void	record( void );

		/**
		Start asking the renderer to save the frames, at a rate of one image saved for each "use" call.
		*/
		void	saving(std::string savePath);

		/**
		Toggle the save flag for video frames when replaying.
		\param saveVideo the new flag status
		*/
		void	savingVideo(bool saveVideo);

		/**
		Stop saving.
		*/
		void	stopSaving(void);

		/**
		Stop playing/recording.
		*/
		void	stop( void );

		/**
		Clear the current recording.
		*/
		void	reset( void );

		/**
		Load a recorded camera stream from a given file.
		\param filename Optional path to the file to load from. By default will try to 
				load SIBR_CAMERARECORDER_DEFAULTFILE from the current directory.
		\return a boolean denoting the loading success/failure.
		*/
		bool	load( const std::string& filename=SIBR_CAMERARECORDER_DEFAULTFILE );

		/**
		Save the current recording stream to a given file.
		\param filename Optional path to the file to write to. By default will try to
		write to SIBR_CAMERARECORDER_DEFAULTFILE in the current directory.
		*/
		void	save( const std::string& filename=SIBR_CAMERARECORDER_DEFAULTFILE );

		/** Load recorded path based on file extension.
		 *\param filename the file to load
		 *\param w resoltuion width
		 *\param h resolution height
		 *\return a success boolean
		 *\note w and h are needed when loading a Bundle.
		 */
		bool safeLoad(const std::string& filename, int w = 1920, int h = 1080);

		/**
		Load a recording stream saved as a bundle file (useful for path from FRIBR).
		\param filePath Path to the bundle file to write to.
		*/
		void	loadBundle(const std::string & filePath, int w = 1920, int h = 1080);

		/**
		Save the current recording stream as a bundle file.
		\param filePath Path to the bundle file to write to. 
		\param height the height in pixels of the camera. Used to compute focal length in mm as expected by bundle.
		\param step set to a value greater than 1 to save every "step" camera in the recording stream. 
		*/
		void	saveAsBundle(const std::string & filePath, const int height, const int step = 1);

		/**
		Save the current recording stream as a bundle file and a series of empty images for FRIBR compatibility.
		\param dirPath Path to the directory to export to.
		\param width the width in pixels of the camera. 
		\param height the height in pixels of the camera.
		*/
		void	saveAsFRIBRBundle(const std::string & dirPath, const int width, const int height);

		/**
		Save the current recording stream as a lookat file.
		\param filePath Path to the lookat file to write to.
		*/
		void saveAsLookAt(const std::string& filePath) const;

		/**
		\return a boolean denoting if the recorder is currently playing.
		*/
		bool isPlaying() const { return _playing; }

		/**
		\return a boolean denoting if the recorder is currently recording.
		*/
		bool isRecording() const { return _recording;  }

		/**
		\return a boolean denoting if the recorder is currently asking frames to be saved.
		*/
		bool isSaving() const { return _saving; }


		/**
		\return A reference to the current stream of recorded cameras.
		*/
		std::vector<Camera>& cams() { return _cameras;  }

		/**
		 * \return the interpolation speed
		*/
		float & speed() { return _speed; }

	private:
		uint					_pos; ///< Current camera ID for replay.
		std::vector<Camera>		_cameras; ///< List of recorded cameras.
		bool					_recording; ///< Are we currently recording.
		bool					_playing; ///< Are we currently playing.
		bool					_saving; ///< Are we saving the path as images.
		std::string				_savingPath; ///< Destination base path for saved images.
		bool					_savingVideo; ///< Are we saving the path as video.
		std::string				_savingVideoPath; ///< Destination base path for saved video.
		float					_speed; ///< Playback speed.
		float					_interp; ///< Current interpoaltion factor.
	};

	///// DEFINITIONS /////



} // namespace sibr

#endif // __SIBR_VIEW_CAMERARECORDER_HPP__
