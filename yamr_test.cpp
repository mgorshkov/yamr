#define BOOST_TEST_MODULE yamr_test

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <iostream>
#include <set>

#include "utils.h"
#include "yamr.h"

BOOST_AUTO_TEST_SUITE(yamr_test)

BOOST_AUTO_TEST_CASE(CheckFileSplit2Parts)
{
    boost::filesystem::path path = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
    boost::filesystem::create_directories(path);
    boost::filesystem::path filePath = path / "split-test";

    const std::string fileText =
        "email1@domain1.com\n"
        "email2@domain2.com\n"
        "email3@domain3.com\n"
        "email4@domain4.com\n";
        "email5@domain5.com\n"
        "email6@domain6.com\n"
        "email7@domain7.com\n"
        "email8@domain8.com\n"
        "email9@domain9.com\n"
        "email10@domain10.com\n"
        "email11@domain11.com\n"
        "email12@domain12.com";

    std::ofstream out(filePath.string());
    out << fileText;
    out.close();

    Offsets offsets = SplitFileAtLineBoundary(filePath.string(), 2);

    if (boost::filesystem::exists(path))
    {
        boost::filesystem::remove_all(path);
    }

    Offsets s{0, 36};
    BOOST_CHECK(offsets == s);
}

BOOST_AUTO_TEST_CASE(CheckFileSplit3Parts)
{
    boost::filesystem::path path = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
    boost::filesystem::create_directories(path);
    boost::filesystem::path filePath = path / "split-test";

    const std::string fileText =
        "email1@domain1.com\n"
        "email2@domain2.com\n"
        "email3@domain3.com\n"
        "email4@domain4.com\n";
        "email5@domain5.com\n"
        "email6@domain6.com\n"
        "email7@domain7.com\n"
        "email8@domain8.com\n"
        "email9@domain9.com\n"
        "email10@domain10.com\n"
        "email11@domain11.com\n"
        "email12@domain12.com";

    std::ofstream out(filePath.string());
    out << fileText;
    out.close();

    Offsets offsets = SplitFileAtLineBoundary(filePath.string(), 3);

    if (boost::filesystem::exists(path))
    {
        boost::filesystem::remove_all(path);
    }
    Offsets s{0, 17, 36};
    BOOST_CHECK(offsets == s);
}

BOOST_AUTO_TEST_CASE(CheckFileSplit4Parts)
{
    boost::filesystem::path path = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
    boost::filesystem::create_directories(path);
    boost::filesystem::path filePath = path / "split-test";

    const std::string fileText =
        "email1@domain1.com\n"
        "email2@domain2.com\n"
        "email3@domain3.com\n"
        "email4@domain4.com\n";
        "email5@domain5.com\n"
        "email6@domain6.com\n"
        "email7@domain7.com\n"
        "email8@domain8.com\n"
        "email9@domain9.com\n"
        "email10@domain10.com\n"
        "email11@domain11.com\n"
        "email12@domain12.com";

    std::ofstream out(filePath.string());
    out << fileText;
    out.close();

    Offsets offsets = SplitFileAtLineBoundary(filePath.string(), 4);

    if (boost::filesystem::exists(path))
    {
        boost::filesystem::remove_all(path);
    }
    Offsets s{0, 17, 36, 55};
    BOOST_CHECK(offsets == s);
}

}
