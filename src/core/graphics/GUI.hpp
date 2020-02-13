
#ifndef __SIBR_GRAPHICS_GUI_HPP__
# define __SIBR_GRAPHICS_GUI_HPP__

# include "core/graphics/Config.hpp"
# include "core/system/Vector.hpp"
# include "core/graphics/Image.hpp"
# include "core/graphics/RenderTarget.hpp"
# include "core/graphics/Window.hpp"



namespace sibr
{
	/**	Show the content of a rendertarget in an ImGui window.
	\param windowTitle the window name (unique)
	\param rt the rendrtarget to display
	\param flags ImGui flags
	\param will contain the window extent on screen
	\param invalidTexture ignore the RT
	\param updateLayout force update the camera location on screen
	\return true if window is focused (useful for managing interactions).
	\ingroup sibr_graphics
	*/
	SIBR_GRAPHICS_EXPORT bool		showImGuiWindow(const std::string& windowTitle, const IRenderTarget& rt, ImGuiWindowFlags flags, Viewport & viewport, const bool invalidTexture, const bool updateLayout);

	/**
	Helper that compute the location and extent to display an image in a given region without cropping or distorting it
	\param imgSize the image size
	\param regionSize the region size
	\param offset will containg the top-left corner location
	\param size will contain the size to use
	\ingroup sibr_graphics
	*/
	SIBR_GRAPHICS_EXPORT void 		fitImageToDisplayRegion(const Vector2f & imgSize, const Vector2i & regionSize, Vector2f& offset, Vector2f& size);
	
	/** Generate a mesh for a given label, using ImGui internally. This will generate a mesh that contain both the background and foreground geometry packed together sequentially.
	\param text the text to generate the label of
	\param separationIndex will contain the location of the first triangle of the foreground mesh
	\return the mesh containing first the background triangles then the foreground triangles
	\ingroup sibr_graphics
	*/
	SIBR_GRAPHICS_EXPORT Mesh::Ptr generateMeshForText(const std::string & text, unsigned int & separationIndex);

} // namespace sibr

namespace ImGui {

	/** \return the height of the title bar (for layout)
	\ingroup sibr_graphics
	*/
	SIBR_GRAPHICS_EXPORT const float GetTitleBarHeight();
	
	/**
	* Push a width for item which is HiDPI aware.
	* \param item_width The with to push, in regular pixels.
	* \ingroup sibr_graphics
	*/
	SIBR_GRAPHICS_EXPORT void PushScaledItemWidth(float item_width);

	/** Helper to create a tab button item.
	\param label the button text
	\param highlight should the button be highlit
	\param size the size of the button (0,0 will autosize).
	\return true if the tab is active
	* \ingroup sibr_graphics
	*/
	SIBR_GRAPHICS_EXPORT bool TabButton(const char* label, bool highlight, const ImVec2& size = ImVec2(0, 0));
}

namespace sibr {

	/** Display an image using ImGui::image, with additional options.
	\param texture the ID of the texture to display
	\param displaySize the target size
	\param uv0 bottom-left corner of the image to display
	\param uv1 top-right corner of the image to display
	*/
	SIBR_GRAPHICS_EXPORT void DisplayImageGui(
		GLuint texture,
		const sibr::Vector2i & displaySize,
		const sibr::Vector2f& uv0 = { 0, 0 },
		const sibr::Vector2f& uv1 = { 1, 1 }
	);

	/** Store user interaction information to be returned when displaying an image. */
	struct SIBR_GRAPHICS_EXPORT CallBackData {
		sibr::Vector2f positionRatio; ///< Position ratio.
		sibr::Vector2f itemPos, itemSize, mousePos; ///< Mouse information.
		float scroll = 0.0f; ///< Scroll amount.
		bool isHoovered = false, isClickedRight = false, isClickedLeft = false,
			isReleasedRight = false, isReleasedLeft = false, ctrlPressed = false; ///< Is the image currently: hovered, right-clicked pressed, left-click pressed , right-click released, left-click released, is the ctrl key pressed.
	};

	/** Display an image using ImGui::image with support for returning interaction information (did the user click the image, etc.).
	\param texture the ID of the texture to display
	\param displaySize the target size
	\param callbackDataOut will contain interaction information for the current frame 
	\param uv0 bottom-left corner of the image to display
	\param uv1 top-right corner of the image to display
	*/
	SIBR_GRAPHICS_EXPORT void ImageWithCallback(
		GLuint texture,
		const sibr::Vector2i & displaySize,
		CallBackData & callbackDataOut,
		const sibr::Vector2f& uv0 = { 0, 0 },
		const sibr::Vector2f& uv1 = { 1, 1 }
	);

	/** Store additional user zoom information to be returned when displaying an image. */
	struct SIBR_GRAPHICS_EXPORT ZoomData {

		/** \return the zoomed region top left corner in image space. */
		sibr::Vector2f topLeft()		const;

		/** \return the zoomed region bottom right corner in image space. */
		sibr::Vector2f bottomRight()	const;

		/** Convert pixel coordinates in UV, taking the zoom into account.
		\param pos the pixel position
		\return the corresponding UVs
		*/
		sibr::Vector2f uvFromBoxPos(const sibr::Vector2f& pos)	const;

		/** Rescale zoom data using a reference region size.
		\param size the region size
		\return the resized data
		*/
		ZoomData scaled(const sibr::Vector2f& size) const; 

		sibr::Vector2f center = sibr::Vector2f(0.5f, 0.5f), diagonal = sibr::Vector2f(0.5f, 0.5f),
			tmpTopLeft, tmpBottonRight, firstClickPixel, secondClickPixel; ///< Zoomed region corners and location.
		bool underMofidication = false; ///< Is the user currently zooming. 
	};

	/** Store user interaction and zooming information to be returned when displaying an image. */
	struct SIBR_GRAPHICS_EXPORT ZoomInterraction {
		/** update zoom information.
		\param canvasSize size ot the displayed region of the image
		*/
		void updateZoom(const sibr::Vector2f& canvasSize);

		CallBackData callBackData; ///< Interaction data.
		ZoomData zoomData; ///< Zoom data.
	};

	/** Display an image using ImGui::image with support for returning user zoom information.
	\param texture the ID of the texture to display
	\param displaySize the target size
	\param zoom will contain zoom information for the current frame
	*/
	SIBR_GRAPHICS_EXPORT void ImageWithZoom(
		GLuint texture,
		const sibr::Vector2i & displaySize,
		ZoomInterraction & zoom
	);

	/** \todo Document */
	struct SIBR_GRAPHICS_EXPORT SegmentSelection {
		void update(const CallBackData& callback, const sibr::Vector2i& size, const ZoomData& zoom = {});
		void computeRasterizedLine();

		sibr::Vector2i firstPosScreen, secondPosScreen, firstPosIm, secondPosIm;
		std::vector<sibr::Vector2i> rasterizedLine;
		bool first = false, valid = false;
	};
}

/** Convert an ImGui vector to a sibr vector.
\param v the vector to convert
\return the corresponding sibr vector.
*/
template<typename T> sibr::Vector<T, 2> toSIBR(const ImVec2 & v) {
	return sibr::Vector<T, 2>(v.x, v.y);
}

#endif // __SIBR_GRAPHICS_GUI_HPP__
