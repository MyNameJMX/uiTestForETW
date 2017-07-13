#include "test.h"
#include <iostream>
#include <Qtcore/QCoreApplication>



Test::Test(QWidget *parent) : QWidget(parent), session(NULL), param(NULL), selectFiltedAll(NULL), 
filterLeftProvidersScroll(NULL),
filterLeftLayOut(NULL),
filterLeftBox(NULL),

scrollAreaFilted(NULL),
vBoxLayOutFilted(NULL),//vBoxLayOutFilted
groupBoxFilted(NULL)//groupBoxFilted

{
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
	scrollAreaAllProvider = new QScrollArea;
	scrollAreaAllProvider->setWidget(CreatProvidesGroupBox());
	grid->addWidget(scrollAreaAllProvider, 1, 0);
	textShowSelectedProviders = new QTextEdit;
	grid->addWidget(textShowSelectedProviders, 1, 2);
	grid->setColumnStretch(0, 1);
	grid->setColumnStretch(1, 1);
	grid->setColumnStretch(2, 1);
	grid->setRowStretch(0, 1);
	grid->setRowStretch(1, 1);
	this->setLayout(grid);
	this->setWindowTitle(tr("Test Version"));
	const wchar_t* privilege[1] = { SE_SYSTEM_PROFILE_NAME };
	bool tokenValid = ETWLib::GrantPrivilegeW(privilege, 1);
	filePath = (QCoreApplication::applicationDirPath()).toStdWString ()+ L"/Test.etl";//if path not set,default path "Workspace\Test.etl" 
	this->resize(850, 490);

	//FOR FILTER
	//filterLeftProvidersScroll = new QScrollArea;
	//filterLeftLayOut = new QVBoxLayout;
	//filterLeftBox = new QGroupBox;

	//scrollAreaFilted = new QScrollArea;
	//vBoxLayOutFilted = new QVBoxLayout;//vBoxLayOutFilted
	//groupBoxFilted = new QGroupBox(tr("Searching result"));//groupBoxFilted


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
	//showSelectedProviders->setFixedSize(300, 20);
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
	//filter->setFixedSize(300, 20);
	connect(filter, SIGNAL(textEdited(const QString)), this, SLOT(HandleFilter()));
	return filter;
}

void Test::HandleStart()
{
	if (session == NULL)
	{
		QMessageBox::information(this, tr("Hint"), tr("Set path first"));
		return;
	}
	ULONG status = session->StartSession(ETWLib::LogFileMode);
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
	delete session;
	delete param;
	session = new ETWLib::ETWSession(L"TraceTest", filePath);
	param = new ETWLib::SessionParameters();
	start->setEnabled(true);
	end->setEnabled(false);
}

void Test::HandleFilter()
{
	if (selectFiltedAll != NULL)
	{
		delete selectFiltedAll;
		selectFiltedAll = NULL;
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

	//vBoxLayOutFilted reset them here
	//filterLeftLayOut

	/*for (int idx = 0; idx < vBoxLayOutFilted->count(); idx++)
	{
		QLayoutItem * const item = vBoxLayOutFilted->itemAt(idx);
		if (dynamic_cast<QWidgetItem *>(item))
			vBoxLayOutFilted->removeWidget(item->widget());
	}

	for (int idx = 0; idx < filterLeftLayOut->count(); idx++)
	{
		QLayoutItem * const item = filterLeftLayOut->itemAt(idx);
		if (dynamic_cast<QWidgetItem *>(item))
			filterLeftLayOut->removeWidget(item->widget());
	}*/

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
	vBoxLayOutFilted->addStretch(1);
	groupBoxFilted->setLayout(vBoxLayOutFilted);
	
	scrollAreaFilted->setWidget(groupBoxFilted);
	grid->addWidget(scrollAreaFilted, 1, 1);

	filterLeftBox->setLayout(filterLeftLayOut);
	filterLeftProvidersScroll->setWidget(filterLeftBox);
	grid->addWidget(filterLeftProvidersScroll, 1, 0);
}

void Test::HandleSave()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save path"), "", tr("*.etl"));
	if (fileName.size() == 0)
	{
		QMessageBox::information(this, tr("FileSaving"), tr("Please set a path,if none,default path is workspace"));
		fileName = QCoreApplication::applicationDirPath() + "/Test.etl";
	}
	filePath = fileName.toStdWString();
	if (fileName != QCoreApplication::applicationDirPath() + "/Test.etl")
	{
		QMessageBox::information(this, tr("FileSaving"), tr("Path already set"));
	}
	QObject* sender = QObject::sender();
	((QPushButton*)sender)->setText(fileName);
	if (session == NULL) //Make sure there is only one instance exist
	{
		session = new ETWLib::ETWSession(L"TraceTest", filePath);
		param = new ETWLib::SessionParameters();
	}
	param->EnableProfilling(true);
	session->SetParameters(*param);
	start->setEnabled(true);
}

void Test::CheckBoxClicked(int state)
{
	QObject* sender = QObject::sender();
	if (param == NULL)
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

void Test::SeclectAllProviders(int state)
{
	QObject* sender = QObject::sender();
	std::wstring wstr = ((QCheckBox*)sender)->text().toStdWString();
	if (filePath.size())
	{
		if (state == Qt::Checked)
		{
			for (int i = 0; i < vecAllProviders.size(); ++i)
			{
				vecAllProviders[i]->setCheckState(Qt::Checked);
			}
		}
		else
		{
			for (int i = 0; i < vecAllProviders.size(); ++i)
			{
				vecAllProviders[i]->setCheckState(Qt::Unchecked);
			}
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
	QRect  labelRect = QRect(this->textShowSelectedProviders->pos(),this->textShowSelectedProviders->size());
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


