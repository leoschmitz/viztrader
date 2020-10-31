//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCWorkAreaManagerWidget.h"
#include "VizMath.h"
#include "VizCApplicationData.h"
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QDialogButtonBox>
#include <QApplication>
#include <QList>
#include <QMessageBox>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCWorkAreaManagerItem : public QListWidgetItem
{
public:
	VizCWorkAreaManagerItem(const QString& name) : QListWidgetItem()
	{
		setText(name);
	}
	s32 Index;
};
//-----------------------------------------------------------------------------
VizCWorkAreaManagerWidget* VizCWorkAreaManagerWidget::Instance = 0;
//-----------------------------------------------------------------------------
VizCWorkAreaManagerWidget* VizCWorkAreaManagerWidget::instance()
{
	if(!Instance)
	{
		Instance = new VizCWorkAreaManagerWidget;
	}

	return Instance;
}
//-----------------------------------------------------------------------------
VizCWorkAreaManagerWidget::VizCWorkAreaManagerWidget() : QDialog(0), WorkAreaManager(0), ListWidget(0)
{
	setWindowIcon(QIcon(":/VizTrader/Resources/windowIcon.png"));
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	setMinimumSize(QSize(600, 300));
	setModal(true);
	setWindowTitle(tr("Gerenciar Múltiplas Área de Trabalho"));	

	NewWorkAreaButton = new QToolButton(this);
	NewWorkAreaButton->setAutoRaise(true);
	NewWorkAreaButton->setIcon(QPixmap(":/VizTrader/Resources/addWorkArea.png"));
	QObject::connect(NewWorkAreaButton, SIGNAL(clicked()), this, SLOT(newWorkArea()));
	NewWorkAreaButton->setToolTip(tr("Adicionar"));

	RemoveWorkAreaButton = new QToolButton(this);
	RemoveWorkAreaButton->setAutoRaise(true);
	RemoveWorkAreaButton->setIcon(QPixmap(":/VizTrader/Resources/removeWorkArea.png"));
	QObject::connect(RemoveWorkAreaButton, SIGNAL(clicked()), this, SLOT(removeWorkArea()));
	RemoveWorkAreaButton->setToolTip(tr("Remover"));

	RenameWorkAreaButton = new QToolButton(this);
	RenameWorkAreaButton->setAutoRaise(true);
	RenameWorkAreaButton->setIcon(QPixmap(":/VizTrader/Resources/edit.png"));
	QObject::connect(RenameWorkAreaButton, SIGNAL(clicked()), this, SLOT(renameWorkArea()));
	RenameWorkAreaButton->setToolTip(tr("Renomear"));


	MoveUpWorkAreaButton = new QToolButton(this);
	MoveUpWorkAreaButton->setAutoRaise(true);
	MoveUpWorkAreaButton->setIcon(QPixmap(":/VizTrader/Resources/up.png"));
	QObject::connect(MoveUpWorkAreaButton, SIGNAL(clicked()), this, SLOT(moveUpWorkArea()));
	MoveUpWorkAreaButton->setToolTip(tr("Mover para Cima"));

	MoveDownWorkAreaButton = new QToolButton(this);
	MoveDownWorkAreaButton->setAutoRaise(true);
	MoveDownWorkAreaButton->setIcon(QPixmap(":/VizTrader/Resources/down.png"));
	QObject::connect(MoveDownWorkAreaButton, SIGNAL(clicked()), this, SLOT(moveDownWorkArea()));
	MoveDownWorkAreaButton->setToolTip(tr("Mover para Baixo"));

	
	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(accept()));
	

	ListWidget = new QListWidget(this);
	connect(ListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slotItemClicked(QListWidgetItem*)));
	connect(ListWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(slotItemChanged(QListWidgetItem*)));
	connect(ListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slotItemDoubleClicked(QListWidgetItem*)));

	ListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	ListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	ListWidget->setWordWrap(true);
	ListWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
	ListWidget->setAlternatingRowColors(true);
	
	QHBoxLayout* layout1 = new QHBoxLayout;
	QVBoxLayout* layout2 = new QVBoxLayout;
	QVBoxLayout* layout3 = new QVBoxLayout;
	

	layout2->addWidget(NewWorkAreaButton);
	layout2->addWidget(RemoveWorkAreaButton);
	layout2->addWidget(RenameWorkAreaButton);
	layout2->addWidget(MoveUpWorkAreaButton);
	layout2->addWidget(MoveDownWorkAreaButton);
	layout2->addStretch();

	layout1->addWidget(ListWidget);
	layout1->addLayout(layout2);

	layout3->addLayout(layout1);
	layout3->addWidget(buttonBox);
	
	setLayout(layout3);

}
//-----------------------------------------------------------------------------
VizCWorkAreaManagerWidget::~VizCWorkAreaManagerWidget()
{
}
//-----------------------------------------------------------------------------
void VizCWorkAreaManagerWidget::setWorkAreaManager(VizCWorkAreaManager* wm)
{
	Q_ASSERT(wm);
	WorkAreaManager = wm;
}
//-----------------------------------------------------------------------------
int	VizCWorkAreaManagerWidget::exec()
{
	updateListView();
	updateButtons();
	
	Modified = 0;
	QDialog::exec();
	return Modified;
}
//-----------------------------------------------------------------------------
void VizCWorkAreaManagerWidget::updateListView()
{
	ListWidget->blockSignals(true);

	ListWidget->clear();
	for(s32 i = 0; i < WorkAreaManager->workAreaCount(); i++)
	{
		QString itemText = WorkAreaManager->workArea(i).first;
		VizCWorkAreaManagerItem* newItem = new VizCWorkAreaManagerItem(itemText);
		newItem->Index = i;
		newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
		ListWidget->insertItem(i, newItem);
	}

	ListWidget->blockSignals(false);

}
//-----------------------------------------------------------------------------
void VizCWorkAreaManagerWidget::updateButtons()
{
	if(ListWidget->currentRow() == -1)
	{
		RemoveWorkAreaButton->setEnabled(false);
		RenameWorkAreaButton->setEnabled(false);
		MoveUpWorkAreaButton->setEnabled(false);
		MoveDownWorkAreaButton->setEnabled(false);
	}
	else
	{
		RemoveWorkAreaButton->setEnabled(true);
		RenameWorkAreaButton->setEnabled(true);
		MoveUpWorkAreaButton->setEnabled(true);
		MoveDownWorkAreaButton->setEnabled(true);
	}

}
//-----------------------------------------------------------------------------
void VizCWorkAreaManagerWidget::newWorkArea()
{
	WorkAreaManager->newWorkArea();
	updateListView();
	Modified = 1;
}
//-----------------------------------------------------------------------------
void VizCWorkAreaManagerWidget::removeWorkArea()
{
	
	VizCWorkAreaManagerItem* item = dynamic_cast<VizCWorkAreaManagerItem*>(ListWidget->currentItem());
	if(item)
	{
		if(ListWidget->count() > 1)
		{
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, system::VizCApplicationData::instance()->appicationName(), "Remover área de trabalho?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			if(reply == QMessageBox::Yes)
			{
				WorkAreaManager->removeWorkArea(item->Index);
				updateListView();
				Modified = 1;
			}
		}
		else
		{
			QApplication::beep();
		}
	}
}
//-----------------------------------------------------------------------------
void VizCWorkAreaManagerWidget::renameWorkArea()
{
	
	QListWidgetItem* item = ListWidget->currentItem();

	if(item)
	{
		OldText = item->text();
		ListWidget->editItem(item);
	}
}
//-----------------------------------------------------------------------------
void VizCWorkAreaManagerWidget::moveUpWorkArea()
{
	s32 row = ListWidget->currentRow();
	if(row != -1)
	{
		s32 newrow = row - 1;
		if(newrow >= 0)
		{
			WorkAreaManager->moveWorkArea(newrow, row);
			updateListView();
			Modified = 1;
		}
	}
}
//-----------------------------------------------------------------------------
void VizCWorkAreaManagerWidget::moveDownWorkArea()
{
	s32 row = ListWidget->currentRow();
	if(row != -1)
	{
		s32 newrow = row + 1;
		if(newrow <= ListWidget->count() - 1)
		{
			WorkAreaManager->moveWorkArea(newrow, row);
			updateListView();
			Modified = 1;
		}
	}
}
//-----------------------------------------------------------------------------
void VizCWorkAreaManagerWidget::slotItemClicked(QListWidgetItem* item)
{
	updateButtons();
}
//-----------------------------------------------------------------------------
void VizCWorkAreaManagerWidget::slotItemDoubleClicked(QListWidgetItem* item)
{
	OldText = item->text();
}
//-----------------------------------------------------------------------------
void VizCWorkAreaManagerWidget::slotItemChanged(QListWidgetItem* i)
{
	VizCWorkAreaManagerItem* item = dynamic_cast<VizCWorkAreaManagerItem*>(i);
	if(item)
	{
		QString text = item->text();
		if(OldText != text)
		{
			
			bool ok = WorkAreaManager->renameWorkAreaName(item->Index, text);
			if(!ok)
			{
				QApplication::beep();
			}
			else
			{
				updateListView();
				Modified = 1;
			}
			
		}
	}

	OldText.clear();

}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
