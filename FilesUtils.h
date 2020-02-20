#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

using std::vector;
using std::string;
using std::cout;
using std::endl;

namespace boost {
namespace filesystem {

    struct pathExt : public path {
        path m_pathname;

        // TODO!!!!! tmp experiments
        wchar_t dot = '.';
        wchar_t separator = '\\';
        wchar_t colon = L':';

////////////
        pathExt(path curPath) : m_pathname{ curPath } {

        }
		path pathExt::lexically_normal() const
		{
			return m_pathname;
		}
//        path pathExt::lexically_normal() const
//        {
//
//            if (m_pathname.empty())
//                return *this;
//
//            path temp;
//            iterator start(begin());
//            iterator last(end());
//            iterator stop(last--);
//            for (iterator itr(start); itr != stop; ++itr)
//            {
//                // ignore "." except at start and last
//                if (itr->native().size() == 1
//                    && (itr->native())[0] == dot
//                    && itr != start
//                    && itr != last) continue;
//
//                // ignore a name and following ".."
//                if (!temp.empty()
//                    && itr->native().size() == 2
//                    && (itr->native())[0] == dot
//                    && (itr->native())[1] == dot) // dot dot
//                {
//                    string_type lf(temp.filename().native());
//                    if (lf.size() > 0
//                        && (lf.size() != 1
//                            || (lf[0] != dot
//                                && lf[0] != separator))
//                        && (lf.size() != 2
//                            || (lf[0] != dot
//                                && lf[1] != dot
//#             ifdef BOOST_WINDOWS_API
//                                && lf[1] != colon
//#             endif
//                                )
//                            )
//                        )
//                    {
//                        temp.remove_filename();
//                        //// if not root directory, must also remove "/" if any
//                        //if (temp.native().size() > 0
//                        //  && temp.native()[temp.native().size()-1]
//                        //    == separator)
//                        //{
//                        //  string_type::size_type rds(
//                        //    root_directory_start(temp.native(), temp.native().size()));
//                        //  if (rds == string_type::npos
//                        //    || rds != temp.native().size()-1)
//                        //  {
//                        //    temp.m_pathname.erase(temp.native().size()-1);
//                        //  }
//                        //}
//
//                        iterator next(itr);
//                        if (temp.empty() && ++next != stop
//                            && next == last && *last == detail::dot_path())
//                        {
//                            temp /= detail::dot_path();
//                        }
//                        continue;
//                    }
//                }
//
//                temp /= *itr;
//            };
//
//            if (temp.empty())
//                temp /= detail::dot_path();
//            return temp;
//        }
};


}  // namespace filesystem
}  // namespace boost
class FilesUtils {
public:
	static vector<string> getFiles(const string& dir) {
		vector<string> res;
		for (const fs::path& file : fs::recursive_directory_iterator(dir)) {
			if (fs::is_regular_file(file)) {
				string sFile = (boost::filesystem::pathExt{file.string()}).lexically_normal().generic_string();
				res.push_back(sFile);
			}
		}
		return res;
	}

	static string normalizeFilePath(const string& sPath) {
		fs::path fsPath = fs::path{ sPath };
		if (fsPath.is_absolute()) {
			return fsPath.generic_string();
		}
		fs::path workdir = fs::current_path();
        return (boost::filesystem::pathExt((workdir / fsPath).string())).lexically_normal().generic_string();
	}
};
