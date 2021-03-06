#include "Engine/Core/StringUtils.hpp"

#include "Engine/Core/Rgba.hpp"
#include "Engine/Core/Win.hpp"

#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Matrix4.hpp"

#include "Engine/System/Cpu.hpp"

#include <cstdarg>
#include <cwctype>

#include <algorithm>
#include <locale>
#include <numeric>
#include <sstream>

const int STRINGF_STACK_LOCAL_TEMP_LENGTH = 2048;

namespace StringUtils {

std::string to_string(const Vector2& v) {
    std::ostringstream ss;
    ss << '[' << v.x << ',' << v.y << ']';
    return ss.str();
}

std::string to_string(const Vector3& v) {
    std::ostringstream ss;
    ss << '[' << v.x << ',' << v.y << ',' << v.z << ']';
    return ss.str();
}

std::string to_string(const Vector4& v) {
    std::ostringstream ss;
    ss << '[' << v.x << ',' << v.y << ',' << v.z << ',' << v.w << ']';
    return ss.str();
}

std::string to_string(const Matrix4& m) {
    std::ostringstream ss;
    const auto& x = m.GetXComponents();
    const auto& y = m.GetYComponents();
    const auto& z = m.GetZComponents();
    const auto& w = m.GetWComponents();
    ss << '[';
    ss << x.x << ',' << x.y << ',' << x.z << ',' << x.w;
    ss << y.x << ',' << y.y << ',' << y.z << ',' << y.w;
    ss << z.x << ',' << z.y << ',' << z.z << ',' << z.w;
    ss << w.x << ',' << w.y << ',' << w.z << ',' << w.w;
    ss << ']';
    return ss.str();
}

std::string to_string(const Rgba& clr) {
    std::ostringstream ss;
    ss << std::hex << clr;
    return ss.str();
}

std::string to_string(const System::Cpu::ProcessorArchitecture& architecture) {
    using namespace System::Cpu;
    switch(architecture) {
    case ProcessorArchitecture::Unknown:
        return "Unknown";
    case ProcessorArchitecture::x64: //Also Amd64
        return "x64";
    case ProcessorArchitecture::Arm:
        return "ARM";
    case ProcessorArchitecture::Arm64:
        return "ARM 64";
    case ProcessorArchitecture::Ia64:
        return "Intel Itanium 64";
    case ProcessorArchitecture::x86: //Also Intel
        return "x86";
    case ProcessorArchitecture::Mips:
        return "Mips";
    case ProcessorArchitecture::Alpha:
        return "Alpha";
    case ProcessorArchitecture::Ppc:
        return "PPC";
    case ProcessorArchitecture::Shx:
        return "SHX";
    case ProcessorArchitecture::Alpha64:
        return "Alpha 64";
    case ProcessorArchitecture::Msil:
        return "MSIL";
    case ProcessorArchitecture::Ia32OnWin64:
        return "Intel Itanium on Win64";
    case ProcessorArchitecture::Neutral:
        return "Neutral";
    case ProcessorArchitecture::Arm32OnWin64:
        return "ARM32 on Win64";
    case ProcessorArchitecture::Ia32OnArm64:
        return "Intel Itanium on ARM64";
    default:
        return "";
    }
}

const std::string Stringf(const char* format, ...) {
    char textLiteral[STRINGF_STACK_LOCAL_TEMP_LENGTH];
    va_list variableArgumentList;
    va_start(variableArgumentList, format);
    vsnprintf_s(textLiteral, STRINGF_STACK_LOCAL_TEMP_LENGTH, _TRUNCATE, format, variableArgumentList);
    va_end(variableArgumentList);
    textLiteral[STRINGF_STACK_LOCAL_TEMP_LENGTH - 1] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

    return std::string(textLiteral);
}

const std::string Stringf(const int maxLength, const char* format, ...) {
    char textLiteralSmall[STRINGF_STACK_LOCAL_TEMP_LENGTH];
    char* textLiteral = textLiteralSmall;
    if(maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH)
        textLiteral = new char[maxLength];

    va_list variableArgumentList;
    va_start(variableArgumentList, format);
    vsnprintf_s(textLiteral, maxLength, _TRUNCATE, format, variableArgumentList);
    va_end(variableArgumentList);
    textLiteral[maxLength - 1] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

    std::string returnValue(textLiteral);
    if(maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH)
        delete[] textLiteral;

    return returnValue;
}

std::vector<std::string> Split(const std::string& string, char delim /*= ','*/, bool skip_empty /*= true*/) {
    std::size_t potential_count = 1 + std::count(string.begin(), string.end(), delim);
    std::vector<std::string> result;
    result.reserve(potential_count);
    std::string curString;

    std::stringstream ss;
    ss.str(string);
    ss.seekg(0);
    ss.seekp(0);
    ss.clear();

    while(std::getline(ss, curString, delim)) {
        if(skip_empty && curString.empty()) continue;
        result.push_back(curString);
    }
    result.shrink_to_fit();
    return result;
}


std::vector<std::wstring> Split(const std::wstring& string, wchar_t delim /*= ','*/, bool skip_empty /*= true*/) {
    std::size_t potential_count = 1 + std::count(string.begin(), string.end(), delim);
    std::vector<std::wstring> result;
    result.reserve(potential_count);
    std::wstring curString;

    std::wstringstream ss;
    ss.str(string);
    ss.seekg(0);
    ss.seekp(0);
    ss.clear();

    while(std::getline(ss, curString, delim)) {
        if(skip_empty && curString.empty()) continue;
        result.push_back(curString);
    }
    result.shrink_to_fit();
    return result;

}

std::pair<std::string, std::string> SplitOnFirst(const std::string& string, char delim) {
    auto eq_loc = string.find_first_of(delim);
    return std::make_pair(string.substr(0, eq_loc), string.substr(eq_loc + 1));
}

std::pair<std::wstring, std::wstring> SplitOnFirst(const std::wstring& string, wchar_t delim) {
    auto eq_loc = string.find_first_of(delim);
    return std::make_pair(string.substr(0, eq_loc), string.substr(eq_loc + 1));
}

std::pair<std::string, std::string> SplitOnLast(const std::string& string, char delim) {
    auto eq_loc = string.find_last_of(delim);
    return std::make_pair(string.substr(0, eq_loc), string.substr(eq_loc + 1));
}


std::pair<std::wstring, std::wstring> SplitOnLast(const std::wstring& string, wchar_t delim) {
    auto eq_loc = string.find_last_of(delim);
    return std::make_pair(string.substr(0, eq_loc), string.substr(eq_loc + 1));
}

std::string Join(const std::vector<std::string>& strings, char delim, bool skip_empty /*= true*/) {
    auto acc_op = [](const std::size_t& a, const std::string& b)->std::size_t { return a + static_cast<std::size_t>(1u) + b.size(); };
    auto total_size = std::accumulate(std::begin(strings), std::end(strings), static_cast<std::size_t>(0u), acc_op);
    std::string result{};
    result.reserve(total_size);
    for(auto iter = strings.begin(); iter != strings.end(); ++iter) {
        if(skip_empty && (*iter).empty()) continue;
        result += (*iter);
        if(iter != strings.end() - 1) {
            result.push_back(delim);
        }
    }
    result.shrink_to_fit();
    return result;
}

std::string Join(const std::vector<std::string>& strings, bool skip_empty /*= true*/) {
    auto acc_op = [](const std::size_t& a, const std::string& b) { return a + b.size(); };
    std::size_t total_size = std::accumulate(std::begin(strings), std::end(strings), static_cast<std::size_t>(0u), acc_op);
    std::string result;
    result.reserve(total_size);
    for(const auto& string : strings) {
        if(skip_empty && string.empty()) continue;
        result += string;
    }
    result.shrink_to_fit();
    return result;
}


std::wstring Join(const std::vector<std::wstring>& strings, wchar_t delim, bool skip_empty /*= true*/) {
    auto acc_op = [](const std::size_t& a, const std::wstring& b)->std::size_t { return a + static_cast<std::size_t>(1u) + b.size(); };
    auto total_size = std::accumulate(std::begin(strings), std::end(strings), static_cast<std::size_t>(0u), acc_op);
    std::wstring result{};
    result.reserve(total_size);
    for(auto iter = strings.begin(); iter != strings.end(); ++iter) {
        if(skip_empty && (*iter).empty()) continue;
        result += (*iter);
        if(iter != strings.end() - 1) {
            result.push_back(delim);
        }
    }
    result.shrink_to_fit();
    return result;
}


std::wstring Join(const std::vector<std::wstring>& strings, bool skip_empty /*= true*/) {
    auto acc_op = [](const std::size_t& a, const std::wstring& b) { return a + b.size(); };
    std::size_t total_size = std::accumulate(std::begin(strings), std::end(strings), static_cast<std::size_t>(0u), acc_op);
    std::wstring result;
    result.reserve(total_size);
    for(const auto& string : strings) {
        if(skip_empty && string.empty()) continue;
        result += string;
    }
    result.shrink_to_fit();
    return result;
}

std::string ToUpperCase(std::string string) {
    std::transform(string.begin(), string.end(), string.begin(), [](unsigned char c) -> unsigned char { return std::toupper(c, std::locale("")); });
    return string;
}

std::wstring ToUpperCase(std::wstring string) {
    std::transform(string.begin(), string.end(), string.begin(), [](wchar_t c) -> wchar_t { return std::towupper(static_cast<std::wint_t>(c)); });
    return string;
}

std::string ToLowerCase(std::string string) {
    std::transform(string.begin(), string.end(), string.begin(), [](unsigned char c) -> unsigned char { return std::tolower(c, std::locale("")); });
    return string;
}


std::wstring ToLowerCase(std::wstring string) {
    std::transform(string.begin(), string.end(), string.begin(), [](wchar_t c) -> wchar_t { return std::towlower(static_cast<std::wint_t>(c)); });
    return string;
}

std::string ConvertUnicodeToMultiByte(const std::wstring& unicode_string) {
    char* buf = nullptr;
    auto buf_size = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, unicode_string.data(), -1, buf, 0, nullptr, nullptr);
    std::string mb_string;
    buf = new char[buf_size * sizeof(char)];
    buf_size = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, unicode_string.data(), -1, buf, buf_size, nullptr, nullptr);
    mb_string.assign(buf, buf_size - 1);
    delete[] buf;
    buf = nullptr;
    return mb_string;
}

