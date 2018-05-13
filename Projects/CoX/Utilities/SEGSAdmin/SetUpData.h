/*
 * SEGS - Super Entity Game Server
 * http://www.segs.io/
 * Copyright (c) 2006 - 2018 SEGS Team (see Authors.txt)
 * This software is licensed! (See License.txt for details)
 */

#ifndef SETUPDATA_H
#define SETUPDATA_H

#include <QDialog>
#include <QProcess>


namespace Ui {
class SetUpData;
}

class SetUpData : public QDialog
{
    Q_OBJECT

public:
    explicit SetUpData(QWidget *parent = 0);
    ~SetUpData();

public slots:
    void select_piggs_dir();
    void open_data_dialog();
    void copy_piggs_files();
    void pigg_dispatcher();
    void pigg_tool_worker(QString program);
    void create_default_directory(QString maps_dir);
    void read_piggtool();
    void pigg_dispatcher_wait_dialog();
    void check_client_version();

signals:
    void fileCopyComplete();
    void callPiggWorker(QString program);
    void dataSetupComplete();
    void getMapsDir();
    void quitPiggLoop();
    void readyToCopy();

private:
    Ui::SetUpData *ui;
    QProcess *m_pigg_tool;

};

#endif // SETUPDATA_H
