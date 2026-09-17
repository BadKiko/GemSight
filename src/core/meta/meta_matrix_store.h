#pragma once

#include <QObject>
#include <QString>
#include <QVector>

namespace gemsight::core {

class IntelCache;

class MetaMatrixStore : public QObject {
    Q_OBJECT
public:
    explicit MetaMatrixStore(QObject* parent = nullptr);

    void attachCache(IntelCache* cache);
    void ensureSeeded();

    double matchupAdvantage(int ourHeroId, const QVector<int>& enemyHeroIds) const;

private:
    void seedDemoMatrices();

    IntelCache* m_cache = nullptr;
    bool m_seeded = false;
};

} // namespace gemsight::core
