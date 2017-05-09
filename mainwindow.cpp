#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include <QtDebug>
#include <QLibrary>
//#include <QSqlDatabase>
//#include <QSqlError>
//#include <QStringList>
//#include <QtDebug>
#include <Windows.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"

QSqlDatabase db;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SetDllDirectory(TEXT("C:\\ZZZtemp\\"));
    QLibrary library("C:\\ZZZtemp\\libpq.dll");
    if (!library.load())
        qDebug() << library.errorString();
    else
        qDebug() << "library loaded";

    db = QSqlDatabase::addDatabase("QPSQL");
    qDebug() << db.lastError();

    db.setHostName("192.168.0.247");
    db.setDatabaseName("projects");
    db.setUserName( "fps_usr");
    db.setPassword( "archeiothiki1!");
    db.setPort(5432);
    bool ok = db.open();
    if (!ok) {
        QMessageBox::information(this,"Error","Cannot Open DB!");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString sqlstr = ui->plainTextEdit->document()->toPlainText();
    //QMessageBox::information(this,"Debug",sqlstr);
    QSqlQuery query(sqlstr);
    QSqlRecord rec = query.record();
    ui->tableWidget->setColumnCount(rec.count());

    QString headerstr="";
    for (int j=0; j<rec.count(); j++ ) {
        headerstr += rec.fieldName(j);
        headerstr += ";";
    }
    ui->tableWidget->setHorizontalHeaderLabels(headerstr.split(";"));

    QString linestr, itemstr;
    int tbcurrow=0;

    while (query.next()) {

        // Add a new row to table
        tbcurrow = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(tbcurrow);

        for (int j=0; j<rec.count(); j++ ) {
            QTableWidgetItem *pCell = new QTableWidgetItem(query.value(j).toString());
            if (tbcurrow % 2 == 0) {
                pCell->setBackgroundColor(Qt::green);
            }

            ui->tableWidget->setItem(tbcurrow,j,pCell);
        } // end for
    } // end while
}
