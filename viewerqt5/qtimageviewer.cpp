#include "qtimageviewer.hpp"

#include <QFileDialog>
#include <QMainWindow>
#include <QPalette>
#include <QMenu>
#include <QMenuBar>
#include <QImageReader>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolBar>
#include <initializer_list>
#include <QAbstractItemView>
#include <QFrame>
#include <QLabel>
#include <QScrollArea>
#include <QStackedWidget>
#include <QListWidget>
#include <QHBoxLayout>

#include <tiffloader.hpp> /// include your image loader
#include <imageprocessor.hpp> // including image processor

#include <iostream>

QtImageViewer::QtImageViewer(QWidget *parent) :
  QMainWindow(parent){
  init();
};

void QtImageViewer::init(){
    // Keep the original left/right image comparison layout.
    _lImageLabel = new QLabel;
    _lImageLabel->setBackgroundRole(QPalette::Base);
    _lImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    _lImageLabel->setScaledContents(false);

    _lScrollArea = new QScrollArea;
    _lScrollArea->setBackgroundRole(QPalette::Dark);
    _lScrollArea->setWidget(_lImageLabel);
    _lScrollArea->setVisible(true);

    _rImageLabel = new QLabel;
    _rImageLabel->setBackgroundRole(QPalette::Base);
    _rImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    _rImageLabel->setScaledContents(false);

    _rScrollArea = new QScrollArea;
    _rScrollArea->setBackgroundRole(QPalette::Dark);
    _rScrollArea->setWidget(_rImageLabel);
    _rScrollArea->setVisible(true);

    _mainSplitter = new QSplitter;
    _leftSplitter = new QSplitter(Qt::Vertical);
    _rightSplitter = new QSplitter(Qt::Vertical);
    _mainSplitter->addWidget(_leftSplitter);
    _mainSplitter->addWidget(_rightSplitter);
    _leftSplitter->addWidget(_lScrollArea);
    _rightSplitter->addWidget(_rScrollArea);
    setCentralWidget(_mainSplitter);

    // These QActions are already connected to the existing processing slots.
    createActions();

    // Operations dock: categories on the left, selected category's tools on the right.
    auto* operationsDock = new QDockWidget(tr("Operations"), this);
    operationsDock->setObjectName("operationsDock");
    operationsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    operationsDock->setFeatures(QDockWidget::DockWidgetMovable |
                                QDockWidget::DockWidgetFloatable |
                                QDockWidget::DockWidgetClosable);

    auto* dockContents = new QWidget(operationsDock);
    dockContents->setObjectName("operationsContainer");
    auto* dockLayout = new QHBoxLayout(dockContents);
    dockLayout->setContentsMargins(7, 8, 7, 8);
    dockLayout->setSpacing(8);

    // Category navigation (never expands; switching changes the right-hand page).
    auto* categories = new QListWidget(dockContents);
    categories->setObjectName("operationCategories");
    categories->setFixedWidth(125);
    categories->setFrameShape(QFrame::NoFrame);
    categories->setSpacing(3);
    categories->setUniformItemSizes(true);
    categories->setSelectionMode(QAbstractItemView::SingleSelection);
    categories->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    categories->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    categories->setFocusPolicy(Qt::NoFocus);

    categories->addItem(tr("Transforms"));
    categories->addItem(tr("LUT"));
    categories->addItem(tr("Spatial"));
    categories->addItem(tr("Utilities"));

    // Each page is a vertically scrollable selection of tool buttons.
    auto* operationPages = new QStackedWidget(dockContents);
    operationPages->setObjectName("operationPages");

    struct OperationEntry {
        QString label;
        QAction* action;
    };

    // One common helper makes all pages look and behave consistently.
    // No new slots or members are required in qtimageviewer.hpp.
    auto addPage = [&](const QString& title,
                       const QString& subtitle,
                       std::initializer_list<OperationEntry> operations) {
        auto* content = new QWidget;
        content->setObjectName("operationPage");
        auto* contentLayout = new QVBoxLayout(content);
        contentLayout->setContentsMargins(8, 8, 8, 8);
        contentLayout->setSpacing(7);

        auto* heading = new QLabel(title, content);
        heading->setObjectName("operationPageTitle");
        contentLayout->addWidget(heading);

        auto* description = new QLabel(subtitle, content);
        description->setObjectName("operationPageSubtitle");
        description->setWordWrap(true);
        contentLayout->addWidget(description);
        contentLayout->addSpacing(7);

        for (const auto& op : operations) {
            if (!op.action) continue;
            auto* button = new QPushButton(op.label, content);
            button->setObjectName("operationButton");
            button->setMinimumHeight(42);
            button->setCursor(Qt::PointingHandCursor);
            button->setToolTip(op.action->text());
            button->setEnabled(op.action->isEnabled());
            if (!op.action->icon().isNull())
                button->setIcon(op.action->icon());
            QAction* operation = op.action;
            connect(button, &QPushButton::clicked, this,
                    [operation]() {
                        if (operation && operation->isEnabled())
                            operation->trigger();
                    });
            contentLayout->addWidget(button);
        }
        contentLayout->addStretch();

        auto* scroll = new QScrollArea(operationPages);
        scroll->setObjectName("operationPageScroll");
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);
        scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scroll->setWidget(content);
        operationPages->addWidget(scroll);
    };

    addPage(tr("Transforms"), tr("Basic intensity adjustments"), {
        {tr("Negation"), _trNegate},
        {tr("Power law (Gamma)"), _trPowerlaw},
        {tr("Piecewise linear"), _trLinear},
        {tr("Thresholding"), _trThresholding},
        {tr("Histogram equalization"), _trHistogrameq}
    });

    addPage(tr("LUT transformations"), tr("Lookup table versions"), {
        {tr("Negation (LUT)"), _trNegateLUT},
        {tr("Power law (LUT)"), _trPowerlawLUT},
        {tr("Piecewise linear (LUT)"), _trLinearLUT},
        {tr("Thresholding (LUT)"), _trThresholdingLUT}
    });

    addPage(tr("Spatial filtering"), tr("Smoothing and sharpening"), {
        {tr("Lowpass filtering"), _spatialLowpass},
        {tr("Median filtering"), _spatialMedian},
        {tr("Laplacian"), _spatialLaplacian},
        {tr("Unsharp mask"), _spatialUnsharp},
        {tr("Multistep processing"), _spatialMulti}
    });

    addPage(tr("Utilities"), tr("Image conversion and composition"), {
        {tr("Convert to grayscale"), _trTogreyscale},
        {tr("Combine RGB channels"), _trCombine}
    });

    connect(categories, &QListWidget::currentRowChanged,
            operationPages, &QStackedWidget::setCurrentIndex);
    categories->setCurrentRow(0);

    dockLayout->addWidget(categories);
    dockLayout->addWidget(operationPages, 1);

    // Scoped QSS so it doesn't restyle the two image viewers or charts.
    dockContents->setStyleSheet(R"QSS(
        QWidget#operationsContainer {
            background-color: #1c222a;
            color: #e5eaf1;
        }
        QListWidget#operationCategories {
            background-color: #242c36;
            border: none;
            border-radius: 9px;
            color: #bac5d2;
            padding: 3px;
            outline: none;
            font-size: 12px;
        }
        QListWidget#operationCategories::item {
            padding: 12px 7px;
            border-radius: 6px;
            margin: 1px;
        }
        QListWidget#operationCategories::item:hover {
            background-color: #35414f;
        }
        QListWidget#operationCategories::item:selected {
            background-color: #315b61;
            color: #ffffff;
        }
        QStackedWidget#operationPages, QWidget#operationPage,
        QScrollArea#operationPageScroll,
        QScrollArea#operationPageScroll > QWidget > QWidget {
            background-color: #1c222a;
        }
        QLabel#operationPageTitle {
            color: #f4f7fb;
            font-weight: bold;
            font-size: 15px;
        }
        QLabel#operationPageSubtitle {
            color: #aab7c6;
            font-size: 11px;
        }
        QPushButton#operationButton {
            background-color: #29323d;
            color: #e5eaf1;
            border: 1px solid #3b4857;
            border-radius: 7px;
            padding: 10px 12px;
            text-align: left;
            font-size: 12px;
        }
        QPushButton#operationButton:hover {
            background-color: #364454;
            border-color: #6ba6a8;
        }
        QPushButton#operationButton:pressed {
            background-color: #315b61;
        }
        QPushButton#operationButton:disabled {
            background-color: #242a32;
            color: #727c87;
        }
    )QSS");

    operationsDock->setWidget(dockContents);
    addDockWidget(Qt::LeftDockWidgetArea, operationsDock);
    resizeDocks({operationsDock}, {395}, Qt::Horizontal);
}

