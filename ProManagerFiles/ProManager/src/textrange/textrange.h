#ifndef TEXTRANGE_H
#define TEXTRANGE_H

#include <QDataStream>

struct TextRange
{
    unsigned int start, lenght;

    TextRange(unsigned int start, unsigned int end)
        : start(start), lenght(end - start)
    {
    }
    TextRange()
        : start(0), lenght(0)
    {
    }

    inline unsigned int end() const { return start + lenght; }

    void addToLength(int count) {
        if (count < 0 && (-count) > lenght)
            lenght = 0;
        else
            lenght += count;
    }
    void addToStart(int count) {
        if (count < 0 && (-count) > start)
            start = 0;
        else
            start += count;
    }
    void setEnd(int end) {
        lenght = end - start;
    }

    friend QDataStream &operator>>(QDataStream& in, TextRange& textRange);
    friend QDataStream &operator<<(QDataStream& out, TextRange textRange);
};

#endif // TEXTRANGE_H
