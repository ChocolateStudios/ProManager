#include "customs/customcolorfontdialog.h"

#include <QGridLayout>
#include <QPushButton>
#include <QString>
#include <QCloseEvent>

OfficeLib::Word::WdColor CustomColorFontDialog::selectedColor;

CustomColorFontDialog::CustomColorFontDialog()
{
    setWindowTitle("Select Font Color");

    QVector<QString> colorNames = {
        "Automatic",
        "Black",
        "Blue",
        "Turquoise",
        "BrightGreen",
        "Pink",
        "Red",
        "Yellow",
        "White",
        "DarkBlue",
        "Teal",
        "Green",
        "Violet",
        "DarkRed",
        "DarkYellow",
        "Brown",
        "OliveGreen",
        "DarkGreen",
        "DarkTeal",
        "Indigo",
        "Orange",
        "BlueGray",
        "LightOrange",
        "Lime",
        "SeaGreen",
        "Aqua",
        "LightBlue",
        "Gold",
        "SkyBlue",
        "Plum",
        "Rose",
        "Tan",
        "LightYellow",
        "LightGreen",
        "LightTurquoise",
        "PaleBlue",
        "Lavender",
        "Gray05",
        "Gray10",
        "Gray125",
        "Gray15",
        "Gray20",
        "Gray25",
        "Gray30",
        "Gray35",
        "Gray375",
        "Gray40",
        "Gray45",
        "Gray50",
        "Gray55",
        "Gray60",
        "Gray625",
        "Gray65",
        "Gray70",
        "Gray75",
        "Gray80",
        "Gray85",
        "Gray875",
        "Gray90",
        "Gray95"
    };

    QGridLayout* layout = new QGridLayout;

    for (int row = 0, i = 0; row < 6; row++) {
        for (int column = 0; column < 10; column++) {
            QPushButton *btn = new QPushButton(colorNames.at(i));
            i++;
            colorBtns.append(btn);
            layout->addWidget(btn, row, column);
            connect(btn, &QPushButton::clicked, this, &CustomColorFontDialog::setSelectedColor);
        }
    }
    this->setLayout(layout);
}

OfficeLib::Word::WdColor CustomColorFontDialog::getColor()
{
    CustomColorFontDialog dialog;
    dialog.exec();
    return CustomColorFontDialog::selectedColor;
}

void CustomColorFontDialog::closeEvent(QCloseEvent *event)
{
    if (!CustomColorFontDialog::selectedColor)
        CustomColorFontDialog::selectedColor = OfficeLib::Word::wdColorAutomatic;
    event->accept();
}

void CustomColorFontDialog::setSelectedColor()
{
    QVector<OfficeLib::Word::WdColor> colorFonts = {
        OfficeLib::Word::wdColorAutomatic      ,
        OfficeLib::Word::wdColorBlack          ,
        OfficeLib::Word::wdColorBlue           ,
        OfficeLib::Word::wdColorTurquoise      ,
        OfficeLib::Word::wdColorBrightGreen    ,
        OfficeLib::Word::wdColorPink           ,
        OfficeLib::Word::wdColorRed            ,
        OfficeLib::Word::wdColorYellow         ,
        OfficeLib::Word::wdColorWhite          ,
        OfficeLib::Word::wdColorDarkBlue       ,
        OfficeLib::Word::wdColorTeal           ,
        OfficeLib::Word::wdColorGreen          ,
        OfficeLib::Word::wdColorViolet         ,
        OfficeLib::Word::wdColorDarkRed        ,
        OfficeLib::Word::wdColorDarkYellow     ,
        OfficeLib::Word::wdColorBrown          ,
        OfficeLib::Word::wdColorOliveGreen     ,
        OfficeLib::Word::wdColorDarkGreen      ,
        OfficeLib::Word::wdColorDarkTeal       ,
        OfficeLib::Word::wdColorIndigo         ,
        OfficeLib::Word::wdColorOrange         ,
        OfficeLib::Word::wdColorBlueGray       ,
        OfficeLib::Word::wdColorLightOrange    ,
        OfficeLib::Word::wdColorLime           ,
        OfficeLib::Word::wdColorSeaGreen       ,
        OfficeLib::Word::wdColorAqua           ,
        OfficeLib::Word::wdColorLightBlue      ,
        OfficeLib::Word::wdColorGold           ,
        OfficeLib::Word::wdColorSkyBlue        ,
        OfficeLib::Word::wdColorPlum           ,
        OfficeLib::Word::wdColorRose           ,
        OfficeLib::Word::wdColorTan            ,
        OfficeLib::Word::wdColorLightYellow    ,
        OfficeLib::Word::wdColorLightGreen     ,
        OfficeLib::Word::wdColorLightTurquoise ,
        OfficeLib::Word::wdColorPaleBlue       ,
        OfficeLib::Word::wdColorLavender       ,
        OfficeLib::Word::wdColorGray05         ,
        OfficeLib::Word::wdColorGray10         ,
        OfficeLib::Word::wdColorGray125        ,
        OfficeLib::Word::wdColorGray15         ,
        OfficeLib::Word::wdColorGray20         ,
        OfficeLib::Word::wdColorGray25         ,
        OfficeLib::Word::wdColorGray30         ,
        OfficeLib::Word::wdColorGray35         ,
        OfficeLib::Word::wdColorGray375        ,
        OfficeLib::Word::wdColorGray40         ,
        OfficeLib::Word::wdColorGray45         ,
        OfficeLib::Word::wdColorGray50         ,
        OfficeLib::Word::wdColorGray55         ,
        OfficeLib::Word::wdColorGray60         ,
        OfficeLib::Word::wdColorGray625        ,
        OfficeLib::Word::wdColorGray65         ,
        OfficeLib::Word::wdColorGray70         ,
        OfficeLib::Word::wdColorGray75         ,
        OfficeLib::Word::wdColorGray80         ,
        OfficeLib::Word::wdColorGray85         ,
        OfficeLib::Word::wdColorGray875        ,
        OfficeLib::Word::wdColorGray90         ,
        OfficeLib::Word::wdColorGray95         ,
    };

    if (QPushButton* btn = qobject_cast<QPushButton*>(sender())) {
        CustomColorFontDialog::selectedColor = colorFonts[colorBtns.indexOf(btn)];
    }

    close();
}
