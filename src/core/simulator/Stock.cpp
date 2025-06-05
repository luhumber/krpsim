#include "Stock.h"

Stock::Stock() = default;

Stock::Stock(const QMap<QString, int32_t>& data)
    : _data(data) {}

int32_t& Stock::operator[](const QString& key) {
    return _data[key];
}

const int32_t Stock::operator[](const QString& key) const {
    return _data.value(key, 0);
}

bool Stock::CanApplyProcess(const Stock& needs) const {
    for (auto it = needs._data.cbegin(); it != needs._data.cend(); ++it) {
        if (_data.value(it.key()) < it.value())
            return false;
    }
    return true;
}

void Stock::ApplyProcess(const Stock& needs, const Stock& gives) {
    for (auto it = needs._data.begin(); it != needs._data.end(); ++it) {
        _data[it.key()] -= it.value();
    }
    for (auto it = gives._data.begin(); it != gives._data.end(); ++it) {
        _data[it.key()] += it.value();
    }
}

int32_t Stock::value(const QString& key) const {
    return _data.value(key, 0);
}

QString Stock::toString() const {
    QStringList parts;
    for (auto it = _data.begin(); it != _data.end(); ++it)
        parts << QString("%1:%2").arg(it.key()).arg(it.value());
    return parts.join(", ");
}

const QMap<QString, int32_t>& Stock::data() const {
    return _data;
}

QString Stock::toStringSorted() const {
        QStringList parts;
        auto sortedKeys = _data.keys();
        std::sort(sortedKeys.begin(), sortedKeys.end());
        for (const QString& key : sortedKeys) {
            parts << QString("%1:%2").arg(key).arg(_data[key]);
        }
        return parts.join(";");
    }
