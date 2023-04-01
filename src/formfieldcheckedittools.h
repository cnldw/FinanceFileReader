/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the Apache License, Version 2.0 (the "License");
*you may not use this file except in compliance with the License.
*You may obtain a copy of the License at
*
*    http:*www.apache.org/licenses/LICENSE-2.0
*
*Unless required by applicable law or agreed to in writing, software
*distributed under the License is distributed on an "AS IS" BASIS,
*WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*See the License for the specific language governing permissions and
*limitations under the License.
************************************************************************/
#ifndef FORMFIELDCHECKEDITTOOLS_H
#define FORMFIELDCHECKEDITTOOLS_H

#include <QMainWindow>
#include <QStatusBar>
#include <QCheckBox>
#include <QSplitter>
#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include "src/fieldcheckitem.h"
#include "src/ofdfiledefinition.h"
#include "src/fixedfiledefinition.h"
#include "src/csvfiledefinition.h"
#include "src/configfile.h"
#include "src/utils.h"
#include "src/dialogmytip.h"
#include "src/dialogedittext.h"


namespace Ui {
class FormFieldCheckEditTools;
}

class FormFieldCheckEditTools : public QMainWindow
{
    Q_OBJECT

public:
    explicit FormFieldCheckEditTools(QList<ConfigFile<OFDFileDefinition>> ofdConfigListPar,QList<ConfigFile<CsvFileDefinition>> csvConfigListPar,QList<ConfigFile<FIXEDFileDefinition>> fixedConfigListPar,QWidget *parent = nullptr);
    ~FormFieldCheckEditTools();

private slots:
    void table_field_selectAll();
    void table_field_clearAllSelet();
    void table_field_clearCondition();

    void on_comboBox_1_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_searchPushButton_clicked();

    void on_tableWidgetFieldList_itemSelectionChanged();

    void on_tableWidgetFieldList_customContextMenuRequested(const QPoint &pos);

    void on_tableWidgetCheckList_customContextMenuRequested(const QPoint &pos);

    void fieldCheckEditSelect();

    void fieldCheckDeleteSelect();

    void fieldConEditSelect();

    void fieldConAdd();

    void checkAdd();

    void fieldConDeleteSelect();

    void on_pushButton_abandonSummit_clicked();

    void on_pushButton_summitCon_clicked();

    void on_pushButton_summitField_clicked();

    void on_tableWidgeConditionList_customContextMenuRequested(const QPoint &pos);

    void on_pushButton_Summit_clicked();

    void on_pushButton_AbandonWrite_clicked();

    void on_pushButton_WriteToFile_clicked();

    void closeEvent(QCloseEvent *event);

    void on_pushButton_ImportCon_clicked();

    void on_pushButton_ImportField_clicked();

public slots:
    void getImportString(QString data,int type,bool clearOldData);

private:
    Ui::FormFieldCheckEditTools *ui;
    QList<ConfigFile<OFDFileDefinition>> ofdConfigList;
    QList<ConfigFile<CsvFileDefinition>> csvConfigList;
    QList<ConfigFile<FIXEDFileDefinition>> fixedConfigList;
    QString windowName="字段必填规则配置工具";
    //上级变动时下级不渲染
    bool comboBox_1OK=false;
    bool comboBox_2OK=false;

    void initCurrentItem(int index1,int index2,int index3);
    void statusBarDisplayMessage(QString message);
    void clearOldData();
    void inEditMode();
    void outEditMode();

    QString currentIni="";
    QString currentConfigSegment="";

    int searchRow =0;
    int searchColumn=0;

    bool changeNotSummit=false;

    bool changeNotSaveToFile=false;

    //当前正在编辑配置段的所有必填检查清单--只有提交到文件或者放弃时才可以清空
    QList<fieldcheckitem> currentEditConfigSegmentCheckItemList;
    bool editMode=false;
    //当前正在编辑的是第几个规则和该规则需要的字段清单
    int currentEditCheckIndex=-1;
    QList<int> currentEditRequiredFields;
    //当前编辑的规则中的所有条件清单以及正在编辑的条件分组下标
    int currentEditConditionIndex=-1;
    QList<QList<CheckCondition>> currentEditConListOverList;

    int newAddCheckIndex=-1;
    bool newAddCheckInited=true;


    int newAddConditionIndex=-1;
    bool newAddConditionInited=true;

    //哪些配置修改了
    QStringList modifyConfigList;

    //表格菜单
    QMenu *table_field_PopMenu;
    QAction *action_table_field_selectall;
    QAction *action_table_field_clear_select;
    QAction *action_table_field_clear_condition;

    QMenu *table_check_PopMenu;
    QAction *action_table_check_add;
    QAction *action_table_check_edit_select;
    QAction *action_table_check_delete_select;

    QMenu *table_condition_PopMenu;
    QAction *action_table_condition_add;
    QAction *action_table_condition_edit_select;
    QAction *action_table_condition_delete_select;

signals:
    void sendConfigChange(bool flag,QStringList modifyConfigList);
};

#endif // FORMFIELDCHECKEDITTOOLS_H
