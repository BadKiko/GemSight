#pragma once

#include <QString>

namespace gemsight::core {

class HeroCatalog {
public:
    static QString internalName(int heroId);
    static QString displayName(int heroId);
    static QString portraitUrl(int heroId);
    static QString portraitUrlByInternalName(const QString& internalName);
};

} // namespace gemsight::core
