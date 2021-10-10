#ifndef CUSTOMCOLORFONTDIALOG_H
#define CUSTOMCOLORFONTDIALOG_H

#include "word/document.h"

#include <QDialog>

class CustomColorFontDialog : public QDialog
{
    Q_OBJECT
public:
    CustomColorFontDialog();
    static OfficeLib::Word::WdColor getColor();

protected:
    void closeEvent(QCloseEvent* event) override;

public slots:
    void setSelectedColor();

private:
    QList<QPushButton*> colorBtns;
    static OfficeLib::Word::WdColor selectedColor;
};

#endif // CUSTOMCOLORFONTDIALOG_H
