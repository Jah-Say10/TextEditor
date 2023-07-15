#include "texteditor.h"
#include "ui_texteditor.h"

TextEditor::TextEditor(QWidget *parent) : QMainWindow(parent)
{
    // Style
    createStyle();

    // Actions
    createActions();

    // Menu
    createMenu();

    // Tool bar
    createToolBar();

    // Body
    createBody();

    // Dock
    createDock();

    // Signals ans slots system
    eventHandle();
}

void TextEditor::newfile()
{
    // chech weather there is a file
    if(hasFile)
    {
        textFile.close();
        mainText->setText("");
    }

    bool ok = false;
    QString newFileName = QInputDialog::getText(this, "File name", "Name:", QLineEdit::Normal, "", &ok);

    if(ok)
    {
        if(newFileName.isEmpty())
        {
            QMessageBox::warning(this, "Warnning!", "The name is empty!");
            return;
        }

        // File creation
        filePath = QFileDialog::getExistingDirectory(this);
        filePath = filePath + "/" +  newFileName;

        // Get the actual file as c++ file
        textFile.setFileName(filePath);

        // Open the file
        if(!textFile.open(QIODevice::ReadWrite))
        {
            // If there is a error
            QMessageBox::critical(this, "Warning!!", "An error occur!!!");
            return;
        }

        // Set hasFile to true
        hasFile = true;

        // Set file to saved
        fileSaved = true;

        title->setText(newFileName);
        // The tool print absolute path of the file
        title->setToolTip(filePath);

        // Enable editing
        mainText->setEnabled(true);
    }
    else
    {
        return;
    }

    // Set fileSaved to false
    fileSaved = false;

    // Add in new file list
    new QListWidgetItem(textFile.fileName(), recentFilesList);


    // Set the status bar
    statusBar()->showMessage("File created...");
}

void TextEditor::openfile()
{
    // chech weather there is a file opened
    if(hasFile)
        textFile.close();

    // Get the path of the file
    filePath = QFileDialog::getOpenFileName(this);

    // Print only the name of the file
    QString fileName = getOnlyNameFromPath(filePath);

    title->setText(fileName);
    // The tool print absolute path of the file
    title->setToolTip(filePath);
    // Get the actual file as c++ file
    textFile.setFileName(filePath);

    // Test if we press the cancel button
    if(fileName.isEmpty())
        return;

    // Open the file
    if(!textFile.open(QIODevice::ReadWrite))
    {
        // If there is a error
        QMessageBox::critical(this, "Warning!!", "An error occur!!!");
        return;
    }

    // Set hasFile to true
    hasFile = true;

    // Set file to saved
    fileSaved = true;

    // Enable editing
    mainText->setEnabled(true);

    // QTextStream is read or write in file
    QTextStream in(&textFile);
    QString text = "";

    while(!in.atEnd())
    {
        // Get all line of the file
        text += in.readLine() + "\n";
    }
    // Remove last new line
    text = text.mid(0, text.size() - 1);

    // set the resultat of our variable
    mainText->setText(text);

    // Set fileSaved to false
    fileSaved = false;

    // Add in new file list
    bool isInList = false;
    for(int i {0}; i < recentFilesList->count(); i++)
    {
        if(recentFilesList->item(i)->text() == textFile.fileName())
        {
            isInList = true;
            break;
        }
    }
    if(!isInList)
        new QListWidgetItem(textFile.fileName(), recentFilesList);

    // Set the status bar
    statusBar()->showMessage("File opened...");
}

