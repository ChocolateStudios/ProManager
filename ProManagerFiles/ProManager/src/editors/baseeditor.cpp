#include "baseeditor.h"

BaseEditor::BaseEditor()
{

}

void BaseEditor::updateEditorName(const QString &name)
{
    editorName = name;
}

const QString &BaseEditor::getEditorName()
{
    return editorName;
}
