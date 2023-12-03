#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlRecord>
#include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QODBC");
    QString connectString = "DRIVER={SQL Server};";
    connectString.append("SERVER=LAPTOP-6LPS8V34\\SQLEXPRESS;");   // Hostname,SQL-Server Instance
    connectString.append("DATABASE=karina;");  // Schema
    connectString.append("UID=LAPTOP-6LPS8V34\\Рушан;");           // User
    connectString.append("PWD=;");           // Pass
    connectString.append("Trusted_Connection=Yes;");
    db.setDatabaseName(connectString);

    if(db.open()) {
        qDebug("NICE");
    } else {
        qDebug("LOOS");
    }

    // подгрузка первой страницы при запуске

    OperationCosts = new QSqlTableModel(this, db);
    OperationCosts->setEditStrategy(QSqlTableModel::OnManualSubmit);
    OperationCosts-> setTable("Details");
    OperationCosts->select();
    ui->OperationCostsTable->setModel(OperationCosts);

    Personnel = new QSqlTableModel(this, db);
    Personnel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    Personnel-> setTable("ShopSection");
    Personnel->select();
    ui->PersonnelTable->setModel(Personnel);

    WorkAccounting = new QSqlTableModel(this, db);
    WorkAccounting->setEditStrategy(QSqlTableModel::OnManualSubmit);
    WorkAccounting-> setTable("ProductionProgram");
    WorkAccounting->select();
    ui->WorkAccountingTable->setModel(WorkAccounting);
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}



// Обновление всех таблиц
void MainWindow::on_refresh_tables_clicked()
{
    OperationCosts->revertAll(); // Удаляет изменения
    OperationCosts->clear();
    ui->OperationCostsTable->clearFocus();
    ui->OperationCostsTable->setModel(OperationCosts);
    OperationCosts = new QSqlTableModel(this, db);
    OperationCosts->setEditStrategy(QSqlTableModel::OnManualSubmit);
    OperationCosts-> setTable("Details");
    OperationCosts->select();
    ui->OperationCostsTable->setModel(OperationCosts);

    Personnel->revertAll();
    Personnel->clear();
    ui->PersonnelTable->clearFocus();
    ui->PersonnelTable->setModel(OperationCosts);
    Personnel = new QSqlTableModel(this, db);
    Personnel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    Personnel-> setTable("ShopSection");
    Personnel->select();
    ui->PersonnelTable->setModel(Personnel);

    WorkAccounting->revertAll();
    WorkAccounting->clear();
    ui->WorkAccountingTable->clearFocus();
    ui->WorkAccountingTable->setModel(OperationCosts);
    WorkAccounting = new QSqlTableModel(this, db);
    WorkAccounting->setEditStrategy(QSqlTableModel::OnManualSubmit);
    WorkAccounting-> setTable("ProductionProgram");
    WorkAccounting->select();
    ui->WorkAccountingTable->setModel(WorkAccounting);
}

// Сохранение всех таблиц
void MainWindow::on_save_tables_clicked()
{
    OperationCosts->submitAll(); // Сохраняет изменения
    OperationCosts->clear();
    ui->OperationCostsTable->clearFocus();
    ui->OperationCostsTable->setModel(OperationCosts);
    OperationCosts = new QSqlTableModel(this, db);
    OperationCosts->setEditStrategy(QSqlTableModel::OnManualSubmit);
    OperationCosts-> setTable("Details");
    OperationCosts->select();
    ui->OperationCostsTable->setModel(OperationCosts);

    Personnel->submitAll();
    Personnel->clear();
    ui->PersonnelTable->clearFocus();
    ui->PersonnelTable->setModel(OperationCosts);
    Personnel = new QSqlTableModel(this, db);
    Personnel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    Personnel-> setTable("ShopSection");
    Personnel->select();
    ui->PersonnelTable->setModel(Personnel);

    WorkAccounting->submitAll();
    WorkAccounting->clear();
    ui->WorkAccountingTable->clearFocus();
    ui->WorkAccountingTable->setModel(OperationCosts);
    WorkAccounting = new QSqlTableModel(this, db);
    WorkAccounting->setEditStrategy(QSqlTableModel::OnManualSubmit);
    WorkAccounting-> setTable("ProductionProgram");
    WorkAccounting->select();
    ui->WorkAccountingTable->setModel(WorkAccounting);
}



// VIEW c автоподгрузкой при выборе таба
void MainWindow::on_tabWidget_currentChanged(int index)
{

    if(index == 2) {
        QSqlQueryModel *setquery1 = new QSqlQueryModel;
        setquery1->setQuery("SELECT * FROM ShopDetailsInfo");
        QTableView *tv = new QTableView(this);
        tv->setModel(setquery1);
        ui->view->setModel(setquery1);
    }
}


