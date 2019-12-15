#ifndef MOVEMKV_HXX
#define MOVEMKV_HXX

#include <map>
#include <filesystem>

namespace fs = std::filesystem;

static const char PATH_SEP =
#ifdef _WIN32
    '\\';
#else
    '/';
#endif // _WIN32

static const int THREAD_MAX = 8;

enum class Month {
    Jan = 1,
    Feb,
    Mar,
    Apr,
    May,
    Jun,
    Jul,
    Aug,
    Sep,
    Oct,
    Nov,
    Dec
};

static std::map<const Month, const char*> MONTH_STR = {
    {Month::Jan, "01_January"},
    {Month::Feb, "02_February"},
    {Month::Mar, "03_March"},
    {Month::Apr, "04_April"},
    {Month::May, "05_May"},
    {Month::Jun, "06_June"},
    {Month::Jul, "07_July"},
    {Month::Aug, "08_August"},
    {Month::Sep, "09_September"},
    {Month::Oct, "10_October"},
    {Month::Nov, "11_November"},
    {Month::Dec, "12_December"}
};

static const char OPTS[] = ":i:o:";

// For use with threads.
struct InOutPair {
    fs::path    inPath;
    fs::path    outPath;
    bool        success;
};

/**
 * Uses the name of the MKV file to determine file tree.
 * @param filePath original path of MKV file.
 * @return path to append to the output directory.
*/
static fs::path determineLoc(fs::path& filePath);


static void relocate( void* pathPair);


#endif // MOVEMKV_HXX