void TextEditor::savefile()
{
    textFile.close();

    // Get the actual file as c++ file
    textFile.setFileName(filePath);
    if(!textFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
    {
        // If there is a error
        QMessageBox::critical(this, "Warning!!", "An error occur!!!");
        return;
    }
    textFile.close();

    // Get the actual file as c++ file
    textFile.setFileName(filePath);
    if(!textFile.open(QIODevice::ReadWrite))
    {
        // If there is a error
        QMessageBox::critical(this, "Warning!!", "An error occur!!!");
        return;
    }

    // QTextStream is read or write in file
    QTextStream out(&textFile);
    out << mainText->toPlainText();

    fileSaved = true;

    // Set the status bar
    statusBar()->showMessage("File saved...");
}

void TextEditor::closefile()
{
    if(hasFile)
    {
        // check weather the file is saved
        if(!fileSaved)
        {
            int answer = QMessageBox::question(this, "Warning", "Want before closing save the file ?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

            if(answer == QMessageBox::Yes)
            {
                savefile();
            }
            else if(answer == QMessageBox::No)
            {
                // Do nothing
            }
            else
            {
                // Return ot the app
                return;
            }
        }

        textFile.close();
        hasFile = false;

        mainText->setText("");
        mainText->setEnabled(false);

        title->setText("");

        // Set the status bar
        statusBar()->showMessage("File closed...");
    }
}

void TextEditor::quitApp()
{
    // check weather the file is saved
    if(!fileSaved)
    {
        int answer = QMessageBox::question(this, "Warning", "Want before quitting save the file ?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if(answer == QMessageBox::Yes)
        {
            savefile();
        }
        else if(answer == QMessageBox::No)
        {
            // Do nothing
        }
        else
        {
            // Return ot the app
            return;
        }
    }

    if(hasFile)
        textFile.close();

    qApp->quit();
}

void TextEditor::undo()
{
    mainText->undo();
    // Set the status bar
    statusBar()->showMessage("Undo...");
}

void TextEditor::redo()
{
    mainText->redo();
    // Set the status bar
    statusBar()->showMessage("Redo...");
}

void TextEditor::copy()
{
    mainText->copy();
    // Set the status bar
    statusBar()->showMessage("copied...");
}

void TextEditor::cut()
{
    mainText->cut();
    // Set the status bar
    statusBar()->showMessage("Cut...");
}

void TextEditor::paste()
{
    mainText->paste();
    // Set the status bar
    statusBar()->showMessage("Pasted...");
}

void TextEditor::openRecentFile()
{
    // chech weather there is a file opened
    if(hasFile)
        textFile.close();

    // Get the path of the file
    filePath = recentFilesList->currentItem()->text();

    // Print only the name of the file
    QString fileName = getOnlyNameFromPath(filePath);

    title->setText(fileName);
    // The tool print absolute path of the file
    title->setToolTip(filePath);
    // Get the actual file as c++ file
    textFile.setFileName(filePath);

    // Test if we press the cancel button
    if(fileName.isEmpty())
        return;

    // Open the file
    if(!textFile.open(QIODevice::ReadWrite))
    {
        // If there is a error
        QMessageBox::critical(this, "Warning!!", "An error occur!!!");
        return;
    }

    // Set hasFile to true
    hasFile = true;

    // Set file to saved
    fileSaved = true;

    // Enable editing
    mainText->setEnabled(true);

    // QTextStream is read or write in file
    QTextStream in(&textFile);
    QString text = "";

    while(!in.atEnd())
    {
        // Get all line of the file
        text += in.readLine() + "\n";
    }
    // Remove last new line
    text = text.mid(0, text.size() - 1);

    // set the resultat of our variable
    mainText->setText(text);

    // Set fileSaved to false
    fileSaved = false;

    // Add in new file list
    bool isInList = false;
    for(int i {0}; i < recentFilesList->count(); i++)
    {
        if(recentFilesList->item(i)->text() == textFile.fileName())
        {
            isInList = true;
            break;
        }
    }
    if(!isInList)
        new QListWidgetItem(textFile.fileName(), recentFilesList);

    // Set the status bar
    statusBar()->showMessage("File opened...");
}

void TextEditor::recentFileVisibility()
{
    if(recentFileVisible)
    {
        dock->hide();
        recentFileVisible = false;
        showRecentFileAction->setText("Show recents files");
    }
    else
    {
        dock->show();
        recentFileVisible = true;
        showRecentFileAction->setText("Hide recents files");
    }
}

void TextEditor::getHelp()
{
     QMessageBox::warning(this, "Warnning!", recentFilesList->currentItem()->text());
}

void TextEditor::updateSaved()
{
    fileSaved = false;
}

void TextEditor::createBody()
{
    // Minimum hiehgt of the window
    setMinimumHeight(400);

    QWidget *body = new QWidget;

    title = new QLabel;

    mainText = new QTextEdit;
    mainText->setEnabled(false);
    mainText->setMaximumWidth(800);
    mainText->setMinimumWidth(600);
    mainText->setTabStopDistance(32);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(title, 0, 0);
    layout->addWidget(mainText, 1, 0);

    body->setLayout(layout);

    setCentralWidget(body);

    // Set hasFile to true
    hasFile = false;

    // Status bar
    statusBar()->showMessage("...");
}

void TextEditor::createDock()
{
    dock = new QDockWidget("Recent Files", this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    // The list of the most recent files
    recentFilesList = new QListWidget(this);
    recentFilesList->setMaximumWidth(400);
    recentFilesList->setMinimumWidth(150);

    dock->setWidget(recentFilesList);

    QMainWindow::addDockWidget(Qt::RightDockWidgetArea ,dock);
}

void TextEditor::createMenu()
{
    file = menuBar()->addMenu("&File");
    file->addAction(newFileAction);
    file->addAction(openFileAction);
    file->addAction(saveFileAction);
    file->addAction(closeFileAction);
    file->addAction(quitAction);

    edit = menuBar()->addMenu("&Edit");
    edit->addAction(undoAction);
    edit->addAction(redoAction);
    edit->addAction(copyAction);
    edit->addAction(cutAction);
    edit->addAction(pasteAction);

    view = menuBar()->addMenu("&View");
    view->addAction(showRecentFileAction);

    help = menuBar()->addMenu("&Help");
    help->addAction(helpAction);
}

void TextEditor::createActions()
{
    newFileAction = new QAction("New file", this);
    newFileAction->setIcon(QIcon("..\\Icon\\White\\file.png"));
    newFileAction->setShortcut(QKeySequence("Ctrl+N"));

    openFileAction = new QAction("Open file", this);
    openFileAction->setIcon(QIcon("..\\Icon\\White\\open.png"));
    openFileAction->setShortcut(QKeySequence("Ctrl+O"));

    saveFileAction = new QAction("Save file", this);
    saveFileAction->setIcon(QIcon("..\\Icon\\White\\save.png"));
    saveFileAction->setShortcut(QKeySequence("Ctrl+S"));

    closeFileAction = new QAction("Close file", this);
    closeFileAction->setIcon(QIcon("..\\Icon\\White\\close.png"));
    closeFileAction->setShortcut(QKeySequence("Ctrl+W"));

    quitAction = new QAction("Quit", this);
    quitAction->setIcon(QIcon("..\\Icon\\White\\quit.png"));
    quitAction->setShortcut(QKeySequence("Ctrl+Q"));

    /*-------------------------*/

    undoAction = new QAction("Undo", this);
    undoAction->setIcon(QIcon("..\\Icon\\White\\undo.png"));
    undoAction->setShortcut(QKeySequence("Ctrl+Z"));

    redoAction = new QAction("Redo", this);
    redoAction->setIcon(QIcon("..\\Icon\\White\\redo.png"));
    redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));

    copyAction = new QAction("Copy", this);
    copyAction->setIcon(QIcon("..\\Icon\\White\\copy.png"));
    copyAction->setShortcut(QKeySequence("Ctrl+C"));

    cutAction = new QAction("Cut", this);
    cutAction->setIcon(QIcon("..\\Icon\\White\\cut.png"));
    cutAction->setShortcut(QKeySequence("Ctrl+X"));

    pasteAction = new QAction("Paste", this);
    pasteAction->setIcon(QIcon("..\\Icon\\White\\paste.png"));
    pasteAction->setShortcut(QKeySequence("Ctrl+V"));

    /*-------------------------*/

    showRecentFileAction = new QAction("Hide recents files", this);

    /*-------------------------*/

    helpAction = new QAction("Help", this);
    helpAction->setIcon(QIcon("..\\Icon\\White\\help.png"));
    helpAction->setShortcut(QKeySequence("Ctrl+H"));
}

void TextEditor::createToolBar()
{
    QToolBar *toolBar1 = addToolBar("");
    toolBar1->addAction(newFileAction);
    toolBar1->addAction(openFileAction);
    toolBar1->addAction(saveFileAction);
    toolBar1->addAction(closeFileAction);
    toolBar1->addSeparator();

    QToolBar *toolBar2 = new QToolBar("");
    addToolBar(Qt::LeftToolBarArea, toolBar2);
    toolBar2->addAction(undoAction);
    toolBar2->addAction(redoAction);
    toolBar2->addAction(copyAction);
    toolBar2->addAction(cutAction);
    toolBar2->addAction(pasteAction);
    toolBar2->addSeparator();

    toolBar2->addAction(helpAction);
}

void TextEditor::eventHandle()
{
    QWidget::connect(newFileAction, SIGNAL(triggered()), this, SLOT(newfile()));
    QWidget::connect(openFileAction, SIGNAL(triggered()), this, SLOT(openfile()));
    QWidget::connect(saveFileAction, SIGNAL(triggered()), this, SLOT(savefile()));
    QWidget::connect(closeFileAction, SIGNAL(triggered()), this, SLOT(closefile()));
    QWidget::connect(quitAction, SIGNAL(triggered()), this, SLOT(quitApp()));

    QWidget::connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));
    QWidget::connect(redoAction, SIGNAL(triggered()), this, SLOT(redo()));
    QWidget::connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));
    QWidget::connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));
    QWidget::connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    QWidget::connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    QWidget::connect(showRecentFileAction, SIGNAL(triggered()), this, SLOT(recentFileVisibility()));

    // Chande for saved
    QWidget::connect(mainText, SIGNAL(textChanged()), this, SLOT(updateSaved()));

    // For the recents files list
    QWidget::connect(recentFilesList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(openRecentFile()));
}