// Первая задача SQL & ORM
void MainWindow::on_task_1_clicked()
{
    short int year = ui->year->text().toShort();
    if(ui->year->text() != '\0') {
        if(ui->radio_task_1_SQL->isChecked() == true) {

            QSqlQueryModel *setquery1 = new QSqlQueryModel;
            QString sql = QString("SELECT ShopSection.ShopNumber, ShopSection.SectionName, Details.DetailNumber, ProductionProgram.ReleasePlan"
                                  "FROM ShopSection"
                                  "JOIN ProductionProgram ON ShopSection.ShopNumber = ProductionProgram.ShopNumber AND ShopSection.SectionNumber = ProductionProgram.SectionNumber"
                                  "JOIN Details ON ProductionProgram.DetailNumber = Details.DetailNumber"
                                  "WHERE ProductionProgram.Year = YEAR(GETDATE()) AND ProductionProgram.Month = MONTH(GETDATE())").arg(year);
//SELECT p.ShopNumber, p.SectionNumber, p.FIO, wa.WorkDate, wa.DefectiveParts "
                             //     "FROM Personnel p "
                                //  "JOIN WorkAccounting wa ON p.WorkerNumber = wa.WorkerNumber "
                                  //"WHERE wa.DefectiveParts > 0 "
                                  //"AND YEAR(wa.WorkDate) = %1").arg(year);
            setquery1->setQuery(sql);
            QTableView *tv = new QTableView(this);
            tv->setModel(setquery1);
            ui->table_task_1->setModel(setquery1);
        } else if(ui->radio_task_1_ORM->isChecked() == true) {

            QSqlTableModel *person = new QSqlTableModel(this, db);
            person->setEditStrategy(QSqlTableModel::OnManualSubmit);
            person-> setTable("ShopSection");
            person->select();

            QSqlTableModel *account = new QSqlTableModel(this, db);
            account->setEditStrategy(QSqlTableModel::OnManualSubmit);
            account-> setTable("ProductionProgram");
            account->select();

            QSqlTableModel *det = new QSqlTableModel(this, db);
            det->setEditStrategy(QSqlTableModel::OnManualSubmit);
            det-> setTable("Details");
            det->select();

            QStandardItemModel *result = new QStandardItemModel(0, 4, this);
            result->setHeaderData(0, Qt::Horizontal, "ShopNumber");
            result->setHeaderData(1, Qt::Horizontal, "SectionNumber");
            result->setHeaderData(2, Qt::Horizontal, "DetailNumber");//фио
            result->setHeaderData(3, Qt::Horizontal, "ReleasePlan");//дата


            QDate Start_date;
            QDate End_date;
            Start_date.setDate((int)year, 1, 1);
            End_date.setDate((int)year+1, 1, 1);

            QModelIndex index;
            int index_x = 0;

            for(int i=0; i < account->rowCount(); i++){
                for(int j = 0; j < person->rowCount(); j++){
                    if(person->record(j).value("ShopNumber").toInt() == account->record(i).value("ShopNumber").toInt()){

                        QDate workDate = account->record(i).value("WorkDate").toDate();

                        if(workDate >= Start_date and workDate <= End_date ){

                            if(account->record(i).value("ReleasePlan").toInt() > 0){

                                result->appendRow(NULL);

                                index = result->index(index_x, 0);
                                result->setData(index, person->record(j).value("ShopNumber").toInt());

                                index = result->index(index_x, 1);
                                result->setData(index, person->record(j).value("SectionNumber").toInt());

                                index = result->index(index_x, 2);
                                result->setData(index, person->record(j).value("DetailNumber").toString());

                                index = result->index(index_x, 3);
                                result->setData(index, account->record(i).value("ReleasePlan").toString());



                                index_x++;
                            }
                        }
                    }
                }
            }
            ui->table_task_1->setModel(result);
        }
    }
}


// Вторая задача Оконная функция
void MainWindow::on_task_2_buttom_clicked()
{
    if(ui->year_2->text() != '\0') {

        short int qualification = ui->year_2->text().toShort();
        QSqlQueryModel *setquery1 = new QSqlQueryModel;
        QString sql = QString("SELECT * FROM ProductionProgramInfo").arg(qualification);
        setquery1->setQuery(sql);
        QTableView *tv = new QTableView(this);
        tv->setModel(setquery1);
        ui->window_func->setModel(setquery1);
    }
}

