#include "RegexDefs.h"

const QRegularExpression Regex::optimizeLineRegex(
    R"(^optimize:\(([^():;]+(?:;[^():;]+)*)\)$)"
);
const QRegularExpression Regex::stockLineRegex(
    R"(^([^():]+):(\d+)$)"
);
const QRegularExpression Regex::processLineRegex(
    R"(^([^():;]+):\(((?:[^():;]+:\d+)(?:;(?:[^():;]+:\d+))*)\):(?:\(((?:[^():;]+:\d+)(?:;(?:[^():;]+:\d+))*)\))?:(\d+)$)"
);
