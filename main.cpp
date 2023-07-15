/*
    v- There is a probleme when I create a new file when there is file
    already opened.
    v- When I add a file many time it appear mane time in recent file.
    v- Devlop copy, cut, paste, undo and redo
    v- Bring back the recent file widget when it is closed
    v- With the open window without selecting file an error occur
    v- When I click to a recent file it gotta be opened
    Check for save before closing the app or changing file
*/

#include "texteditor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TextEditor textEditor;
    textEditor.show();

    return a.exec();
}
