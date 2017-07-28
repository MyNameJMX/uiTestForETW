#ifndef TEST_H
#define TEST_H

#include <ProcessNameAndPID.h>
#include <set>
#include <memory>
#include <etwlib.h>
#include <qtablewidget.h>
#include <QWidgetItem>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTextEdit>
#include <QPointer>
#include <QTimer>
#include <QDebug>
#include <QPoint>
#include <QCursor>
#include "ui_test.h"

class Test : public QWidget
{
	Q_OBJECT

public:
	Test(QWidget *parent = 0);
	private slots:
	void ProcessTableDoubleClicked();
	void HandleSave();
	void HandleStart();
	void HandleEnd();
	void HandleFilter();
	void HandleRefreshTable();
	void CheckBoxClicked(int state);
	void ProcessTableItemClicked(int row);
	void SeclectAllProviders(int state);
	void SeclectAllFiltedProviders(int state);
	void ShowSeclectedProviders();
	void Timer(int);
	void closeEvent(QCloseEvent *event);
private:
	ProcessNameAndPID *processNameAndPID;
	QTimer* timer;
	std::vector<ETWLib::SessionInfo> infos;
	std::vector<std::wstring> allProvidersName;
	QPushButton *CreatStartButton();
	QPushButton *CreatEndButton();
	QPushButton* CreatSavePathButton();
	QPushButton* CreatRefreshProcessPushButton();
	QCheckBox *selectFilterAll;
	QCheckBox *CreatSelectAllCheckBox();
	QCheckBox *CreatShowSelectedProvidersBox();
	QGroupBox *CreatProvidesGroupBox();
	QLineEdit *CreatFilterLineEdit();
	QTableWidget* CreatTable();
	QCheckBox *selectAll;
	QCheckBox *selectFiltedAll;
	QScrollArea* filterLeftProvidersScroll;
	QScrollArea* scrollAreaFilterLeft;
	QScrollArea *scrollAreaAllProvider;
	QScrollArea *scrollAreaSelectedProviders;
	QScrollArea *scrollAreaFilted;
	QVBoxLayout *vBoxAllProviders;
	QVBoxLayout *vBoxLayOutFilted;
	QVBoxLayout* filterLeftLayOut;
	QGroupBox* allProcesses;
	QGroupBox* filterLeftBox;
	QGroupBox* groupBoxFilted;
	QGroupBox* groupBoxSelectedFilterAll;
	QGroupBox* groupBoxAllProviders;
	QLineEdit* lineEditSelectedAllProviders;
	QLineEdit* filter;
	QGridLayout* grid;
	QPushButton* start;
	QPushButton* end;
	QPushButton* save;
	QPushButton* RefreshProcess;
	QCheckBox* showSelectedProviders;
	QTextEdit* textShowSelectedProviders;
	QTableWidget* nameAndPIDTable;
	std::unique_ptr<ETWLib::ETWSession> session;
	std::unique_ptr<ETWLib::SessionParameters> param;
	std::vector<QCheckBox*> vecAllProviders;//1079
	std::vector<QCheckBox*> vecAllFilterProviders;
	std::set<std::wstring> SelectedProviders;
	std::wstring filePath;
	std::map<std::string, DWORD> processNameAndPIDMap;
	std::vector<DWORD> selectedProcessID;
	std::vector<QString> selectedProcessesNames;
	QString selectedProcessName;
	//Ui::TestClass ui;
};

#endif // TEST_H