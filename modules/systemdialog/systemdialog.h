#ifndef __HEADER_SYSTEMDIALOG__
#define __HEADER_SYSTEMDIALOG__

#include <string>
#include <vector>

namespace systemdialog
{

	void showMessageBox(const std::string & message);

	struct FileOpenParams
	{
		std::string initialDirectory;
		bool multipleSelection;
		std::vector<std::string> filters;
	};

	bool showFileDialog(const FileOpenParams & params, std::vector<std::string> & files);

} // namespace systemdialog

#endif // __HEADER_SYSTEMDIALOG__

