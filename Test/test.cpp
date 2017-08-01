#include "test.h"
#include<stdio.h>
#include <iostream>
#include <Qtcore/QCoreApplication>
#include <QScrollBar>


Test::Test(QWidget *parent) : QWidget(parent), session(nullptr), param(nullptr), selectFiltedAll(nullptr),
allProvidersName(ETWLib::GetUserProvidersName()),
filterLeftProvidersScroll(nullptr),
filterLeftLayOut(nullptr),
filterLeftBox(nullptr),
scrollAreaFilted(nullptr),
vBoxLayOutFilted(nullptr),//vBoxLayOutFilted
groupBoxFilted(nullptr),//groupBoxFilted
selectedProcessID(0),//initial ProcessID
selectedProcessName(),
selectedProcessesNames(),
filePath((QCoreApplication::applicationDirPath()).toStdWString() + L"/Test.etl")
{
	const wchar_t* privilege[1] = { SE_SYSTEM_PROFILE_NAME };
	bool tokenValid = ETWLib::GrantPrivilegeW(privilege, 1);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(ShowSeclectedProviders()));
	timer->start(1000);

	groupBoxSelectedFilterAll = new QGroupBox(tr("searching result after filter"));
	groupBoxSelectedFilterAll->setFlat(true);
	
	grid = new QGridLayout;
	QGridLayout *gridCom = new QGridLayout;//the following four
	gridCom->addWidget(CreatStartButton(), 0, 0);
	gridCom->addWidget(CreatEndButton(), 0, 1);
	gridCom->addWidget(CreatSavePathButton(), 0, 2);
	gridCom->addWidget(CreatSelectAllCheckBox(), 0, 3);

	grid->addLayout(gridCom, 0, 1);                             //01
	grid->addWidget(CreatFilterLineEdit(), 0, 2);               //02
	grid->addWidget(CreatShowSelectedProvidersBox(), 0, 3);     //03
	grid->addWidget(CreatRefreshProcessPushButton(), 0, 0);     //00
	grid->addWidget(CreatProcessTable(), 1, 0);                 //10
	scrollAreaAllProvider = new QScrollArea;
	scrollAreaAllProvider->setWidget(CreatProvidesGroupBox());
	grid->addWidget(scrollAreaAllProvider, 1, 1);               //11
	textShowSelectedProviders = new QTextEdit;
	grid->addWidget(textShowSelectedProviders, 1, 3);           //13

	grid->setColumnStretch(0, 1);
	grid->setColumnStretch(1, 1);
	grid->setColumnStretch(2, 1);
	grid->setColumnStretch(3, 1);

	grid->setRowStretch(0, 1);
	grid->setRowStretch(1, 1);

	this->setLayout(grid);
	this->setWindowTitle(tr("Test Version"));
	this->resize(1000, 620);
}

QPushButton* Test::CreatStartButton()
{
	start = new QPushButton(tr("Start"));
	start->setCheckable(true);
	start->setEnabled(true);
	connect(start, SIGNAL(clicked()), this, SLOT(HandleStart()));
	return start;
}

QPushButton* Test::CreatEndButton()
{
	end = new QPushButton(tr("End"));
	end->setCheckable(true);
	end->setEnabled(false);
	connect(end, SIGNAL(clicked()), this, SLOT(HandleEnd()));
	return end;
}

QPushButton* Test::CreatSavePathButton()
{
	save = new QPushButton(QString::fromStdWString(filePath));
	save->setCheckable(true);
	save->setEnabled(true);
	connect(save, SIGNAL(clicked()), this, SLOT(HandleSave()));
	return save;
}

QCheckBox* Test::CreatSelectAllCheckBox()
{
	selectAll = new QCheckBox(tr("SelectAll"));
	selectAll->isEnabled();
	selectAll->setCheckable(true);
	selectAll->setChecked(false);
	connect(selectAll, SIGNAL(stateChanged(int)), this, SLOT(SeclectAllProviders(int)));
	return selectAll;
}

