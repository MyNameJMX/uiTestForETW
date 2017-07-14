#ifndef TEST_H
#define TEST_H

#include <set>
#include <etwlib.h>
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
	void HandleSave();
	void HandleStart();
	void HandleEnd();
	void HandleFilter();
	void CheckBoxClicked(int state);
	void SeclectAllProviders(int state);
	void SeclectAllFiltedProviders(int state);
	void ShowSeclectedProviders();
	void Timer(int);
private:
	QTimer* timer;
	std::vector<ETWLib::SessionInfo> infos;
	std::vector<std::wstring> allProvidersName;
	QPushButton *CreatStartButton();
	QPushButton *CreatEndButton();
	QPushButton* CreatSavePathButton();
	QCheckBox *selectFilterAll;
	QCheckBox *CreatSelectAllCheckBox();
	QCheckBox *CreatShowSelectedProvidersBox();
	QGroupBox *CreatProvidesGroupBox();
	QLineEdit *CreatFilterLineEdit(); 
	QCheckBox *selcetAll;
	QCheckBox *selectFiltedAll;
	//QScrollArea *scrollAreaFilted;
	QScrollArea* filterLeftProvidersScroll;
	QScrollArea* scrollAreaFilterLeft;
	QScrollArea *scrollAreaAllProvider;
	QScrollArea *scrollAreaSelectedProviders;
	QScrollArea *scrollAreaFilted;
	QVBoxLayout *vBoxAllProviders;
	QVBoxLayout *vBoxLayOutFilted;
	QVBoxLayout* filterLeftLayOut;
	//QVBoxLayout *selectedFilterAllLayOut;
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
	QCheckBox* showSelectedProviders;
	QTextEdit* textShowSelectedProviders;
	ETWLib::ETWSession* session;
	ETWLib::SessionParameters* param;
	std::vector<QCheckBox*> vecAllProviders;//1079
	std::vector<QCheckBox*> vecAllFilterProviders;
	std::set<std::wstring> SelectedProviders;
	std::wstring filePath;
	//Ui::TestClass ui;
};

#endif // TEST_H
