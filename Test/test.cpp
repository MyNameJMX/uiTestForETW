#include "test.h"
#include<stdio.h>
#include <iostream>
#include <Qtcore/QCoreApplication>



Test::Test(QWidget *parent) : QWidget(parent), session(nullptr), param(nullptr), selectFiltedAll(nullptr),
filterLeftProvidersScroll(nullptr),
filterLeftLayOut(nullptr),
filterLeftBox(nullptr),
scrollAreaFilted(nullptr),
vBoxLayOutFilted(nullptr),//vBoxLayOutFilted
groupBoxFilted(nullptr)//groupBoxFilted
{
	const wchar_t* privilege[1] = { SE_SYSTEM_PROFILE_NAME };
	bool tokenValid = ETWLib::GrantPrivilegeW(privilege, 1);
	timer = new QTimer(this);
	groupBoxSelectedFilterAll = new QGroupBox(tr("searching result after filter"));
	groupBoxSelectedFilterAll->setFlat(true);
	allProvidersName = ETWLib::GetUserProvidersName();
	grid = new QGridLayout;
	QGridLayout *gridCom = new QGridLayout;
	gridCom->addWidget(CreatStartButton(), 0, 0);
	gridCom->addWidget(CreatEndButton(), 0, 1);
	gridCom->addWidget(CreatSavePathButton(), 0, 2);
	gridCom->addWidget(CreatSelectAllCheckBox(), 0, 3);
	grid->addLayout(gridCom, 0, 0);
	grid->addWidget(CreatFilterLineEdit(), 0, 1);
	grid->addWidget(CreatShowSelectedProvidersBox(), 0, 2);
	grid->addWidget(CreatShowProcessNameAndPIDBox(), 0, 3);
	QGridLayout *gridTable = new QGridLayout;
	gridTable->addWidget(CreatTable(), 0, 1); 
	scrollAreaAllProcess = new QScrollArea;
	scrollAreaAllProcess->setWidget(CreatSelectProcessBox());
	gridTable->addWidget(scrollAreaAllProcess, 0, 2);
	grid->addLayout(gridTable, 1, 3);
	scrollAreaAllProvider = new QScrollArea;
	scrollAreaAllProvider->setWidget(CreatProvidesGroupBox());
	grid->addWidget(scrollAreaAllProvider, 1, 0);
	textShowSelectedProviders = new QTextEdit;
	grid->addWidget(textShowSelectedProviders, 1, 2);
	grid->setColumnStretch(0, 1);
	grid->setColumnStretch(1, 1);
	grid->setColumnStretch(2, 1);
	grid->setColumnStretch(3, 1);
	grid->setRowStretch(0, 1);
	grid->setRowStretch(1, 1);
	this->setLayout(grid);
	this->setWindowTitle(tr("Test Version"));
	filePath = (QCoreApplication::applicationDirPath()).toStdWString() + L"/Test.etl";//if path not set,default path "Workspace\Test.etl" 
	this->resize(850, 490);

}

QPushButton* Test::CreatStartButton()
{
	start = new QPushButton(tr("Start"));
	start->setCheckable(true);
	start->setEnabled(false);
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
	save = new QPushButton(tr("SetSavePathHere"));
	save->setCheckable(true);
	save->setEnabled(true);
	connect(save, SIGNAL(clicked()), this, SLOT(HandleSave()));
	return save;
}

QCheckBox* Test::CreatSelectAllCheckBox()
{
	selcetAll = new QCheckBox(tr("SelectAll"));
	selcetAll->isEnabled();
	selcetAll->setCheckable(true);
	selcetAll->setChecked(false);
	connect(selcetAll, SIGNAL(stateChanged(int)), this, SLOT(SeclectAllProviders(int)));
	return selcetAll;
}

QCheckBox* Test::CreatShowSelectedProvidersBox()
{
	showSelectedProviders = new QCheckBox(tr("Show selected providers"));
	showSelectedProviders->setCheckable(true);
	showSelectedProviders->setChecked(false);
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
		connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(CheckBoxClicked(int)));
		vBoxAllProviders->addWidget(checkBox);
	}
	vBoxAllProviders->addStretch(1);
	groupBoxAllProviders->setLayout(vBoxAllProviders);
	return groupBoxAllProviders;
}

QLineEdit* Test::CreatFilterLineEdit()
{
	filter = new QLineEdit;
	filter->setPlaceholderText("Search here!");
	connect(filter, SIGNAL(textEdited(const QString)), this, SLOT(HandleFilter()));
	return filter;
}

QCheckBox* Test::CreatShowProcessNameAndPIDBox() 
{
	showProcessNameAndPID = new QCheckBox(tr("ShowProcessNameAndPID"));
	connect(showProcessNameAndPID, SIGNAL(stateChanged(int)), this, SLOT(HandleTable(int)));
	return showProcessNameAndPID;
}