QtImageViewer::~QtImageViewer(){
	delete(_lImageLabel);
	delete(_lScrollArea);
	delete(_rImageLabel);
	delete(_rScrollArea);
	delete(_fileMenu);
	delete(_fileOpenAction);
	delete(_fileClearAction);
	delete(_quitAction);

	delete(_currentImage);
};

void QtImageViewer::createActions(){

	_fileOpenAction = new QAction(tr("&Open..."), this);
	_fileOpenAction->setShortcut(QKeySequence::Open);
	connect(_fileOpenAction, SIGNAL(triggered()), this, SLOT(openFile()));

	_fileClearAction = new QAction(tr("&Clear display..."), this);
	_fileClearAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Delete));
	connect(_fileClearAction, SIGNAL(triggered()), this, SLOT(clearFile()));
	
	_quitAction = new QAction(tr("&Quit..."), this);
	_quitAction->setShortcut(QKeySequence::Quit);
	connect(_quitAction, SIGNAL(triggered()), this, SLOT(quit()));

	_trFourierTransform = new QAction(tr("&Fourier transform..."), this);
	_trFourierTransform->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));

	_trTogreyscale = new QAction(tr("Convert image to greyscale..."), this);
	_trTogreyscale->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
	connect(_trTogreyscale, SIGNAL(triggered()), this, SLOT(togreyscale()));

	_trCombine = new QAction(tr("&Combine images into RGB..."), this);
	_trCombine->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));
	connect(_trCombine, SIGNAL(triggered()), this, SLOT(combineImagesRGB()));
	
	_trNegate = new QAction(tr("Image negation..."), this);
	_trNegate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
	connect(_trNegate, SIGNAL(triggered()), this, SLOT(negate()));

	_trNegateLUT = new QAction(tr("Image negation using LUT..."), this);
	_trNegateLUT->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_N));
	connect(_trNegateLUT, SIGNAL(triggered()), this, SLOT(negateLUT()));

	_trPowerlaw = new QAction(tr("Power law..."), this);
	_trPowerlaw->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
	connect(_trPowerlaw, SIGNAL(triggered()), this, SLOT(powerlaw()));

	_trPowerlawLUT = new QAction(tr("Power law using LUT..."), this);
	_trPowerlawLUT->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_P));
	connect(_trPowerlawLUT, SIGNAL(triggered()), this, SLOT(powerlawLUT()));

	_trLinear = new QAction(tr("Piece-wise linear..."), this);
	_trLinear->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
	connect(_trLinear, SIGNAL(triggered()), this, SLOT(linear()));

	_trLinearLUT = new QAction(tr("Linear using LUT..."), this);
	_trLinearLUT->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_L));
	connect(_trLinearLUT, SIGNAL(triggered()), this, SLOT(linearLUT()));

	_trThresholding = new QAction(tr("Thresholding..."), this);
	_trThresholding->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_T));
	connect(_trThresholding, SIGNAL(triggered()), this, SLOT(thresholding()));

	_trThresholdingLUT = new QAction(tr("Thresholding using LUT..."), this);
	_trThresholdingLUT->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_T));
	connect(_trThresholdingLUT, SIGNAL(triggered()), this, SLOT(thresholdingLUT()));

	_trHistogrameq = new QAction(tr("Histogram equalization..."), this);
	_trHistogrameq->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_H));
	connect(_trHistogrameq, SIGNAL(triggered()), this, SLOT(histogrameq()));

	_spatialLowpass = new QAction(tr("Lowpass filtering..."), this);
	_spatialLowpass->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
	connect(_spatialLowpass, SIGNAL(triggered()), this, SLOT(lowpass()));

	_spatialMedian = new QAction(tr("Median filtering..."), this);
	_spatialMedian->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
	connect(_spatialMedian, SIGNAL(triggered()), this, SLOT(median()));

	_spatialLaplacian = new QAction(tr("Laplacian transform..."), this);
	_spatialLaplacian->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_X));
	connect(_spatialLaplacian, SIGNAL(triggered()), this, SLOT(laplacian()));

	_spatialUnsharp = new QAction(tr("Unsharp transform..."), this);
	_spatialUnsharp->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_U));
	connect(_spatialUnsharp, SIGNAL(triggered()), this, SLOT(unsharp()));

	_spatialMulti = new QAction(tr("Multi transform..."), this);
	_spatialMulti->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
	connect(_spatialMulti, SIGNAL(triggered()), this, SLOT(multi()));

	_fileMenu = menuBar()->addMenu(tr("&File"));
	_fileMenu->addAction(_fileOpenAction);
	_fileMenu->addAction(_fileClearAction);
	_fileMenu->addAction(_quitAction);

	_trMenu = menuBar()->addMenu(tr("&Transforms"));
	_trMenu->addAction(_trFourierTransform);
	_trMenu->addAction(_trTogreyscale);
	_trMenu->addAction(_trCombine);
	_trMenu->addAction(_trNegate);
	_trMenu->addAction(_trNegateLUT);
	_trMenu->addAction(_trPowerlaw);
	_trMenu->addAction(_trPowerlawLUT);
	_trMenu->addAction(_trLinear);
	_trMenu->addAction(_trLinearLUT);
	_trMenu->addAction(_trThresholding);
	_trMenu->addAction(_trThresholdingLUT);
	_trMenu->addAction(_trHistogrameq);
	
	_spatialMenu = menuBar()->addMenu(tr("&Spatial filtering"));
	_spatialMenu->addAction(_spatialLowpass);
	_spatialMenu->addAction(_spatialMedian);
	_spatialMenu->addAction(_spatialLaplacian);
	_spatialMenu->addAction(_spatialUnsharp);
	_spatialMenu->addAction(_spatialMulti);

	_toolsMenu = menuBar()->addMenu(tr("&Tools"));
};