QCheckBox* Test::CreatShowSelectedProvidersBox()
{
	showSelectedProviders = new QCheckBox(tr("Show selected providers"));
	showSelectedProviders->setCheckable(true);
	showSelectedProviders->setChecked(true);
	connect(showSelectedProviders, SIGNAL(stateChanged(int)), this, SLOT(Timer(int)));
	return showSelectedProviders;
}

QGroupBox* Test::CreatProvidesGroupBox()
{
	groupBoxAllProviders = new QGroupBox(tr("All Providers"));
	groupBoxAllProviders->setFlat(true);
	vBoxAllProviders = new QVBoxLayout;
	for (int i = 0; i < allProvidersName.size(); i++)
	{
		const QString qprovider_str = QString::fromStdWString(allProvidersName[i]);
		QCheckBox *checkBox = new QCheckBox(qprovider_str);
		vecAllProviders.push_back(checkBox);
		checkBox->setCheckState(Qt::Unchecked);
		connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(ProviderCheckBoxClicked(int)));
		vBoxAllProviders->addWidget(checkBox);
	}
	vBoxAllProviders->addStretch();
	groupBoxAllProviders->setLayout(vBoxAllProviders);
	return groupBoxAllProviders;
}

QLineEdit* Test::CreatFilterLineEdit()
{
	filter = new QLineEdit;
	filter->setPlaceholderText("Search here!");
	filter->setMinimumWidth(400);
	connect(filter, SIGNAL(textEdited(const QString)), this, SLOT(HandleFilter()));
	return filter;
}

QPushButton* Test::CreatRefreshProcessPushButton()
{
	RefreshProcess = new QPushButton(tr("RefreshProcess"));
	connect(RefreshProcess, SIGNAL(clicked()), this, SLOT(HandleRefreshTable()));
	return RefreshProcess;
}

QTableWidget* Test::CreatProcessTable() 
{
	ProcessNameAndPID processNameAndPID;
	processNameAndPIDMap = processNameAndPID.GetProcessNameAndPID();
	nameAndPIDTable = new QTableWidget(processNameAndPIDMap.size(), 2, this);
	nameAndPIDTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	nameAndPIDTable->setSelectionMode(QAbstractItemView::MultiSelection); 
	nameAndPIDTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	nameAndPIDTable->setFixedWidth(445);
	nameAndPIDTable->setColumnWidth(0,200);
	nameAndPIDTable->setColumnWidth(1,200);
	connect(nameAndPIDTable, SIGNAL(cellClicked(int,int)), this, SLOT(ProcessTableItemClicked(int)));
	connect(nameAndPIDTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(ProcessTableDoubleClicked()));

	int row = 0,col = 0 ;
	for (auto itor = processNameAndPIDMap.begin(); itor != processNameAndPIDMap.end(); ++itor) 
	{
		QTableWidgetItem *processName = new QTableWidgetItem(QString::fromStdString(itor->first));
		nameAndPIDTable->setItem(row, 0, processName);
		
		QTableWidgetItem *PID = new QTableWidgetItem(QString::fromStdString(std::to_string(itor->second)));
		nameAndPIDTable->setItem(col, 1, PID);
		++row;
		++col;
	}
	
	return nameAndPIDTable;
}

void Test::HandleRefreshTable() 
{
	delete nameAndPIDTable;
	grid->addWidget(CreatProcessTable(), 1, 0);
}

