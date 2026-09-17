#include "core/meta/meta_matrix_store.h"

#include "core/cache/intel_cache.h"

#include <QSqlQuery>
#include <tuple>

namespace gemsight::core {

MetaMatrixStore::MetaMatrixStore(QObject* parent)
    : QObject(parent)
{
}

void MetaMatrixStore::attachCache(IntelCache* cache)
{
    m_cache = cache;
}

void MetaMatrixStore::ensureSeeded()
{
    if (m_seeded || !m_cache)
        return;
    seedDemoMatrices();
    m_seeded = true;
}

void MetaMatrixStore::seedDemoMatrices()
{
    QSqlDatabase db = m_cache->database();
    if (!db.isOpen())
        return;

    QSqlQuery countQ(db);
    countQ.exec(QStringLiteral("SELECT COUNT(*) FROM hero_matchup"));
    if (countQ.next() && countQ.value(0).toInt() > 0)
        return;

    QSqlQuery ins(db);
    ins.prepare(QStringLiteral(
        "INSERT INTO hero_matchup (hero_id, vs_hero_id, advantage) VALUES (?, ?, ?)"));

    const QList<std::tuple<int, int, double>> rows = {
        {110, 39, 2.4},
        {110, 53, 1.8},
        {76, 39, -1.1},
        {76, 8, 0.6},
        {16, 14, 0.3},
        {53, 110, -4.8},
        {39, 86, 3.2},
        {14, 76, -0.5},
    };

    for (const auto& row : rows) {
        ins.addBindValue(std::get<0>(row));
        ins.addBindValue(std::get<1>(row));
        ins.addBindValue(std::get<2>(row));
        ins.exec();
    }
}

double MetaMatrixStore::matchupAdvantage(int ourHeroId, const QVector<int>& enemyHeroIds) const
{
    if (!m_cache || ourHeroId <= 0 || enemyHeroIds.isEmpty())
        return 0.0;

    QSqlDatabase db = m_cache->database();
    if (!db.isOpen())
        return 0.0;

    double sum = 0.0;
    int count = 0;
    QSqlQuery q(db);
    q.prepare(QStringLiteral("SELECT advantage FROM hero_matchup WHERE hero_id = ? AND vs_hero_id = ?"));

    for (int enemyId : enemyHeroIds) {
        if (enemyId <= 0)
            continue;
        q.addBindValue(ourHeroId);
        q.addBindValue(enemyId);
        if (q.exec() && q.next()) {
            sum += q.value(0).toDouble();
            ++count;
        }
    }

    return count > 0 ? sum / count : 0.0;
}

} // namespace gemsight::core
