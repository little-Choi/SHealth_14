#pragma once

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

namespace golden {

inline const char* projectRoot() {
#ifdef SHEALTH_PROJECT_ROOT
    return SHEALTH_PROJECT_ROOT;
#else
    return ".";
#endif
}

inline const char* goldenDir() {
#ifdef SHEALTH_GOLDEN_DIR
    return SHEALTH_GOLDEN_DIR;
#else
    return "src/test/golden";
#endif
}

inline std::string joinPath(const std::string& base, const std::string& leaf) {
    if (base.empty()) {
        return leaf;
    }
    const char last = base.back();
    if (last == '/' || last == '\\') {
        return base + leaf;
    }
    return base + "/" + leaf;
}

inline std::string readFile(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return {};
    }
    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

inline bool writeFile(const std::string& path, const std::string& content) {
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        return false;
    }
    out << content;
    return static_cast<bool>(out);
}

inline std::string normalizeNewlines(std::string text) {
    std::string out;
    out.reserve(text.size());
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '\r') {
            if (i + 1 < text.size() && text[i + 1] == '\n') {
                ++i;
            }
            out.push_back('\n');
        } else {
            out.push_back(text[i]);
        }
    }
    if (!out.empty() && out.back() != '\n') {
        out.push_back('\n');
    }
    return out;
}

inline std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::string line;
    std::istringstream stream(text);
    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    return lines;
}

inline bool updateGoldenEnabled() {
    const char* env = std::getenv("SHEALTH_UPDATE_GOLDEN");
    return env != nullptr && env[0] != '\0' && env[0] != '0';
}

inline std::string approvedPath(const std::string& relativeName) {
    return joinPath(goldenDir(), relativeName);
}

inline std::string actualArtifactPath(const std::string& relativeName) {
    return joinPath(joinPath(projectRoot(), "build/test_output/golden"), relativeName);
}

inline ::testing::AssertionResult filesEqual(const std::string& expectedPath,
                                             const std::string& actualPath,
                                             const std::string& expected,
                                             const std::string& actual) {
    const std::string normExpected = normalizeNewlines(expected);
    const std::string normActual = normalizeNewlines(actual);

    if (normExpected == normActual) {
        return ::testing::AssertionSuccess();
    }

    const auto expectedLines = splitLines(normExpected);
    const auto actualLines = splitLines(normActual);
    const size_t maxLines = expectedLines.size() > actualLines.size() ? expectedLines.size()
                                                                      : actualLines.size();

    std::ostringstream diff;
    diff << "Golden master mismatch\n"
         << "  approved: " << expectedPath << "\n"
         << "  actual:   " << actualPath << "\n";

    for (size_t i = 0; i < maxLines; ++i) {
        const std::string expLine = i < expectedLines.size() ? expectedLines[i] : "<missing>";
        const std::string actLine = i < actualLines.size() ? actualLines[i] : "<missing>";
        if (expLine != actLine) {
            diff << "  line " << (i + 1) << ":\n"
                 << "    - " << expLine << "\n"
                 << "    + " << actLine << "\n";
        }
    }

    return ::testing::AssertionFailure() << diff.str();
}

inline ::testing::AssertionResult assertMatchesGolden(const std::string& actual,
                                                      const std::string& relativeName) {
    const std::string approvedFile = approvedPath(relativeName);
    const std::string artifactFile = actualArtifactPath(relativeName + ".actual.txt");

    if (updateGoldenEnabled()) {
        if (!writeFile(approvedFile, actual)) {
            return ::testing::AssertionFailure()
                   << "SHEALTH_UPDATE_GOLDEN=1 but failed to write: " << approvedFile;
        }
        return ::testing::AssertionSuccess();
    }

    const std::string expected = readFile(approvedFile);
    if (expected.empty() && !actual.empty()) {
        writeFile(artifactFile, actual);
        return ::testing::AssertionFailure()
               << "Missing approved file: " << approvedFile
               << "\nRun with SHEALTH_UPDATE_GOLDEN=1 to create it.\n"
               << "Actual output written to: " << artifactFile;
    }

    const ::testing::AssertionResult result =
        filesEqual(approvedFile, artifactFile, expected, actual);
    if (!result) {
        writeFile(artifactFile, actual);
    }
    return result;
}

}  // namespace golden