void QtImageViewer::openFile(){
	QFileDialog dialog(this, tr("Open File"));
	QString filename = dialog.getOpenFileName(this, "Select image to open");
	std::cout<<"Opening: "<<filename.toStdString()<<std::endl;

	TiffLoader* tiffLoader = new TiffLoader(filename.toStdString());
	tiffLoader->printMetaData();
	Image<>* myImage = tiffLoader->loadImage();

	showImageLeft(myImage);
	delete(myImage);
};

void QtImageViewer::showImage(Image<> *img, ImageView imageView){

	// get widgets for requested side
	QtCharts::QChartView **chartView{nullptr};
	QSplitter **splitter{nullptr};
	QLabel **imageLabel{nullptr};
	QScrollArea **scrollArea{nullptr};

	bool alreadyShown = false;

	switch(imageView){

	case ImageView::LEFT:
    	alreadyShown = (_imageState & ImageView::LEFT);

    	chartView = &_lChartView;
    	splitter = &_leftSplitter;
    	imageLabel = &_lImageLabel;
    	scrollArea = &_lScrollArea;
    	break;

	case ImageView::RIGHT:
    	alreadyShown = (_imageState & ImageView::RIGHT);

    	chartView = &_rChartView;
    	splitter = &_rightSplitter;
    	imageLabel = &_rImageLabel;
    	scrollArea = &_rScrollArea;
    	break;


	default:
		std::cout<<"showImage - error - image view not correctly specified: "<<imageView<<std::endl;
		break;
	}

	QImage::Format format = QImage::Format_Invalid;
	std::vector<unsigned int> hist{};

	if(img->getChannels() == 3){
		format = QImage::Format_RGB888;
		std::cout<<"Setting empty histogram for RGB image"<<std::endl;
		hist = std::vector<unsigned int>(256,0);
	}
	else if (img->getChannels() == 1){
		format = QImage::Format_Grayscale8;
		// Diagnostic only: temporarily bypass Image<>::getHistogram() to isolate the ASan read.
		std::cerr << "Histogram bypass: " << img->getWidth() << "x" << img->getHeight()
		          << ", channels=" << img->getChannels() << ", bpc=" << img->getBpc() << "\n";
		hist.assign(256, 0);
	}

	QtCharts::QBarSet *bset = new QtCharts::QBarSet("Intensities");
	unsigned int maxInt{0};
	for(unsigned int i : hist) { (*bset) << i; maxInt = std::max(maxInt,i);
		//std::cout<<i<<std::endl;
	}

	QtCharts::QBarSeries *bseries = new QtCharts::QBarSeries();
	bseries->append(bset);

	QtCharts::QChart *chart = new QtCharts::QChart();
	chart->addSeries(bseries);
	chart->setTitle("Intensity Histogram");
	chart->setAnimationOptions(QtCharts::QChart::NoAnimation);

	QtCharts::QValueAxis *xAxis = new  QtCharts::QValueAxis();
	xAxis->setRange(0,255);

	QtCharts::QValueAxis *yAxis = new QtCharts::QValueAxis();
	yAxis->setRange(0,maxInt);

	// Seems you must add axis to chart first, then to bseries
	chart->addAxis(xAxis, Qt::AlignBottom);
	chart->addAxis(yAxis, Qt::AlignLeft);

	bseries->attachAxis(xAxis);
	bseries->attachAxis(yAxis);
	bset->setBorderColor(QColor("black"));

	chart->legend()->setVisible(true);
	chart->legend()->setAlignment(Qt::AlignBottom);

	if(alreadyShown){
    // Replace the existing histogram
    QtCharts::QChart* oldChart = (*chartView)->chart();

    (*chartView)->setChart(chart);

    delete oldChart;
	}
	else{
    	// Create the histogram viewer only once
    	(*chartView) = new QtCharts::QChartView(chart);

    	(*chartView)->setRenderHint(QPainter::Antialiasing);

    	(*splitter)->addWidget(*chartView);

   		// Mark this side as initialized
    	_imageState |= imageView;
	}

	// Copy image data to Qt
	std::cout<<"Width: "<<img->getWidth()
			 <<"\nHeight: "<<img->getHeight()
			 <<"\nBPC: "<<img->getBpc()
			 <<"\nBytes per line: "<<img->getWidth()*img->getChannels()*img->getBpc()/8<<std::endl;
		
	QImage qImg(img->getImageData(),
				img->getWidth(),
				img->getHeight(),
				img->getWidth()*img->getChannels()*img->getBpc()/8,
				format);

	// Tell Qt to show this image data
	(*imageLabel)->setPixmap(QPixmap::fromImage(qImg));
	(*imageLabel)->resize((*imageLabel)->pixmap()->size());
	(*scrollArea)->setVisible(true);

	update(); // For Qt to redraw with new image
};