void Test::HandleStart()
{
	ETWLib::QueryAllSessions(infos);
	for (int i = 0; i < infos.size(); i++)
	{
		if (infos[i].SessionName == std::wstring(L"TraceTest"))//Before start close session named TraceTest
		{
			ETWLib::ETWSession attachedSession(infos[i].TraceHandle);
			attachedSession.CloseSession();
		}
	} 
	session = std::make_unique<ETWLib::ETWSession>(L"TraceTest", L"temp.etl");
	param = std::make_unique<ETWLib::SessionParameters>();
	param->EnableProfilling(true);
	if (QFile::exists(QString::fromStdWString(filePath)))
	{
		if (QMessageBox::warning(this, "File already exists", 
			"You are going to overwrite an existing file!!!\n Press Yes to continue", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
		{
			return;
		}
	}
	for (auto itor = selectedProviders.begin(); itor != selectedProviders.end(); ++itor)
	{
		param->AddUserModeProvider(*itor, true, ETWLib::LevelVerbose, selectedProcessID.data(), selectedProcessID.size());
	}
	
	session->SetParameters(*param);
	ULONG status = session->StartSession(ETWLib::LogFileMode);
	if (status == 0)
	{
		QMessageBox::information(this, tr("Error"), tr("Start failed"));
		return;
	}
	start->setEnabled(false);
	end->setEnabled(true);
}

void Test::HandleEnd()
{
	session->CloseSession();
	start->setEnabled(true);
	end->setEnabled(false);
	std::string strFilePath(filePath.begin(), filePath.end());
	std::string commondToPath = "xperf -merge temp.etl " + strFilePath;
	if (QFile::exists(QString("temp.etl")))
	{
		system(commondToPath.c_str());
	}
}

void Test::HandleSave()
{
	QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save path"), "", tr("*.etl"));
	if (saveFileName.size() == 0)
	{
		QMessageBox::information(this, tr("FileSaving"), tr("Please set a path,if none,default path is workspace"));
		saveFileName = QCoreApplication::applicationDirPath() + "/Test.etl";
	}
	filePath = saveFileName.toStdWString();
	QObject* sender = QObject::sender();
	((QPushButton*)sender)->setText(saveFileName);
}

void Test::HandleFilter()
{
	if (selectFiltedAll != nullptr)
	{
		delete selectFiltedAll;
		selectFiltedAll = nullptr;
	}

	scrollAreaAllProvider->hide();
	vecAllFilterProviders.clear();//USED FOR BUSHUTOON ALL

	QObject* sender = QObject::sender();
	QString qStrFilter = ((QLineEdit*)sender)->text();//qStrFilter

	QPointer<QScrollArea> filterLeftProvidersScroll = new QScrollArea;
	QPointer<QVBoxLayout> filterLeftLayOut = new QVBoxLayout;
	QPointer<QGroupBox> filterLeftBox = new QGroupBox;

	QPointer<QScrollArea> scrollAreaFilted = new QScrollArea;
	QPointer<QVBoxLayout> vBoxLayOutFilted = new QVBoxLayout;//vBoxLayOutFilted
	QPointer<QGroupBox> groupBoxFilted = new QGroupBox(tr("Searching result"));//groupBoxFilted

	selectFiltedAll = new QCheckBox("SelectFiltedAll");//selectFiltedAll
	connect(selectFiltedAll, SIGNAL(stateChanged(int)), this, SLOT(SeclectAllFiltedProviders(int)));
	vBoxLayOutFilted->addWidget(selectFiltedAll);

	for (int i = 0; i < allProvidersName.size(); ++i)
	{
		QString qStrProvider = QString::fromStdWString(allProvidersName[i]); //qStrProvider
		if ((qStrFilter != "") && qStrProvider.contains(qStrFilter, Qt::CaseInsensitive))
		{
			vBoxLayOutFilted->addWidget(vecAllProviders[i]);
			vecAllFilterProviders.push_back(vecAllProviders[i]);
		}
		else
		{
			filterLeftLayOut->addWidget(vecAllProviders[i]);
		}
	}

	groupBoxFilted->setLayout(vBoxLayOutFilted);
	scrollAreaFilted->setWidget(groupBoxFilted);
	grid->addWidget(scrollAreaFilted, 1, 2);// 12


	filterLeftBox->setLayout(filterLeftLayOut);
	filterLeftProvidersScroll->setWidget(filterLeftBox);
	grid->addWidget(filterLeftProvidersScroll, 1, 1);//refresh 11
}

void Test::ProviderCheckBoxClicked(int state)
{
	QObject* sender = QObject::sender();
	std::wstring wstr = ((QCheckBox*)sender)->text().toStdWString();

	if (state == Qt::Checked)
	{
		selectedProviders.insert(wstr);
	}
	else
	{
		auto itor = selectedProviders.find(wstr);
		selectedProviders.erase(itor);
	}
}

void Test::ProcessTableDoubleClicked() 
{
	QObject* sender = QObject::sender();
	int rowCnt = ((QTableWidget*)sender)->rowCount();
	int colCnt = ((QTableWidget*)sender)->columnCount();
	if (selectedProcessID.size() != 0 && selectedProcessesNames.size() != 0)
	{
		for (int i = 1; i < rowCnt; ++i)
		{
			((QTableWidget*)sender)->item(i, 0)->setSelected(false);
			((QTableWidget*)sender)->item(i, 1)->setSelected(false);
		}
		selectedProcessID.clear();
		selectedProcessesNames.clear();
	}
}

void Test::ProcessTableItemClicked(int row) 
{
	bool ok = true;
	auto tableItemName = nameAndPIDTable->item(row, 0);
	DWORD pid;
	QString name;
 	if (tableItemName != nullptr)
 	{
 		selectedProcessName = tableItemName->text();
 	}
	auto tableItemPID = nameAndPIDTable->item(row, 1);
	if (tableItemPID != nullptr)
	{
		pid = tableItemPID->text().toULong(&ok, 10);
		name = tableItemName->text();
	}
	auto itorID = selectedProcessID.begin();
	auto indexName = selectedProcessesNames.begin();
	bool add = 1;//0 means no add
	//Add different element in vector and erase exist element
	while (itorID != selectedProcessID.end() && indexName != selectedProcessesNames.end()) 
	{
		if (*itorID == pid && *indexName == name)
		{
			add = 0;
			break;
		}
		++itorID;
		++indexName;
	}
	if (add == 1) 
	{
		selectedProcessID.push_back(pid);
		selectedProcessesNames.push_back(name);
	}
	else 
	{
		selectedProcessID.erase(itorID);
		selectedProcessesNames.erase(indexName);
	}
}

void Test::SeclectAllProviders(int state)
{
	int cntAllProviders = vecAllProviders.size();
	if (state == Qt::Checked)
	{
		for (int i = 0; i < cntAllProviders; ++i)
		{
			vecAllProviders[i]->setCheckState(Qt::Checked);
		}
	}
	else
	{
		for (int i = 0; i < cntAllProviders; ++i)
		{
			vecAllProviders[i]->setCheckState(Qt::Unchecked);
		}
	}
}

void Test::SeclectAllFiltedProviders(int state)
{
	QObject* sender = QObject::sender();
	std::wstring wstr = ((QCheckBox*)sender)->text().toStdWString();
	if (state == Qt::Checked)
	{
		for (int i = 0; i < vecAllFilterProviders.size(); ++i)
		{
			vecAllFilterProviders[i]->setCheckState(Qt::Checked);
		}
	}
	else
	{
		for (int i = 0; i < vecAllFilterProviders.size(); ++i)
		{
			vecAllFilterProviders[i]->setCheckState(Qt::Unchecked);
		}
	}
}

void Test::Timer(int)
{
	QObject* sender = QObject::sender();
	if (((QCheckBox*)sender)->isChecked())
	{
		timer->start(1000);
	}
	else
	{
		timer->stop();
		textShowSelectedProviders->setText("");
	}
}

void Test::CloseEvent(QCloseEvent *event)
{
	if (end->isEnabled())
	{
		HandleEnd();
	}
}

void Test::ShowSeclectedProviders()
{
	QRect  labelRect = QRect(this->textShowSelectedProviders->pos(), this->textShowSelectedProviders->size());//"textShowSelectedProviders" controls relative to origin
	QPoint mouseCurPos = QCursor::pos() - this->pos();//Mouse relative to origin
	static int scrollBarPos;
	if (labelRect.contains(mouseCurPos))//mouse relative to "textShowSelectedProviders" controls
	{
		return;
	}
	QString text = "";
	for (auto itor =selectedProcessesNames.begin(); itor != selectedProcessesNames.end(); ++itor) 
	{
		text = text + "\n" + *itor;
	}
	for (auto itor = selectedProviders.begin(); itor != selectedProviders.end(); ++itor)
	{
		text = text + "\n" + QString::fromStdWString(*itor);
	}
	textShowSelectedProviders->setText(text);
}
