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
        bool hasFile = false;
        bool fileSaved = true;
        // The menu
        QMenu *file;
        QMenu *edit;
        QMenu *view;
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
        // The dock
        QDockWidget *dock;
        //--
        QAction *showRecentFileAction;
        //--
        QAction *helpAction;
        // Recent files list
        QListWidget *recentFilesList;
        bool recentFileVisible = true;
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
        void openRecentFile();
        void recentFileVisibility();
        //--
        void getHelp();
        //--
        void updateSaved();
};

#endif
