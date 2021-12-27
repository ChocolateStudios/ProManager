#include "textrange/textrange.h"

#include <QDataStream>

QDataStream &operator>>(QDataStream& in, TextRange& textRange) {
    return in >> textRange.start >> textRange.lenght;
}

QDataStream &operator<<(QDataStream& out, TextRange textRange) {
    return out << textRange.start << textRange.lenght;
}