void QtImageViewer::showImageLeft(Image<> *img) {
	showImage(img, ImageView::LEFT);
};

void QtImageViewer::showImageRight(Image<> *img) {
	showImage(img, ImageView::RIGHT);
};

void QtImageViewer::setCurrentImage(Image<uint8_t>* img){
	if(img){
		_currentImage = img;
	}
};

void QtImageViewer::clearFile(){};

void QtImageViewer::togreyscale(){
	ImageProcessor processor;

	QString filename = QFileDialog::getOpenFileName(this, "Select image for conversion");
	if(filename.isEmpty()){
		return;
	}
	TiffLoader trloader(filename.toStdString());
	Image<>* rgb = trloader.loadImage();
	Image<>* gsc = processor.togreyscale(rgb);
	showImageRight(gsc);
	delete gsc;
};

void QtImageViewer::combineImagesRGB() {
    ImageProcessor processor;

    QString redFile   = QFileDialog::getOpenFileName(this, "Select RED channel TIFF");
    if(redFile.isEmpty()) return;

    QString greenFile = QFileDialog::getOpenFileName(this, "Select GREEN channel TIFF");
    if(greenFile.isEmpty()) return;

    QString blueFile  = QFileDialog::getOpenFileName(this, "Select BLUE channel TIFF");
    if(blueFile.isEmpty()) return;

    TiffLoader redLoader(redFile.toStdString());
    Image<>* red = redLoader.loadImage();

    TiffLoader greenLoader(greenFile.toStdString());
    Image<>* green = greenLoader.loadImage();

    TiffLoader blueLoader(blueFile.toStdString());
    Image<>* blue = blueLoader.loadImage();

    if(!red || !green || !blue) {
        std::cout << "Failed to load one of the images." << std::endl;
        return;
    }

    Image<>* rgb = processor.combineRGB(red, green, blue);

    if(!rgb) {
        std::cout << "RGB combination failed." << std::endl;
        return;
    }

    showImageRight(rgb);
	QString filename = QFileDialog::getSaveFileName(this, "Save as...");
	if(!filename.isEmpty()) {
		QImage image (
			rgb->getImageData(),
			rgb->getWidth(),
			rgb->getHeight(),
			rgb->getWidth() * 3,
			QImage::Format_RGB888
		);
		QImage copy = image.copy();
		copy.save(filename);
	}
  
    delete red;
    delete green;
    delete blue;
	delete rgb;
};

