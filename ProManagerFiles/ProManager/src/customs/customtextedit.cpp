#include "customs/customtextedit.h"
#include "word/document.h"
#include "style.h"

#include <QtWidgets>

#include <QAxObject>
CustomTextEdit::CustomTextEdit()
{
    setAcceptRichText(false);
}

void subFunction(const QColor& col, QTextCursor& cur);
const Style* getStyle(const QColor& col, const QList<Style>& styles);

void CustomTextEdit::writeToWord(OfficeLib::Word::Document& wordDocument, const QList<Style>& styles)
{
//    wordDocument.writeText(toPlainText());
    QTextCursor cur(textCursor());
    cur.movePosition(QTextCursor::End);
    int finalPos = cur.position();
    cur.movePosition(QTextCursor::Start);

    QColor col;
    do {
        if (cur.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor)) {
            col = cur.charFormat().background().color();

            QChar t = cur.selectedText().at(0);
            if (t == QChar(8233)) {       //Check for enter character (code 8233)
                wordDocument.writeText("\n");
            }

            const Style* curStyle = getStyle(col, styles);
            if (curStyle) {
                subFunction(col, cur);

                QString text = cur.selectedText();
                text.replace(QChar(8233), "\n");

                QFont font = curStyle->getFont();

                //Font Properties
                OfficeLib::Word::WdColor fontColor = curStyle->getFontColor();
                wordDocument.setFontColor(fontColor);
                wordDocument.setFontBold(font.bold());
                wordDocument.setFontName(font.family());
                wordDocument.setFontSize(font.pointSize());

                //Paragraph Properties
                OfficeLib::Word::WdParagraphAlignment alignment = OfficeLib::Word::wdAlignParagraphJustify;
                wordDocument.setParagraphAlignment(alignment);
                wordDocument.writeText(text);
            }
        }
        cur.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
    } while (cur.position() < finalPos);
}

void subFunction(const QColor& col, QTextCursor& cur) {
    while (cur.charFormat().background().color() == col)
        if (!cur.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor))
            break;
}

const Style* getStyle(const QColor& col, const QList<Style>& styles) {
    for (const Style& s : styles) {
        if (col == s.getIconColor())
            return &s;
    }
    return nullptr;
}

void CustomTextEdit::initTest()
{
    QTextCursor cur(textCursor());
    cur.insertText("Startup Profile:\n");
    cur.insertText("A través de los años la tecnología ha ido mejorando y evolucionando "
                   "a tal grado que en la actualidad la usamos en la vida cotidiana para "
                   "facilitarnos algunas tareas y resolver problemas.\n");
    cur.insertText("Con la llegada de la pandemia mundial muchas empresas se han visto "
                   "perjudicadas. Entre ellos, nuestro equipo se ha percatado de que uno "
                   "de los sectores que más ha perdido es en el que se encuentran las empresas "
                   "cuyos empleados requieren del uso de recursos virtuales proporcionados en "
                   "sus oficinas de trabajo y de un trabajo colaborativo. Esto, debido a que "
                   "la situación impide que sus empleados puedan acudir a espacios físicos públicos "
                   "para hacer uso de dichos recursos o reunirse con compañeros de equipo para discutir "
                   "ideas, planificar su proyecto y realizar un trabajo colaborativo para llevar a "
                   "cabo su objetivo.\n\n");
    cur.insertText("Startup Description:\n");
    cur.insertText("Pollitos FC es un grupo conformado por estudiantes de la carrera de Ingeniería de "
                   "Software. Diego Alonso Olivera, Jorge Sanchez, Sebastián Sasaki, Diego Henriquez"
                   " y Andreluis Ingaroca.  Este grupo surge debido a la situación que se ha vivido "
                   "durante los años 2020 y 2021 por la pandemia mundial, la cual ha perjudicado a "
                   "muchas empresas y a sus empleados. Nuestro equipo tiene como objetivo principal "
                   "ayudar a los equipos de trabajo y personas que no disponen de los recursos necesarios "
                   "brindándoles una solución para que sigan realizando su labor desde un entorno virtual "
                   "y remoto a través de un producto de software.");
}

void CustomTextEdit::saveData(QDataStream &out)
{
    out << toPlainText();
}

void CustomTextEdit::readData(QDataStream &in)
{
    QString data;
    in >> data;
    setPlainText(data);
}

void CustomTextEdit::test()
{
//    QTextCursor cur(textCursor());
//    cur.movePosition(QTextCursor::End);
//    int finalPos = cur.position();
//    cur.movePosition(QTextCursor::Start);

//    QColor col;
//    do {
//        if (cur.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor)) {
//            QString t = cur.selectedText();
//            if (t.at(0) == QChar(8233))
//                qDebug() << "enter";
//            col = cur.charFormat().background().color();
//            Style* curStyle = getStyle(col, styles);
//            if (curStyle) {
//                subFunction(col, cur);
//                QString text = cur.selectedText();
//                qDebug() << curStyle->getColor() << ", " << curStyle->getFont() << ", " << curStyle->getName();
//                qDebug() << text;
//            }
//        }
//        cur.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
//    } while (cur.position() < finalPos);
}

void CustomTextEdit::setTextSelectedColor()
{
    if (QAction* act = qobject_cast<QAction*>(sender())) {

        QColor col = act->icon().pixmap(100, 100).toImage().pixel(0, 0);

        QTextCharFormat charFormat;
        charFormat.setBackground(col);

        QTextCursor cur(textCursor());
        cur.beginEditBlock();
        cur.setCharFormat(charFormat);
        cur.endEditBlock();
    }
}

void CustomTextEdit::resetSelectedTextFormat()
{
    QTextCharFormat charFormat;

    QTextCursor cur(textCursor());
    cur.beginEditBlock();
    cur.setCharFormat(charFormat);
    cur.endEditBlock();
}