QTableWidget* Test::CreatTable() 
{
	ProcessNameAndPID processNameAndPID;
	processNameAndPIDMap = processNameAndPID.GetProcessNameAndPID();
	nameAndPIDTable = new QTableWidget(processNameAndPIDMap.size() + 1, 2, this);
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

QGroupBox* Test::CreatSelectProcessBox() 
{
	allProcesses = new QGroupBox(tr("All Processes"));
	allProcesses->setFlat(true);
	vBoxAllProcess = new QVBoxLayout;
	for (auto itor = processNameAndPIDMap.begin(); itor != processNameAndPIDMap.end(); itor++)
	{
		const QString qprovider_str = QString::fromStdString(itor->first);
		QCheckBox *checkBoxProcess = new QCheckBox(qprovider_str);
		checkBoxProcess->setCheckState(Qt::Unchecked);
		connect(checkBoxProcess, SIGNAL(stateChanged(int)), this, SLOT(ProcessCheckBoxClicked(int)));
		vBoxAllProcess->addWidget(checkBoxProcess);
	}
	vBoxAllProcess->addStretch(1);
	allProcesses->setLayout(vBoxAllProcess);
	return allProcesses;
}
void Test::HandleTable(int state) 
{
	QObject* sender = QObject::sender();
	if (state == Qt::Checked) 
	{
		nameAndPIDTable->show();
	}
	else 
	{
		nameAndPIDTable->hide();
	}
}
void Test::HandleStart()
{
	if (session == nullptr)
	{
		QMessageBox::information(this, tr("Hint"), tr("Set path first"));
		return;
	}
	param->EnableProfilling(true);
	session->SetParameters(*param);
	ULONG status = session->StartSession(ETWLib::LogFileMode);

	std::vector<ETWLib::SessionInfo> infosUpdate; 
	ETWLib::QueryAllSessions(infosUpdate);
	infos = infosUpdate;

	start->setEnabled(false);

	end->setEnabled(true);
	if (status == 0)
	{
		QMessageBox::information(this, tr("Error"), tr("Start failed"));
	}
}

void Test::HandleEnd()
{
	session->CloseSession();
	start->setEnabled(true);
	end->setEnabled(false);
	std::string strFilePath(filePath.begin(), filePath.end());
	std::string commondToPath = "xperf -merge temp.etl " + strFilePath;
	QFile file(QString("temp.etl"));
	if (file.exists())
	{
		system(commondToPath.c_str());
	}
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
	grid->addWidget(scrollAreaFilted, 1, 1);


	filterLeftBox->setLayout(filterLeftLayOut);
	filterLeftProvidersScroll->setWidget(filterLeftBox);
	grid->addWidget(filterLeftProvidersScroll, 1, 0);
}

void Test::HandleSave()
{
	std::vector<ETWLib::SessionInfo> infosUpdate;
	ETWLib::QueryAllSessions(infosUpdate);
	infos = infosUpdate;

	for (int i = 0; i < infos.size(); i++)
	{
		if (infos[i].SessionName == std::wstring(L"TraceTest"))
		{
			ETWLib::ETWSession attachedSession(infos[i].TraceHandle);
			attachedSession.CloseSession();
		}
	}
	
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save path"), "", tr("*.etl"));
	if (fileName.size() == 0)
	{
		QMessageBox::information(this, tr("FileSaving"), tr("Please set a path,if none,default path is workspace"));
		fileName = QCoreApplication::applicationDirPath() + "/Test.etl";
	}
	filePath = fileName.toStdWString();
	QObject* sender = QObject::sender();
	((QPushButton*)sender)->setText(fileName);
	session = std::make_unique<ETWLib::ETWSession>(L"TraceTest", L"temp.etl");
	param = std::make_unique<ETWLib::SessionParameters>();
	start->setEnabled(true);
}

void Test::CheckBoxClicked(int state)
{
	QObject* sender = QObject::sender();
	if (param == nullptr)
	{
		QMessageBox::information(this, tr("Error"), tr("Set path first"));
		((QCheckBox*)sender)->setChecked(false);
		return;
	}

	std::wstring wstr = ((QCheckBox*)sender)->text().toStdWString();

	if (state == Qt::Checked)
	{
		SelectedProviders.insert(wstr);
		param->AddUserModeProvider(wstr, true);
	}
	else
	{
		auto itor = SelectedProviders.find(wstr);
		SelectedProviders.erase(itor);
		param->EraseUserModeProvider(wstr);
	}
}

void Test::ProcessCheckBoxClicked(int state) 
{
	//All Processes slots
}
void Test::SeclectAllProviders(int state)
{
	int cntAllProviders = vecAllProviders.size();
	if (param)
	{
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
	else
	{
		QMessageBox::information(this, tr("Error"), tr("Set path first"));
	}
}

void Test::SeclectAllFiltedProviders(int state)
{
	QObject* sender = QObject::sender();
	std::wstring wstr = ((QCheckBox*)sender)->text().toStdWString();
	if (param) 
	{
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
	else 
	{
		QMessageBox::information(this, tr("Error"), tr("Set path first"));
	}
}

void Test::Timer(int)
{
	QObject* sender = QObject::sender();
	if (((QCheckBox*)sender)->isChecked())
	{
		textShowSelectedProviders->show();
		connect(timer, SIGNAL(timeout()), this, SLOT(ShowSeclectedProviders()));
		timer->start(1000);
	}
	else
	{
		disconnect(timer, SIGNAL(timeout()), 0, 0);
		textShowSelectedProviders->hide();
	}
}

void Test::ShowSeclectedProviders()
{
	QRect  labelRect = QRect(this->textShowSelectedProviders->pos(), this->textShowSelectedProviders->size());
	QPoint mouseCurPos = QCursor::pos();
	if (labelRect.contains(mouseCurPos))
	{
		return;
	}
	QString text = "";
	for (auto itor = SelectedProviders.begin(); itor != SelectedProviders.end(); ++itor)
	{
		text = text + "\n" + QString::fromStdWString(*itor);
	}
	textShowSelectedProviders->setText(text);
}