void QtImageViewer::negate() {
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select Image for transformation...");
    if(filename.isEmpty()) return;
	
	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.negationtr(img);
	showImageRight(img);
	delete img; // make this work somehow with qt getting ownership of the image. Maybe also merge functions somehow?
};

void QtImageViewer::negateLUT() {
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select Image for transformation...");
    if(filename.isEmpty()) return;
	
	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.negationlut(img);
	showImageRight(img);

	delete img;
};

void QtImageViewer::powerlaw(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for transformation...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.powerlawtr(img);
	showImageRight(img);

	delete img;
};

void QtImageViewer::powerlawLUT(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for transformation...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.powerlawlut(img);
	showImageRight(img);

	delete img;
};

void QtImageViewer::linear(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for transformation...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.lineartr(img);
	showImageRight(img);
	delete img;
};

void QtImageViewer::linearLUT(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for transformation...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.linearlut(img);
	showImageRight(img);
	delete img;
};

void QtImageViewer::thresholding(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for thresholding...");
	if(filename.isEmpty()) return;
	
	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.thresholdtr(img);
	showImageRight(img);
	delete img;
};

void QtImageViewer::thresholdingLUT(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for thresholding...");
	if(filename.isEmpty()) return;
	
	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.thresholdlut(img);
	showImageRight(img);
	delete img;
};

void QtImageViewer::histogrameq(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for equalization...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.histogramtr(img);
	showImageRight(img);
	delete img;
};

void QtImageViewer::lowpass(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for lowpass filtering...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	Image<>* filtered = processor.lowpass(img);
	showImageRight(filtered);
	delete img;
	delete filtered;
};
void QtImageViewer::median(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for median filtering...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	Image<>* filtered = processor.median(img);
	showImageRight(filtered);
	delete img;
	delete filtered;
};
void QtImageViewer::laplacian(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for laplacian transform...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	Image<>* filtered = processor.laplacian(img);
	showImageRight(filtered);
	delete img;
	delete filtered;
};
void QtImageViewer::unsharp(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for unsharp transform...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	Image<>* filtered = processor.unsharp(img);
	showImageRight(filtered);
	delete img;
	delete filtered;
};
void QtImageViewer::multi(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for multi transform...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	Image<>* filtered = processor.multi(img);
	showImageRight(filtered);
	delete img;
	delete filtered;
};

void QtImageViewer::quit(){
	close();
};