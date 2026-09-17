#include "core/heroes/hero_catalog.h"

#include <QHash>

namespace gemsight::core {

namespace {

struct HeroMeta {
    const char* internalName;
    const char* displayName;
};

const HeroMeta* metaForId(int heroId)
{
    static const QHash<int, HeroMeta> kTable = {
        {1, {"antimage", "Anti-Mage"}},
        {2, {"axe", "Axe"}},
        {3, {"bane", "Bane"}},
        {4, {"bloodseeker", "Bloodseeker"}},
        {5, {"crystal_maiden", "Crystal Maiden"}},
        {6, {"drow_ranger", "Drow Ranger"}},
        {8, {"juggernaut", "Juggernaut"}},
        {14, {"pudge", "Pudge"}},
        {16, {"sand_king", "Sand King"}},
        {26, {"lion", "Lion"}},
        {39, {"queenofpain", "Queen of Pain"}},
        {53, {"furion", "Nature's Prophet"}},
        {76, {"obsidian_destroyer", "Outworld Destroyer"}},
        {86, {"rubick", "Rubick"}},
        {110, {"phoenix", "Phoenix"}},
    };
    const auto it = kTable.constFind(heroId);
    if (it == kTable.end())
        return nullptr;
    return &it.value();
}

} // namespace

QString HeroCatalog::internalName(int heroId)
{
    const HeroMeta* meta = metaForId(heroId);
    if (!meta)
        return QStringLiteral("pudge");
    return QString::fromUtf8(meta->internalName);
}

QString HeroCatalog::displayName(int heroId)
{
    const HeroMeta* meta = metaForId(heroId);
    if (!meta)
        return QStringLiteral("hero_%1").arg(heroId);
    return QString::fromUtf8(meta->displayName);
}

QString HeroCatalog::portraitUrlByInternalName(const QString& internalName)
{
    const QString slug = internalName.trimmed().toLower();
    if (slug.isEmpty())
        return {};
    return QStringLiteral("https://cdn.cloudflare.steamstatic.com/apps/dota2/images/dota_react/heroes/%1.png")
        .arg(slug);
}

QString HeroCatalog::portraitUrl(int heroId)
{
    return portraitUrlByInternalName(internalName(heroId));
}

} // namespace gemsight::core
