#include "HelpDialog.h"
#include "ui_HelpDialog.h"

#include "QFileDialog"
#include "QFileInfo"
#include "QtPrintSupport/QPrinter"
#include "QtPrintSupport/QPrintDialog"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    initUi();
    initConnections();
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::initUi()
{
    static QString info("<html><head><meta http-equiv=Content-Type : content=text/html; charset=utf-8 />"
                        "<basefont size=7>"
                        "<title>About Program</title></head>"
                        "<body>"
                        "<p style=\"margin-left: 10px\"><a name=Content id=Content></a></p>"
                        "<h1 style=\"margin-left: 10px\"> Planning 1.0 </h1>"
                        "<h1 style=\"margin-left: 10px\"></h1>"
                        "<div style =\"margin-left: 10px\"><br>"
                        "<big><a href=#About Program>About Program</a></big><br>"
                        "<big><a href=#Data Format>Data Format</a></big><br>"
                        "<big><a href=#Created By>Created By</a></big><br>"
                        "<a name=Overview id=Overview></a></div>"

                        "<a name=About Program id=About program></a></div>"
                        "<h3 style=\"margin-left: 10px\">About program</h3><div style=\"margin-left: 10px\">"
                        "  This program generates optimal shcedule<br>"
                        "for imported tasks knowing name, duration<br>"
                        "and depended task names for each task.<br>"
                        "<a href=#Content>Content</a> <a name=About_Program id=About_Program></a><br><br></div>"

                        "<a name=Data Format id=Data Format></a></div>"
                        "<h3 style=\"margin-left: 10px\">Data Format</h3><div style=\"margin-left: 10px\">"
                        "User must import .csv format file.File <br>"
                        "columns describe task name(string), task duration<br>"
                        "(integer value) and depended tasks(stringlist).<br>"
                        "  The last must be separated with commas ',' <br>"
                        "and present in task column.<br>"
                        "See on example.<br>"

                        "<style>table {border-collapse: collapse;}"
                        "th, td {border: 1 px solid orange;padding: 10px;text-align: left;}</style>"

                        "<table border = 1 width = 99%>"
                        "<caption><b>Imported Data Example</b></caption>"
                        "<tr><th>Task Name</th><th>Duration</th> <th>Depended Tasks</th></tr>"
                        "<tr><td>I001</td><td>12</td><td>I002,I003</td></tr> "
                        "<tr><td>I002</td><td>7</td><td>I003</td></tr>"
                        "<tr><td>I003</td><td>25</td><td></td></tr></table>"

                        "<a href=#Content>Content</a> <a name=Data_Format id=Data_Format></a><br><br></div>"

                        "<a name=Created By id=Created By></a><h3 style=\"margin-left: 10px\";>"
                        "Created By</h3><div style=\"margin-left: 10px\";>"
                        "   Created by <i><u><strong>Mher Hakobyan.</strong></u></i><br> "
                        "<a href=#Content>Content</a> <a name=Created_By id=Created_By></a><br><br></div>"
                        "</body></html>"
                        );
    ui->textBrowser->setHtml(info);
    setLayout(ui->verticalLayout);
}

void HelpDialog::initConnections()
{
    connect(ui->pbOK, &QPushButton::clicked, this, &HelpDialog::accept);
}
