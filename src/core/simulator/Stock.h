#pragma once
#include <QMap>
#include <QString>
#include <QStringList>
#include <map>

class Stock {
public:
    Stock();
    Stock(const QMap<QString, int32_t>& data);

    int32_t& operator[](const QString& key);
    const int32_t operator[](const QString& key) const;

    bool CanApplyProcess(const Stock& needs) const;
    void ApplyProcess(const Stock& needs, const Stock& gives);

    int32_t value(const QString& key) const;
    QString toString() const;

    const QMap<QString, int32_t>& data() const;

    QString toStringSorted() const;

private:
    QMap<QString, int32_t> _data;
};