void TextEditor::createStyle()
{
    setWindowState(Qt::WindowMaximized);

    // Style
    setStyleSheet
    (
        "*"
        "{ background-color: rgb(40, 40, 40);"
        "color: white }"

        "QMenu"
        "{ background-color: rgb(50, 50, 50);"
        "color: rgb(230, 230, 230) }"

        "QMenu::item:selected, QMenuBar::item:selected"
        "{ background-color: rgb(20, 20, 20); }"

        "QMenuBar"
        "{ background-color: rgb(80, 80, 80); }"

        "QToolBar"
        "{ background-color: rgb(60, 60, 60); }"

        "QToolTip"
        "{ color: rgb(60, 60, 60); }"

        "QTextEdit"
        "{ background-color: rgb(25, 25, 25); "
        "color: rgb(230, 230, 230); "
        "font-size: 12pt;"
        "border: 1px solid grey; }"

        "QStatusBar"
        "{ background-color: rgb(60, 60, 60);"
        "color: rgb(230, 230, 230);"
        "border-top: 1px solid grey; }"

        "QDockWidget::title"
        "{ background-color: rgb(80, 80, 80); }"

        "QDockWidget::close-button:hover, QDockWidget::float-button:hover"
        "{ background-color: rgb(100, 100, 100); }"

        "QListWidget"
        "{ background-color: rgb(60, 60, 60); "
        "text-decoration: underline; }"

        "QListWidget::item:hover, QListWidget::item:selected"
        "{ background-color: rgb(80, 80, 80);"

        "QLabel"
        "{ background-color: rgb(80, 80, 80); }"
    );
}

