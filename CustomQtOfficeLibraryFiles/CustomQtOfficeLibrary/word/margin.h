#ifndef MARGIN_H
#define MARGIN_H

namespace OfficeLib { namespace Word {

struct Margin {
    double left, right, top, bottom;

    Margin(double left, double right, double top, double bottom)
        : left(left), right(right), top(top), bottom(bottom)
    {
    }

    Margin(){}
};

}}

#endif // MARGIN_H