// Третья SQL
void MainWindow::on_task_3_buttom_clicked()
{
    short int piece_time = ui->plan_qt->text().toShort();
    short int defect = ui->reg_qt->text().toShort();

    if(type_task == 1){
        QSqlQueryModel *setquery1 = new QSqlQueryModel;
        QString sql =QString( "SELECT s.ShopNumber, s.SectionNumber, s.SectionName"
                              "FROM ShopSection s"
                              "JOIN Details d ON s.ShopNumber = d.ShopNumber AND s.SectionNumber = d.SectionNumber"
                              "JOIN ProductionProgram p ON d.DetailNumber = p.DetailNumber"
                              "WHERE d.Price > 100 AND p.Year = YEAR(GETDATE()) AND p.ReleasePlan > 100").arg(piece_time).arg(defect);
        setquery1->setQuery(sql);
        QTableView *tv = new QTableView(this);
        tv->setModel(setquery1);
        ui->table_task_3->setModel(setquery1);
    } else if(type_task == 2){

        QSqlTableModel *personnelModel = new QSqlTableModel(this, db);
        personnelModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        personnelModel->setTable("Details");
        personnelModel->select();

        QSqlTableModel *operationModel = new QSqlTableModel(this, db);
        operationModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        operationModel->setTable("ShopSection");
        operationModel->select();

        QSqlTableModel *workAccountingModel = new QSqlTableModel(this, db);
        workAccountingModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        workAccountingModel->setTable("ProductionProgram ");
        workAccountingModel->select();

        QStandardItemModel *resultModel = new QStandardItemModel(0, personnelModel->columnCount(), this);
        for (int col = 0; col < personnelModel->columnCount(); ++col) {
            resultModel->setHeaderData(col, Qt::Horizontal, personnelModel->headerData(col, Qt::Horizontal));
        }

        for (int personnelRow = 0; personnelRow < personnelModel->rowCount(); ++personnelRow) {
            int professionCode = personnelModel->record(personnelRow).value("ShopNumber").toInt();
            int workerQualification = personnelModel->record(personnelRow).value("SectionNumber").toInt();

            for (int operationRow = 0; operationRow < operationModel->rowCount(); ++operationRow) {
                if (operationModel->record(operationRow).value("ShopNumber").toInt() == professionCode &&
                    operationModel->record(operationRow).value("SectionNumber").toInt() == workerQualification ) {

                    for (int accountRow = 0; accountRow < workAccountingModel->rowCount(); ++accountRow) {
                        if (workAccountingModel->record(accountRow).value("ShopNumber").toInt() == personnelModel->record(personnelRow).value("ShopNumber").toInt() &&
                            workAccountingModel->record(accountRow).value("SectionNumber").toInt() == operationModel->record(operationRow).value("SectionNumber").toInt() &&
                            workAccountingModel->record(accountRow).value("DefectiveParts").toInt() > 0) {

                            QList<QStandardItem *> items;
                            for (int column = 0; column < personnelModel->columnCount(); ++column) {
                                QString value = personnelModel->record(personnelRow).value(column).toString();
                                QStandardItem *item = new QStandardItem(value);
                                items << item;
                            }
                            resultModel->appendRow(items);
                        }
                    }
                }
            }
        }

        ui->table_task_3->setModel(resultModel);

    }

}

// Процедурка
void MainWindow::on_proc_clicked()
{
    short int id_worker = ui->id_worker->text().toShort();
    QString start_date = ui->start_date->text();
    QSqlQuery query;
    QString sql = QString("DECLARE @DetailsCount INT; EXEC GetCountDetails 1, 1, @DetailsCount OUTPUT; SELECT @DetailsCount;").arg(id_worker).arg(start_date);

    query.exec(sql);
    query.first();
    QString res = query.value(0).toString();
    ui->res_proc->setText(res);
}



// Режим запроса ORM / SQL
void MainWindow::on_radioButton_ORM_clicked()
{
    type_task = 2;
}


void MainWindow::on_radioButton_SQL_clicked()
{
    type_task = 1;
}




// Динамическая таблица --> установка таблицы OperationCosts
void MainWindow::on_dynamic_OperationCosts_change_clicked()
{
    OperationCosts = new QSqlTableModel(this, db);
    OperationCosts->setEditStrategy(QSqlTableModel::OnManualSubmit);
    OperationCosts-> setTable("Details");
    OperationCosts->select();
    ui->dynamic_table->setModel(OperationCosts);
    ui->dynamic_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->label_2->setText("Details");
}

// Динамическая таблица --> установка таблицы Personnel
void MainWindow::on_dynamic_Personnel_change_clicked()
{
    Personnel = new QSqlTableModel(this, db);
    Personnel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    Personnel-> setTable("ShopSection");
    Personnel->select();
    ui->dynamic_table->setModel(Personnel);
    ui->label_2->setText("ShopSection");
}

// Динамическая таблица --> установка таблицы WorkAccounting
void MainWindow::on_dynamic_WorkAccounting_change_clicked()
{
    WorkAccounting = new QSqlTableModel(this, db);
    WorkAccounting->setEditStrategy(QSqlTableModel::OnManualSubmit);
    WorkAccounting-> setTable("ProductionProgram");
    WorkAccounting->select();
    ui->dynamic_table->setModel(WorkAccounting);
    ui->label_2->setText("ProductionProgram");
}