std::wstring ConvertMultiByteToUnicode(const std::string& multi_byte_string) {
    wchar_t* buf = nullptr;
    auto buf_size = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, multi_byte_string.data(), -1, buf, 0);
    std::wstring unicode_string;
    buf = new wchar_t[buf_size * sizeof(wchar_t)];
    buf_size = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, multi_byte_string.data(), -1, buf, buf_size);
    unicode_string.assign(buf, buf_size - 1);
    delete[] buf;
    buf = nullptr;
    return unicode_string;
}

bool StartsWith(const std::string& string, const std::string& start) {
    auto found_loc = string.find(start);
    return found_loc != std::string::npos && found_loc == 0;
}


bool StartsWith(const std::wstring& string, const std::wstring& start) {
    auto found_loc = string.find(start);
    return found_loc != std::wstring::npos && found_loc == 0;
}

bool EndsWith(const std::string& string, const std::string& end) {
    auto found_loc = string.rfind(end);
    return found_loc != std::string::npos && found_loc == string.size() - end.size();
}


bool EndsWith(const std::wstring& string, const std::wstring& end) {
    auto found_loc = string.rfind(end);
    return found_loc != std::wstring::npos && found_loc == string.size() - end.size();
}

//From https://stackoverflow.com/a/3418285/421178
std::string ReplaceAll(std::string string, const std::string& from, const std::string& to) {
    if(from.empty()) {
        return string;
    }
    std::size_t start_pos = 0;
    while((start_pos = string.find(from, start_pos)) != std::string::npos) {
        string.replace(start_pos, from.size(), to);
        start_pos += to.size(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
    return string;
}


std::wstring ReplaceAll(std::wstring string, const std::wstring& from, const std::wstring& to) {
    if(from.empty()) {
        return string;
    }
    std::size_t start_pos = 0;
    while((start_pos = string.find(from, start_pos)) != std::wstring::npos) {
        string.replace(start_pos, from.size(), to);
        start_pos += to.size(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
    return string;
}

std::vector<std::size_t> FindAll(std::string string, const char c) {
    std::vector<std::size_t> results{};
    std::size_t offset = 0;
    while((offset = string.find(c, offset)) != std::string::npos) {
        results.push_back(offset++);
    }
    return results;
}


std::vector<std::size_t> FindAll(std::string string, const std::string& sequence) {
    std::vector<std::size_t> results{};
    std::size_t offset = 0;
    while((offset = string.find(sequence, offset)) != std::string::npos) {
        results.push_back(offset);
    }
    return results;
}

std::vector<std::size_t> FindAll(std::wstring string, const wchar_t c) {
    std::vector<std::size_t> results{};
    std::size_t offset = 0;
    while((offset = string.find(c, offset)) != std::wstring::npos) {
        results.push_back(offset++);
    }
    return results;
}

std::vector<std::size_t> FindAll(std::wstring string, const std::wstring& sequence) {
    std::vector<std::size_t> results{};
    std::size_t offset = 0;
    while((offset = string.find(sequence, offset)) != std::wstring::npos) {
        results.push_back(offset);
    }
    return results;
}

std::string TrimWhitespace(std::string string) {
    auto first_non_space = string.find_first_not_of(" \r\n\t\v\f");
    auto last_non_space = string.find_last_not_of(" \r\n\t\v\f");
    return string.substr(first_non_space, last_non_space - first_non_space + 1);
}


std::wstring TrimWhitespace(std::wstring string) {
    auto first_non_space = string.find_first_not_of(L" \r\n\t\v\f");
    auto last_non_space = string.find_last_not_of(L" \r\n\t\v\f");
    return string.substr(first_non_space, last_non_space - first_non_space + 1);
}

namespace Encryption {

std::string ROT13(std::string text) {
    return CaesarShift<13>(text);
}

} //End Encryption

} //End StringUtils