void TextEditor::checkSave()
{

}

TextEditor::~TextEditor()
{
    delete mainText;
    mainText = nullptr;

    delete title;
    title = nullptr;

    //----------------

    delete file;
    file = nullptr;

    delete edit;
    edit = nullptr;

    delete help;
    help = nullptr;

    //---------------

    delete dock;
    dock = nullptr;

    //---------------

    delete newFileAction;
    newFileAction = nullptr;

    delete openFileAction;
    openFileAction = nullptr;

    delete saveFileAction;
    saveFileAction = nullptr;

    delete closeFileAction;
    closeFileAction = nullptr;

    delete quitAction;
    quitAction = nullptr;

    delete undoAction;
    undoAction = nullptr;

    delete redoAction;
    redoAction = nullptr;

    delete copyAction;
    copyAction = nullptr;

    delete cutAction;
    cutAction = nullptr;

    delete pasteAction;
    pasteAction = nullptr;

    delete helpAction;
    helpAction = nullptr;

    //---------------

    delete recentFilesList;
    recentFilesList = nullptr;
}

QString TextEditor::getOnlyNameFromPath(QString path)
{
    QString file;

    for(int i = path.size() - 1; i >= 0; i--)
    {
        if(path[i] == '/')
            break;

        file += path[i];
    }

    // Reverse the result string
    QChar tmp;
    for(int i = 0; i < file.size() / 2; i++)
    {
        tmp = file[i];
        file[i] = file[file.size() - 1 - i];
        file[file.size() - 1 - i] = tmp;
    }

    return file;
}
