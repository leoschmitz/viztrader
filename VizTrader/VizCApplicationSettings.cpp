//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCApplicationSettings.h"
#include "VizCMemoryUtils.h"
#include "VizGUIUtils.h"
#include <QApplication>
#include <QMessageBox>
#include <QStyleFactory>
#include <QFontDialog>
#include <QColorDialog>
#include <QDebug>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCApplicationSettings* VizCApplicationSettings::Instance = 0;
//-----------------------------------------------------------------------------
VizCApplicationSettings::VizCApplicationSettings(QWidget* parent) : QWidget(parent)
{
	setupUi(this);
	setWindowIcon(QIcon(":/VizTrader/Resources/windowIcon.png"));
	verticalLayout_3->setMargin(0);
	verticalLayout_4->setMargin(0);
	verticalLayout_13->setMargin(0);
	verticalLayout_13->setMargin(0);
	verticalLayout_19->setMargin(0);
	
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);


	QObject::connect(PushButtonDefault, SIGNAL(clicked(bool)), this, SLOT(slotResetToDefault()));
	QObject::connect(PushButtonApply, SIGNAL(clicked(bool)), this, SLOT(slotApply()));
	QObject::connect(PushButtonCancel, SIGNAL(clicked(bool)), this, SLOT(slotCancel()));
	QObject::connect(PushButtonOK, SIGNAL(clicked(bool)), this, SLOT(slotOk()));

	Settings = io::VizCSettings::instance();

	createGeneralTab();
	createAppearanceTab();
	createPerformanceTab();

	load();
	//save();

	//QWidget* vp = new QWidget();
	//vp->setAttribute( Qt::WA_NoSystemBackground, true );

	//scrollAreateste->setAttribute( Qt::WA_NoSystemBackground, true );
	//scrollAreateste->setFrameShape( QFrame::NoFrame );
	//scrollAreateste->setViewport(vp);

	//scrollAreateste->setStyleSheet("QScrollArea { background-color: yellow }");


	/*QFont defaultFont;
	defaultFont.fromString(Settings->value("DefaultFont", QApplication::font()).toString());
	setFont(defaultFont);
	*/
	
}
//-----------------------------------------------------------------------------
VizCApplicationSettings::~VizCApplicationSettings()
{
}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::keyPressEvent(QKeyEvent* event)
{
	switch(event->key())
	{
		case Qt::Key_Escape:
		{
			slotCancel();
			break;
		}
	}

	QWidget::keyPressEvent(event);
}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::createGeneralTab()
{
}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::setToDefaultGeneralTab()
{

	General_CheckBox_1->setCheckState(Qt::Checked);
	General_CheckBox_2->setCheckState(Qt::Checked);
	General_CheckBox_3->setCheckState(Qt::Unchecked);
	General_CheckBox_4->setCheckState(Qt::Checked);

}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::loadGeneralTab()
{
	bool checked;

	checked = Settings->value("General/Workspace/RestoreWorkspaceOnStartup", false).toBool();
	General_CheckBox_0->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
	checked = Settings->value("General/Edit/ConfirmDeletionOfItens", false).toBool();
	General_CheckBox_1->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
	checked = Settings->value("General/Edit/ReturnToSelectModeAfterAddedItem", false).toBool();
	General_CheckBox_2->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
	checked = Settings->value("General/Edit/SnapToPriceAfterAddedItem", false).toBool();
	General_CheckBox_3->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
	checked = Settings->value("General/Edit/AddItemAsAChildOfTheNearestItem", true).toBool();
	General_CheckBox_4->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::saveGeneralTab()
{
	bool checked;

	checked = General_CheckBox_0->checkState() == Qt::Checked ? true : false;
	Settings->setValue("General/Workspace/RestoreWorkspaceOnStartup", checked);
	checked = General_CheckBox_1->checkState() == Qt::Checked ? true : false;
	Settings->setValue("General/Edit/ConfirmDeletionOfItens", checked);
	checked = General_CheckBox_2->checkState() == Qt::Checked ? true : false;
	Settings->setValue("General/Edit/ReturnToSelectModeAfterAddedItem", checked);
	checked = General_CheckBox_3->checkState() == Qt::Checked ? true : false;
	Settings->setValue("General/Edit/SnapToPriceAfterAddedItem", checked);
	checked = General_CheckBox_4->checkState() == Qt::Checked ? true : false;
	Settings->setValue("General/Edit/AddItemAsAChildOfTheNearestItem", checked);
}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::createAppearanceTab()
{

	AppearanceButtonGroup = new QButtonGroup(this);

	QFont defaultFont;
	defaultFont.fromString(Settings->value("DefaultFont", QApplication::font()).toString());

	Appearance_ComboBox_1->addItems(QStyleFactory::keys());

	//application font
	Font[0] = defaultFont;
	Appearance_FontLabel1->setPixmap(gui::fontValuePixmap(defaultFont));
	Appearance_FontLabel2->setText(gui::fontValueText(defaultFont));
	//item font
	Font[1] = defaultFont;
	Appearance_FontLabel3->setPixmap(gui::fontValuePixmap(defaultFont));
	Appearance_FontLabel4->setText(gui::fontValueText(defaultFont));
	
	//foreground color
	Color[0] = Qt::black;
	Appearance_ColorLabel1->setPixmap(gui::brushValuePixmap(Color[0]));
	Appearance_ColorLabel2->setText(gui::colorValueText(Color[0]));
	//background color
	Color[1] = Qt::white;
	Appearance_ColorLabel3->setPixmap(gui::brushValuePixmap(Color[1]));
	Appearance_ColorLabel4->setText(gui::colorValueText(Color[1]));
	//grid color
	Color[2] = gui::VizGray;
	Appearance_ColorLabel5->setPixmap(gui::brushValuePixmap(Color[2]));
	Appearance_ColorLabel6->setText(gui::colorValueText(Color[2]));
	//up color
	Color[3] = gui::VizGreen;
	Appearance_ColorLabel7->setPixmap(gui::brushValuePixmap(Color[3]));
	Appearance_ColorLabel8->setText(gui::colorValueText(Color[3]));
	//down color
	Color[4] = gui::VizRed;
	Appearance_ColorLabel9->setPixmap(gui::brushValuePixmap(Color[4]));
	Appearance_ColorLabel10->setText(gui::colorValueText(Color[4]));
	//indicator color
	Color[5] = gui::VizRed;
	Appearance_ColorLabel11->setPixmap(gui::brushValuePixmap(Color[5]));
	Appearance_ColorLabel12->setText(gui::colorValueText(Color[5]));
	//item color
	Color[6] = gui::VizBlue;
	Appearance_ColorLabel13->setPixmap(gui::brushValuePixmap(Color[6]));
	Appearance_ColorLabel14->setText(gui::colorValueText(Color[6]));

	
	AppearanceButtonGroup->addButton(Appearance_FontToolButton1);
	AppearanceButtonGroup->addButton(Appearance_FontToolButton2);
	AppearanceButtonGroup->addButton(Appearance_ColorToolButton1);
	AppearanceButtonGroup->addButton(Appearance_ColorToolButton2);
	AppearanceButtonGroup->addButton(Appearance_ColorToolButton3);
	AppearanceButtonGroup->addButton(Appearance_ColorToolButton4);
	AppearanceButtonGroup->addButton(Appearance_ColorToolButton5);
	AppearanceButtonGroup->addButton(Appearance_ColorToolButton6);
	AppearanceButtonGroup->addButton(Appearance_ColorToolButton7);
	connect(AppearanceButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(Appearance_PushButton_Clicked(QAbstractButton*)));


	//line width
	QStringList linewidth;
	linewidth << tr("1 pixel")<< tr("2 pixel")<< tr("3 pixel")<< tr("4 pixel")<< tr("5 pixel");
	Appearance_ComboBox_3->addItems(linewidth);
	Appearance_ComboBox_5->addItems(linewidth);
	Appearance_ComboBox_8->addItems(linewidth);
	Appearance_ComboBox_10->addItems(linewidth);


	//line style
	QStringList linestyles;
	linestyles << tr("Invisível")<< tr("Linha Sólida")<< tr("Traços")<< tr("Pontos")<< tr("Traço/Ponto")<< tr("Traço/Ponto/Ponto");
	Appearance_ComboBox_2->addItems(linestyles);
	Appearance_ComboBox_7->addItems(linestyles);
	Appearance_ComboBox_9->addItems(linestyles);

	//indicator style
	QStringList indicatortyles;
	indicatortyles << tr("Linhas")<< tr("Pontos")<< tr("Histograma");
	Appearance_ComboBox_6->addItems(indicatortyles);

}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::Appearance_PushButton_Clicked(QAbstractButton* button)
{
	/*QFont defaultFont;
	defaultFont.fromString(Settings->value("DefaultFont", QApplication::font()).toString());
	*/

	if(button == Appearance_FontToolButton1)
	{
		QFont m_font = Font[0];
		bool ok = false;
		QFont newFont = QFontDialog::getFont(&ok, m_font, this);

		if(ok && newFont != m_font)
		{
			QFont f = m_font;
			// prevent mask for unchanged attributes, don't change other attributes (like kerning, etc...)
			if (m_font.family() != newFont.family())
				f.setFamily(newFont.family());
			if (m_font.pointSize() != newFont.pointSize())
				f.setPointSize(newFont.pointSize());
			if (m_font.bold() != newFont.bold())
				f.setBold(newFont.bold());
			if (m_font.italic() != newFont.italic())
				f.setItalic(newFont.italic());
			if (m_font.underline() != newFont.underline())
				f.setUnderline(newFont.underline());
			if (m_font.strikeOut() != newFont.strikeOut())
				f.setStrikeOut(newFont.strikeOut());

			Font[0] = f;
			Appearance_FontLabel1->setPixmap(gui::fontValuePixmap(Font[0]));
			Appearance_FontLabel2->setText(gui::fontValueText(Font[0]));
		}
	}
	else
	if(button == Appearance_FontToolButton2)
	{
		QFont m_font = Font[1];
		bool ok = false;
		QFont newFont = QFontDialog::getFont(&ok, m_font, this);
		if(ok && newFont != m_font)
		{
			QFont f = m_font;
			// prevent mask for unchanged attributes, don't change other attributes (like kerning, etc...)
			if (m_font.family() != newFont.family())
				f.setFamily(newFont.family());
			if (m_font.pointSize() != newFont.pointSize())
				f.setPointSize(newFont.pointSize());
			if (m_font.bold() != newFont.bold())
				f.setBold(newFont.bold());
			if (m_font.italic() != newFont.italic())
				f.setItalic(newFont.italic());
			if (m_font.underline() != newFont.underline())
				f.setUnderline(newFont.underline());
			if (m_font.strikeOut() != newFont.strikeOut())
				f.setStrikeOut(newFont.strikeOut());

			Font[1] = f;
			Appearance_FontLabel3->setPixmap(gui::fontValuePixmap(Font[1]));
			Appearance_FontLabel4->setText(gui::fontValueText(Font[1]));
		}
	}
	else
	if(button == Appearance_ColorToolButton1)
	{
		bool ok = false;
		QRgb oldRgba = Color[0].rgba();
		QRgb newRgba = QColorDialog::getRgba(oldRgba, &ok, this);
		if(ok && newRgba != oldRgba)
		{
			Color[0] = QColor::fromRgba(newRgba);
			Appearance_ColorLabel1->setPixmap(gui::brushValuePixmap(Color[0]));
			Appearance_ColorLabel2->setText(gui::colorValueText(Color[0]));
		}
	}
	else
	if(button == Appearance_ColorToolButton2)
	{
		bool ok = false;
		QRgb oldRgba = Color[1].rgba();
		QRgb newRgba = QColorDialog::getRgba(oldRgba, &ok, this);
		if(ok && newRgba != oldRgba)
		{
			Color[1] = QColor::fromRgba(newRgba);
			Appearance_ColorLabel3->setPixmap(gui::brushValuePixmap(Color[1]));
			Appearance_ColorLabel4->setText(gui::colorValueText(Color[1]));
		}
	}
	else
	if(button == Appearance_ColorToolButton3)
	{
		bool ok = false;
		QRgb oldRgba = Color[2].rgba();
		QRgb newRgba = QColorDialog::getRgba(oldRgba, &ok, this);
		if(ok && newRgba != oldRgba)
		{
			Color[2] = QColor::fromRgba(newRgba);
			Appearance_ColorLabel5->setPixmap(gui::brushValuePixmap(Color[2]));
			Appearance_ColorLabel6->setText(gui::colorValueText(Color[2]));
		}
	}
	else
	if(button == Appearance_ColorToolButton4)
	{
		bool ok = false;
		QRgb oldRgba = Color[3].rgba();
		QRgb newRgba = QColorDialog::getRgba(oldRgba, &ok, this);
		if(ok && newRgba != oldRgba)
		{
			Color[3] = QColor::fromRgba(newRgba);
			Appearance_ColorLabel7->setPixmap(gui::brushValuePixmap(Color[3]));
			Appearance_ColorLabel8->setText(gui::colorValueText(Color[3]));
		}
	}
	else
	if(button == Appearance_ColorToolButton5)
	{
		bool ok = false;
		QRgb oldRgba = Color[4].rgba();
		QRgb newRgba = QColorDialog::getRgba(oldRgba, &ok, this);
		if(ok && newRgba != oldRgba)
		{
			Color[4] = QColor::fromRgba(newRgba);
			Appearance_ColorLabel9->setPixmap(gui::brushValuePixmap(Color[4]));
			Appearance_ColorLabel10->setText(gui::colorValueText(Color[4]));
		}
	}
	else
	if(button == Appearance_ColorToolButton6)
	{
		bool ok = false;
		QRgb oldRgba = Color[5].rgba();
		QRgb newRgba = QColorDialog::getRgba(oldRgba, &ok, this);
		if(ok && newRgba != oldRgba)
		{
			Color[5] = QColor::fromRgba(newRgba);
			Appearance_ColorLabel11->setPixmap(gui::brushValuePixmap(Color[5]));
			Appearance_ColorLabel12->setText(gui::colorValueText(Color[5]));
		}
	}
	else
	if(button == Appearance_ColorToolButton7)
	{
		bool ok = false;
		QRgb oldRgba = Color[6].rgba();
		QRgb newRgba = QColorDialog::getRgba(oldRgba, &ok, this);
		if(ok && newRgba != oldRgba)
		{
			Color[6] = QColor::fromRgba(newRgba);
			Appearance_ColorLabel13->setPixmap(gui::brushValuePixmap(Color[6]));
			Appearance_ColorLabel14->setText(gui::colorValueText(Color[6]));
		}
	}
}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::setToDefaultAppearanceTab()
{

	QString styleName = Settings->value("DefaultStyle").toString();
	Appearance_ComboBox_1->setCurrentIndex(Appearance_ComboBox_1->findText(styleName));

	Font[0].fromString(Settings->value("DefaultFont").toString());
	Appearance_FontLabel1->setPixmap(gui::fontValuePixmap(Font[0]));
	Appearance_FontLabel2->setText(gui::fontValueText(Font[0]));

	Font[1].fromString(Settings->value("DefaultFont").toString());
	Appearance_FontLabel3->setPixmap(gui::fontValuePixmap(Font[1]));
	Appearance_FontLabel4->setText(gui::fontValueText(Font[1]));


	Color[0] = Qt::black;
	Appearance_ColorLabel1->setPixmap(gui::brushValuePixmap(Color[0]));
	Appearance_ColorLabel2->setText(gui::colorValueText(Color[0]));

	Color[1] = Qt::white;
	Appearance_ColorLabel3->setPixmap(gui::brushValuePixmap(Color[1]));
	Appearance_ColorLabel4->setText(gui::colorValueText(Color[1]));

	Color[2] = gui::VizGray;
	Appearance_ColorLabel5->setPixmap(gui::brushValuePixmap(Color[2]));
	Appearance_ColorLabel6->setText(gui::colorValueText(Color[2]));

	Color[3] = gui::VizGreen;
	Appearance_ColorLabel7->setPixmap(gui::brushValuePixmap(Color[3]));
	Appearance_ColorLabel8->setText(gui::colorValueText(Color[3]));

	Color[4] = gui::VizRed;
	Appearance_ColorLabel9->setPixmap(gui::brushValuePixmap(Color[4]));
	Appearance_ColorLabel10->setText(gui::colorValueText(Color[4]));

	Color[5] = gui::VizRed;
	Appearance_ColorLabel11->setPixmap(gui::brushValuePixmap(Color[5]));
	Appearance_ColorLabel12->setText(gui::colorValueText(Color[5]));

	Color[6] = gui::VizBlue;
	Appearance_ColorLabel13->setPixmap(gui::brushValuePixmap(Color[6]));
	Appearance_ColorLabel14->setText(gui::colorValueText(Color[6]));


	Appearance_ComboBox_2->setCurrentIndex(2);

	Appearance_ComboBox_3->setCurrentIndex(0);

	Appearance_ComboBox_5->setCurrentIndex(0);

	Appearance_ComboBox_6->setCurrentIndex(0);

	Appearance_ComboBox_7->setCurrentIndex(1);

	Appearance_ComboBox_8->setCurrentIndex(0);

	Appearance_ComboBox_9->setCurrentIndex(1);

	Appearance_ComboBox_10->setCurrentIndex(0);


}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::loadAppearanceTab()
{
	
	QString tmp;
	bool checked;
	s32 index;

	QString styleName = Settings->value("Appearance/System/Style", gui::styleValueText(QApplication::style())).toString();
	Appearance_ComboBox_1->setCurrentIndex(Appearance_ComboBox_1->findText(styleName));


	tmp = Settings->value("Appearance/DefaultFontAndColor/Font", Settings->value("DefaultFont")).toString();
	Font[0].fromString(tmp);
	Appearance_FontLabel1->setPixmap(gui::fontValuePixmap(Font[0]));
	Appearance_FontLabel2->setText(gui::fontValueText(Font[0]));

	tmp = Settings->value("Appearance/Items/Font", Settings->value("DefaultFont")).toString();
	Font[1].fromString(tmp);
	Appearance_FontLabel3->setPixmap(gui::fontValuePixmap(Font[1]));
	Appearance_FontLabel4->setText(gui::fontValueText(Font[1]));


	tmp = Settings->value("Appearance/DefaultFontAndColor/ForegroundColor", Qt::black).toString();
	Color[0] = QColor(tmp);
	Appearance_ColorLabel1->setPixmap(gui::brushValuePixmap(Color[0]));
	Appearance_ColorLabel2->setText(gui::colorValueText(Color[0]));

	tmp = Settings->value("Appearance/DefaultFontAndColor/BackgroundColor", Qt::white).toString();
	Color[1] = QColor(tmp);
	Appearance_ColorLabel3->setPixmap(gui::brushValuePixmap(Color[1]));
	Appearance_ColorLabel4->setText(gui::colorValueText(Color[1]));

	tmp = Settings->value("Appearance/DefaultFontAndColor/GridColor", VizGray).toString();
	Color[2] = QColor(tmp);
	Appearance_ColorLabel5->setPixmap(gui::brushValuePixmap(Color[2]));
	Appearance_ColorLabel6->setText(gui::colorValueText(Color[2]));

	tmp = Settings->value("Appearance/Charts/UpColor", VizGreen).toString();
	Color[3] = QColor(tmp);
	Appearance_ColorLabel7->setPixmap(gui::brushValuePixmap(Color[3]));
	Appearance_ColorLabel8->setText(gui::colorValueText(Color[3]));

	tmp = Settings->value("Appearance/Charts/DownColor", VizRed).toString();
	Color[4] = QColor(tmp);
	Appearance_ColorLabel9->setPixmap(gui::brushValuePixmap(Color[4]));
	Appearance_ColorLabel10->setText(gui::colorValueText(Color[4]));

	tmp = Settings->value("Appearance/Indicators/Color", VizRed).toString();
	Color[5] = QColor(tmp);
	Appearance_ColorLabel11->setPixmap(gui::brushValuePixmap(Color[5]));
	Appearance_ColorLabel12->setText(gui::colorValueText(Color[5]));

	tmp = Settings->value("Appearance/Items/Color", gui::VizBlue).toString();
	Color[6] = QColor(tmp);
	Appearance_ColorLabel13->setPixmap(gui::brushValuePixmap(Color[6]));
	Appearance_ColorLabel14->setText(gui::colorValueText(Color[6]));

	
	index = Settings->value("Appearance/DefaultFontAndColor/GridStyle", 2).toInt();
	Appearance_ComboBox_2->setCurrentIndex(index);

	index = Settings->value("Appearance/DefaultFontAndColor/GridWidth", 1).toInt() - 1;
	Appearance_ComboBox_3->setCurrentIndex(index);

	index = Settings->value("Appearance/Charts/LineWidth", 1).toInt() - 1;
	Appearance_ComboBox_5->setCurrentIndex(index);

	index = Settings->value("Appearance/Indicators/Style", 0).toInt();
	Appearance_ComboBox_6->setCurrentIndex(index);

	index = Settings->value("Appearance/Indicators/LineStyle", 1).toInt();
	Appearance_ComboBox_7->setCurrentIndex(index);

	index = Settings->value("Appearance/Indicators/LineWidth", 1).toInt() - 1;
	Appearance_ComboBox_8->setCurrentIndex(index);

	index = Settings->value("Appearance/Items/LineStyle", 1).toInt();
	Appearance_ComboBox_9->setCurrentIndex(index);

	index = Settings->value("Appearance/Items/LineWidth", 1).toInt() - 1;
	Appearance_ComboBox_10->setCurrentIndex(index);

	/*
	QApplication::setFont(Font[0]);
	QApplication::setStyle(QStyleFactory::create(styleName));
	*/


}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::saveAppearanceTab()
{

	QString tmp;
	bool checked;
	s32 index;

	QString styleName = Appearance_ComboBox_1->currentText();
	Settings->setValue("Appearance/System/Style", styleName);
	
	Settings->setValue("Appearance/DefaultFontAndColor/Font", Font[0]);
	Settings->setValue("Appearance/Items/Font", Font[1]);
	Settings->setValue("Appearance/DefaultFontAndColor/ForegroundColor", Color[0]);
	Settings->setValue("Appearance/DefaultFontAndColor/BackgroundColor", Color[1]);
	Settings->setValue("Appearance/DefaultFontAndColor/GridColor", Color[2]);
	Settings->setValue("Appearance/Charts/UpColor", Color[3]);
	Settings->setValue("Appearance/Charts/DownColor", Color[4]);
	Settings->setValue("Appearance/Indicators/Color", Color[5]);
	Settings->setValue("Appearance/Items/Color", Color[6]);

	Settings->setValue("Appearance/DefaultFontAndColor/GridStyle", Appearance_ComboBox_2->currentIndex());
	Settings->setValue("Appearance/DefaultFontAndColor/GridWidth", Appearance_ComboBox_3->currentIndex()+1);
	Settings->setValue("Appearance/Charts/LineWidth", Appearance_ComboBox_5->currentIndex()+1);
	
	Settings->setValue("Appearance/Indicators/Style", Appearance_ComboBox_6->currentIndex());
	Settings->setValue("Appearance/Indicators/LineStyle", Appearance_ComboBox_7->currentIndex());
	Settings->setValue("Appearance/Indicators/LineWidth", Appearance_ComboBox_8->currentIndex()+1);
	Settings->setValue("Appearance/Items/LineStyle", Appearance_ComboBox_9->currentIndex());
	Settings->setValue("Appearance/Items/LineWidth", Appearance_ComboBox_10->currentIndex()+1);

}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::createPerformanceTab()
{
	//u64 bytes = system::VizCMemoryUtils::availableSystemMemory();
	TotalSystemMemory = system::VizCMemoryUtils::totalSystemMemory();
	f64 out;
	QString suffix;
	system::VizCMemoryUtils::formatByteSize(TotalSystemMemory, out, suffix);
	QString ms = QString("%1 %2").arg(out, 0, 'f', 2).arg(suffix);
	TextLabelMemoryRAM->setText(ms);


	QObject::connect(Performance_SpinBoxMemoryRAM, SIGNAL(valueChanged(int)), this, SLOT(slotMemoryPercentValueChanged(int)));
	

}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::slotMemoryPercentValueChanged(int i)
{

	f64 m = TotalSystemMemory * (i * 0.01);

	if(i == 0)
	{
		m = 1 * 1024 * 1024;
	}

	f64 out;
	QString suffix;
	system::VizCMemoryUtils::formatByteSize(m, out, suffix);
	QString ms = QString("%1 %2").arg(out, 0, 'f', 2).arg(suffix);

	TextLabelMemoryRAMPercent->setText(ms);

}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::loadPerformanceTab()
{
	bool checked;
	checked = Settings->value("Performance/Render/AntiAliasing", true).toBool();
	Performance_CheckBox_1->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
	checked = Settings->value("Performance/Render/UseGradientToFillShapes", true).toBool();
	Performance_CheckBox_2->setCheckState(checked ? Qt::Checked : Qt::Unchecked);

	s32 memoryPercent;
	memoryPercent = Settings->value("Performance/Memory/MaxPercentOfMemoryToCache", 25).toInt();
	Performance_SpinBoxMemoryRAM->setValue(memoryPercent);
	
}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::savePerformanceTab()
{
	bool checked;
	checked = Performance_CheckBox_1->checkState() == Qt::Checked ? true : false;
	Settings->setValue("Performance/Render/AntiAliasing", checked);
	checked = Performance_CheckBox_2->checkState() == Qt::Checked ? true : false;
	Settings->setValue("Performance/Render/UseGradientToFillShapes", checked);

	s32 memoryPercent = Performance_SpinBoxMemoryRAM->value();
	Settings->setValue("Performance/Memory/MaxPercentOfMemoryToCache", memoryPercent);

}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::setToDefaultPerformanceTab()
{
	Performance_CheckBox_1->setCheckState(Qt::Checked);
	Performance_CheckBox_2->setCheckState(Qt::Checked);
	Performance_SpinBoxMemoryRAM->setValue(25.0);
}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::createNetworkTab()
{
}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::loadNetworkTab()
{
	bool enable = Settings->value("Network/Proxy/Enabled", false).toBool();
	QString host = Settings->value("Network/Proxy/Host", "").toString();
	s32 port = Settings->value("Network/Proxy/Port", 1080).toInt();
	QString user = Settings->value("Network/Proxy/User", "").toString();
	QString pass = Settings->value("Network/Proxy/Password", "").toString();

	Network_groupBox1->setChecked(enable);
	Network_lineEdit1->setText(host);
	Network_spinBox1->setValue(port);
	Network_lineEdit2->setText(user);
	Network_lineEdit3->setText(pass);
}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::saveNetworkTab()
{
	bool enable = Network_groupBox1->isChecked() ? true : false;
	QString host = Network_lineEdit1->text();
	s32 port = Network_spinBox1->value();
	QString user = Network_lineEdit2->text();
	QString pass = Network_lineEdit3->text();

	Settings->setValue("Network/Proxy/Enabled", enable);
	Settings->setValue("Network/Proxy/Host", host);
	Settings->setValue("Network/Proxy/Port", port);
	Settings->setValue("Network/Proxy/User", user);
	Settings->setValue("Network/Proxy/Password", pass);
}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::setToDefaultNetworkTab()
{
	Network_groupBox1->setChecked(false);
	Network_spinBox1->setValue(1080);
	Network_lineEdit1->setText("");
	Network_lineEdit2->setText("");
	Network_lineEdit3->setText("");
}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::load()
{
	loadGeneralTab();
	loadAppearanceTab();
	loadPerformanceTab();
	loadNetworkTab();
}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::save()
{
	saveGeneralTab();
	saveAppearanceTab();
	savePerformanceTab();
	saveNetworkTab();
}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::loadAndShow()
{
	load();
	show();
}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::slotResetToDefault()
{

	QMessageBox msgBox;
	msgBox.setWindowTitle(tr("Valores padrão"));
	msgBox.setText(tr("Restaurar valores padrão para:"));
	//msgBox.setInformativeText("Restaurar valores padrão para todas as abas?");

	QPushButton* b1 = msgBox.addButton(tr("Somente esta aba"), QMessageBox::YesRole);
	QPushButton* b2 = msgBox.addButton(tr("Todas as abas"), QMessageBox::NoRole);
	QPushButton* b3 = msgBox.addButton(tr("Cancelar"), QMessageBox::RejectRole);
	msgBox.setDefaultButton(b1);
	msgBox.exec();

	if(msgBox.clickedButton() == b1)
	{
		s32 index = TabWidget->currentIndex();
		if(index == 0)
		{
			setToDefaultGeneralTab();
		}
		else
		if(index == 1)
		{
			setToDefaultAppearanceTab();

		}
		else
		if(index == 2)
		{
			setToDefaultPerformanceTab();
		}
		else
		if(index == 3)
		{
			setToDefaultNetworkTab();
		}
	}
	else
	if(msgBox.clickedButton() == b2)
	{
		setToDefaultGeneralTab();
		setToDefaultAppearanceTab();
		setToDefaultPerformanceTab();
		setToDefaultNetworkTab();
	}

}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::slotApply()
{
	save();
	emit settingsChange();
}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::slotCancel()
{
	close();
}
//-----------------------------------------------------------------------------
void VizCApplicationSettings::slotOk()
{
	save();
	close();
	emit settingsChange();
}
//-----------------------------------------------------------------------------
VizCApplicationSettings* VizCApplicationSettings::instance()
{
	if(!Instance)
	{
		Instance = new VizCApplicationSettings;
	}
	return Instance;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
