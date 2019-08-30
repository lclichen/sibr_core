
#ifndef __SIBR_SYSTEM_STRING_HPP__
# define __SIBR_SYSTEM_STRING_HPP__

# include "core/system/Config.hpp"

namespace sibr
{

	/**
	* \addtogroup sibr_system
	* @{
	*/

	/**
	* Replaces all occurences of a substring with another substring.
	* \param src the string to perform replacements in
	* \param search the substring to replace
	* \param replaceby the new substring to substitute
	* \return the string with the substitutions performed.
	*/
	SIBR_SYSTEM_EXPORT std::string strSearchAndReplace( const std::string& src, const std::string& search, const std::string& replaceby );

	/**
	* Process a string (a filename or path) to remove any extension if it exists.
	* \param str the string to remove the extension from
	* \return the string without extension
	*/
	SIBR_SYSTEM_EXPORT std::string removeExtension(const std::string& str);

	/**
	* Process a string (a path) to return the parent directory.
	* \param str the string to process
	* \return the string with the last component removed
	* \note Will return the empty string if no separator was found.
	*/
	SIBR_SYSTEM_EXPORT std::string parentDirectory(const std::string& str);

	/**
	* Check if a string only contains digits.
	* \param str the string to check
	* \return true if it only contains digits
	*/
	SIBR_SYSTEM_EXPORT bool strContainsOnlyDigits(const std::string& str);

	/** Split string into sub-strings delimited by a given character. 
	 * \param str the input string
	 * \param delim the delimiting characters
	 * \return a list of split substrings
	 */
	SIBR_SYSTEM_EXPORT std::vector<std::string>	split(const std::string& str, char delim = '\n');

	/** Wrapper around sibr::sprintf that returns a string 
	 * \param msg the string with C placeholders
	 * \param ... the values for each placeholder
	 * \return the string with the formatted values inserted
	 */
	SIBR_SYSTEM_EXPORT std::string	sprint(const char *msg, ...);

	/** Write a formatted string with inserted values to a buffer.
	 * \param buffer the destination string
	 * \param size the size of the format string
	 * \param format the string with C placeholders
	 * \param ... the values for each placeholder
	 * \return a status code similar to sprintf
	 */
	SIBR_SYSTEM_EXPORT int 		sprintf(char* buffer, size_t size, const char* format, ...);

	/*** @} */

} // namespace sibr

#endif // __SIBR_SYSTEM_STRING_HPP__
