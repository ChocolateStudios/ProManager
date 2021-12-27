#ifndef BASEEDITOR_H
#define BASEEDITOR_H

#include <QString>

class BaseEditor
{
public:
    BaseEditor();

    void updateEditorName(const QString& name);
    const QString& getEditorName();

    virtual void insertColumnRightInTable() = 0;
    virtual void insertColumnLeftInTable() = 0;
    virtual void insertRowDownInTable() = 0;
    virtual void insertRowUpInTable() = 0;
    virtual void removeCurrentColumnInTable() = 0;
    virtual void removeCurrentRowInTable() = 0;

private:
    QString editorName;
};

#endif // BASEEDITOR_H
