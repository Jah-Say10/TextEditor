#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>
#include <QtWidgets>
#include <QClipboard>

class TextEditor : public QMainWindow
{
    Q_OBJECT

    private:
        QTextEdit *mainText;
        QLabel *title;
        QFile textFile;
        QString filePath;
        bool hasFile;
        bool fileSaved;
        // The menu
        QMenu *file;
        QMenu *edit;
        QMenu *help;
        // The actions
        QAction *newFileAction;
        QAction *openFileAction;
        QAction *saveFileAction;
        QAction *closeFileAction;
        QAction *quitAction;
        //--
        QAction *undoAction;
        QAction *redoAction;
        QAction *copyAction;
        QAction *cutAction;
        QAction *pasteAction;
        //--
        QAction *helpAction;
        // Recent files list
        QListWidget *recentFilesList;
        // Functions
        void createMenu();
        void createActions();
        void createToolBar();
        void eventHandle();
        void createBody();
        void createDock();
        void createStyle();
        void checkSave();
        // Static function
        static QString getOnlyNameFromPath(QString path);

    public:
        TextEditor(QWidget *parent = nullptr);
        ~TextEditor();

    private slots:
        void newfile();
        void openfile();
        void savefile();
        void closefile();
        void quitApp();
        //--
        void undo();
        void redo();
        void copy();
        void cut();
        void paste();
        //--
        void getHelp();
};

#endif
